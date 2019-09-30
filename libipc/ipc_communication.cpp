#include <cstdio>
#include <iostream>
#include <csignal>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <thread>
#include <fcntl.h>

#include "ipc_communication.h"
#include "nsjail/fork.h"
#include "nsjail/api.h"
#include "shared_memory_manger.h"
#include "malloctest/malloc_tester.h"
#include "mappings.h"
#include "debug.h"


static struct sembuf sema_inc = {0, 1, 0};
static struct sembuf sema_dec = {0, -1, 0};

#define switch(x, y) do {typeof(x) tmp = x; (x) = y; y = tmp; } while(false)


process_local CallbackManager global_callback_manager;

IpcCommunication *IpcCommunication::last_instance;

#ifdef NO_FORK_MODE
extern "C" {
	void *ipc_get_sending_struct();
	void setCommunicationInstance(IpcCommunication *inst);
}
#endif

IpcCommunication::IpcCommunication(size_t sending_size, size_t receiving_size, ipc_call_processor processor, ipc_call_processor library_processor,
								   nsjail_configurator configurator, bool fork_now)
		: processor(processor), library_processor(library_processor), configurator(configurator) {
	// timing_debug("hello world");
	// Initialize globals //TODO only once
	shared_memory_init();
	remap_library_segments();

	// Initialize shared memory
	sending_data = shm_malloc_2(sending_size);
	receiving_data = shm_malloc_2(receiving_size);
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", sending_size, sending_data, pthread_self());
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", receiving_size, receiving_data, pthread_self());

	sending_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_RMID | 0600);
	if (sending_semaphore < 0) perror("semget() 1");
	receiving_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_RMID | 0600);
	if (receiving_semaphore < 0) perror("semget() 2");

	/* union semun {
		int              val;    // Value for SETVAL
		struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET
		unsigned short  *array;  // Array for GETALL, SETALL
		struct seminfo  *__buf;  // Buffer for IPC_INFO (Linux-specific)
	} setzero;
	setzero.val = 0;
	semctl(receiving_semaphore, 0, SETVAL, setzero);
	semctl(sending_semaphore, 0, SETVAL, setzero); */

	if (fork_now) fork_receiving_process();
}


IpcCommunication::~IpcCommunication() {
	semctl(sending_semaphore, 0, IPC_RMID);
	semctl(receiving_semaphore, 0, IPC_RMID);
	debug_printf("IPC calls: %d", ipc_count);
}


void IpcCommunication::switch_sender_receiver() {
	switch(sending_semaphore, receiving_semaphore);
	switch(sending_data, receiving_data);
	switch(processor, library_processor);
	fileDescriptors.setLibrary();
	is_library = true;
}


void IpcCommunication::trigger_ipc_call() {
	shm_msync();
	ipc_count++;
	// fprintf(stderr, "[%d] Trigger with sending data %p\n", is_library, sending_data);
	semop(sending_semaphore, &sema_inc, 1);
	// printf("Done\n");
}


typedef struct communication_t {
	int code;
	void *callback;
} communication_t;


void *IpcCommunication::wait_for_return() {
	while (true) {
		while (semop(receiving_semaphore, &sema_dec, 1) < 0) {
			if (errno == EIDRM) exit(0);
			unregister_shared_memory_as_default();
			perror("semop() wait in thread");
			if (errno == EINVAL) exit(1);
			reregister_shared_memory_as_default();
		}

		int code = ((communication_t *) receiving_data)->code;
		// fprintf(stderr, "[%d] Receiving from %p: %d\n", is_library, receiving_data, code);
		if (code == 1) {
			break;
		} else if (code == -1) {
			fprintf(stderr, "Remote exit requested.\n");
			exit(1);
		} else if (code == 2) {
			// callback
			void *callback = ((communication_t *) receiving_data)->callback;
			global_callback_manager.execute(callback, receiving_data, sending_data);

		} else if (code > 2) {
			processor(receiving_data);
		} else {
			fprintf(stderr, "Invalid return code: %d\n", code);
		}

		// fprintf(stderr, "[%d] Sending response to %p : %d\n", is_library, sending_data, ((communication_t*)sending_data)->code);

		shm_msync();
		while (semop(sending_semaphore, &sema_inc, 1) < 0) {
			if (errno == EIDRM) exit(0);
			unregister_shared_memory_as_default();
			perror("semop() wait for result in thread");
			if (errno == EINVAL) exit(1);
			reregister_shared_memory_as_default();
		}
	}
	return receiving_data;
}


static void add_parent_dependency() {
	if (prctl(PR_SET_PDEATHSIG, SIGTERM) == -1) perror("prctl() - no parent dependency set");
}


void IpcCommunication::child_process_main() {
#ifndef NO_FORK_MODE
	add_parent_dependency();
	// int p;
	// fprintf(stderr, "My stack: %p\n", &p);
	// print_mapping();
#else
	setCommunicationInstance(this);
#endif
	register_shared_memory_as_default();
	switch_sender_receiver();
	//TODO
	// system("echo -n \"Welcome from IPC jail: \" && id && cat /proc/1/cgroup");
	try {
		wait_for_return();
	} catch (...) {
		((communication_t *) sending_data)->code = -1;
		semop(sending_semaphore, &sema_inc, 1);
		_exit(1);
	}
}

static void *old_rsp;

static void child_process() {
	//TODO
	/*register void *rsp asm ("rsp");
	old_rsp = rsp;
	rsp = get_new_stack();
	unmap_stack(old_rsp);*/
	IpcCommunication::last_instance->child_process_main();
}


static std::unique_ptr<struct nsjconf_t> nsjail_config;


void IpcCommunication::fork_receiving_process() {
	shared_memory_forking();
#ifndef NO_FORK_MODE
	last_instance = this;
	/*
	pid_t pid = fork();
	if (pid < 0) perror("fork()");
	else if (pid == 0){
		child_process();
		exit(0);
	}
	return;
	// */
	if (use_nsjail()) {
		nsjail_config = nsjail_default_config();
		// keep shm open
		nsjail_config_add_open_fd(nsjail_config.get(), shared_memory_fd);
		// max file size -> max shm (1GB)
		nsjail_config->rl_fsize = 4L * 1024 * 1024 * 1024;
		nsjail_config->cwd = getcwd(nullptr, 0);
		if (configurator)
			configurator(nsjail_config.get());
		nsjail_fork(nsjail_config.get(), &child_process);
	} else {
		if (fork() == 0) {
			child_process();
			exit(0);
		}
	}
#else
	// Use threads instead of fork
	last_instance = new IpcCommunication(*this);
	// fprintf(stderr, "--- New thread spawned ---\n");
	std::thread thread(child_process);
	thread.detach();
#endif
	// Parent process here
	fileDescriptors.setParent();
}


static inline bool callback_needs_wrapping(void *callback) {
	return callback != nullptr && callback != (void *) -1;
}


void *CallbackManager::wrap_outgoing_callback(void *callback, void (*wrapper)(void *, void *, void *)) {
	if (!callback_needs_wrapping(callback)) return callback;
	auto number = callback_numbers.find(callback);
	if (number != callback_numbers.end()) return (void *) number->second;
	callback_functions.emplace_back(callback, wrapper);
	return (void *) (callback_functions.size() - 1);
}


void *CallbackManager::wrap_incoming_callback(void *callback, void *replacement) {
	if (!callback_needs_wrapping(callback)) return callback;
#ifdef __x86_64__
	// create an asm gadget:
	// mov r11, callback-index
	// jump replacement
	// movabs $0x123456789abcdef0,%r11  =>  49 bb f0 de bc 9a 78 56 34 12
	// jmpq   *0x0(%rip)  =>  ff 25 00 00 00 00
	struct __attribute__((__packed__)) trampoline {
		unsigned char movabs[2] = {0x49, 0xbb};
		void *callback;
		unsigned char jmpq[6] = {0xff, 0x25, 0x00, 0x00, 0x00, 0x00};
		void *replacement;
	} packed;

	// allocate executable memory (if necessary)
	if (!trampolines || remainingTrampolinBytes < sizeof(trampoline)) {
		remainingTrampolinBytes = getpagesize();
		trampolines = real(mmap)(0, (unsigned) remainingTrampolinBytes, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (trampolines == MAP_FAILED) perror("trampoline mmap()");
	}

	// write trampoline
	struct trampoline *t = (struct trampoline *) trampolines;
	*t = trampoline();
	t->callback = callback;
	t->replacement = replacement;

	// update refs
	trampolines = &t[1];
	remainingTrampolinBytes -= sizeof(trampoline);

	return t;
#else
#error "Architecture is not supported!"
#endif
}


void CallbackManager::execute(void *callback, void *receiving_data, void *sending_data) {
	auto entry = callback_functions[(uintptr_t) callback];
	entry.wrapper(entry.callback, receiving_data, sending_data);
}

CallbackManager::CallbackManager() {
	callback_functions.emplace_back(nullptr, nullptr);
}


