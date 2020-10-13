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
#include "signals.h"
#include "program_params.h"
#include "shared_memory_locking.h"
#include "shared_lock.h"

#ifdef MPROTECT_MODE
#include "seccomp.h"
#endif
#ifdef SEQUENTIAL_MODE
#include "seccomp.h"
#endif


#define CALLBACK_MAX_RESERVED 5

//static struct sembuf sema_inc = {0, 1, 0};
//static struct sembuf sema_dec = {0, -1, 0};

#define switch(x, y) do {typeof(x) tmp = x; (x) = y; y = tmp; } while(false)


void __attribute__((weak)) report_coverage() {
	fprintf(stderr, "No coverage given\n");
}

void report_coverage_lite(int);


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
	shared_memory_init(&fileDescriptors);
	remap_library_segments();

	// Initialize shared memory
	sending_data = shm_malloc_2(sending_size);
	receiving_data = shm_malloc_2(receiving_size);
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", sending_size, sending_data, pthread_self());
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", receiving_size, receiving_data, pthread_self());

	sending_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
	if (sending_semaphore < 0) perror("semget() 1");
	receiving_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
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
	real(signal)(SIGUSR2, +[](int) {
		last_instance->printStats();
	});
	if (program_params.concurrentLibraryCommunication) sending_lock = new SharedMutex(true);
}


IpcCommunication::~IpcCommunication() {
	if (program_params.is_forking ? statuslock_is_sole_survivor() : !is_forked) {
		terminate();
		debug_printf("IPC calls: %d", ipc_count);
		// fprintf(stderr, "IPC calls: %d\n", ipc_count);
		// printStats();
	}
	report_coverage_lite(ipc_count);
}


void IpcCommunication::switch_sender_receiver() {
	switch(sending_semaphore, receiving_semaphore);
	switch(sending_data, receiving_data);
	switch(processor, library_processor);
	fileDescriptors.setLibrary();
	is_library = true;
}


typedef struct communication_t {
	int code;
	int libc_errno;
	void *callback;
} communication_t;


void IpcCommunication::trigger_ipc_call() {
	shm_msync();
	ipc_count++;
	((communication_t *) sending_data)->libc_errno = errno;
#ifdef MPROTECT_MODE
	// TODO make memory RO
	shared_memory_set_ro();
#endif
	// fprintf(stderr, "[%d,%d] Trigger with sending code %d (callback=%p)\n", getpid(), is_library, ((communication_t *) sending_data)->code, ((communication_t *) sending_data)->callback);
	// fflush(stderr);
	if (sending_is_blocked) {
		fprintf(stderr, "[WARN] trigger_ipc_call despite sending being blocked, code %d!\n", ((communication_t *) sending_data)->code);
		__warning_breakpoint();
	}
	semop(sending_semaphore, &sema_inc, 1);
	sending_is_blocked = true;
	// printf("Done\n");
}


void *IpcCommunication::wait_for_return() {
	while (true) {
		while (semop(receiving_semaphore, &sema_dec, 1) < 0) {
			unregister_shared_memory_as_default();
			if (errno == EIDRM) exit(0);
			if (errno == EINVAL) exit(1);
			perror("semop() wait in thread");
			reregister_shared_memory_as_default();
		}
		if (!sending_is_blocked) {
			fprintf(stderr, "[WARN] after receive: sending was not properly blocked!\n");
			__warning_breakpoint();
		}
		sending_is_blocked = false;
		remap_shared_memory();

#ifdef MPROTECT_MODE
		//TODO make memory RW
		shared_memory_set_rw();
#endif

		int code = ((communication_t *) receiving_data)->code;
		errno = ((communication_t *) receiving_data)->libc_errno;
		if (sending_lock && (code <= 2 || code > 8)) {  // "internal" functions must not be interrupted
			// fprintf(stderr, "[%d] unlock\n", getpid());
			sending_lock->unlock();
		}
		//fprintf(stderr, "[%d] Receiving %d\n", is_library, code);
		//fflush(stderr);
		if (code == 1) {
			break;
		} else if (code == -1) {
			fprintf(stderr, "Remote exit requested.\n");
			terminate();
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

		//fprintf(stderr, "[%d] Sending response %d\n", is_library, ((communication_t*)sending_data)->code);
		//fflush(stderr);
		((communication_t *) sending_data)->libc_errno = errno;
		if (sending_is_blocked) {
			fprintf(stderr, "[WARN] sending response despite sending being blocked!\n");
			__warning_breakpoint();
		}
		sending_is_blocked = true;

#ifdef MPROTECT_MODE
		// TODO make memory RO
		shared_memory_set_ro();
#endif
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
	if (prctl(PR_SET_PDEATHSIG, SIGTERM)) perror("prctl() - no parent dependency set");
}


void IpcCommunication::child_process_main() {
	if (getenv("CALI_ATTACH_GDB")) {
		sleep(2);
	}
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
	shared_memory_forking_complete();
	sending_is_blocked = true;
	//TODO
	// system("echo -n \"Welcome from IPC jail: \" && id && cat /proc/1/cgroup");
#ifdef MPROTECT_MODE
	// make memory RO
	shared_memory_set_ro();
	// install seccomp filter
	SeccompFilterBuilder builder;
	builder.install_filter();
#elif SEQUENTIAL_MODE
	SeccompFilterBuilder builder;
	builder.install_filter();
#endif
	try {
		wait_for_return();
	} catch (...) {
		((communication_t *) sending_data)->code = -1;
		semop(sending_semaphore, &sema_inc, 1);
		usleep(50000);
		terminate();
		_exit(1);
	}
	terminate();
}

void IpcCommunication::printStats() {
	fprintf(stderr, "IPC calls: %d\n", ipc_count);
	report_coverage();
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
	if (program_params.silent & 0x2) stderr_to_pipe();
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
		auto cwd = getcwd(nullptr, 0);
		nsjail_config->cwd = cwd ? cwd : "";
		if (configurator)
			configurator(nsjail_config.get());
		nsjail_fork(nsjail_config.get(), &child_process, [](int x) {
			// fprintf(stderr, "[%d] (intermediate process got parent terminate %d\n", getpid(), x);
			if (IpcCommunication::last_instance && (!program_params.is_forking || statuslock_is_sole_survivor()))
				IpcCommunication::last_instance->terminate();
		});
		// fprintf(stderr, "Parent process: %d\n", getpid());
	} else {
		if (fork() == 0) {
			child_process();
			exit(0);
		}
	}
	shared_memory_forking_complete();
	if (program_params.is_forking) {
		statuslock_install();
		statuslock_install_forkhandler();
	} else {
		pthread_atfork(nullptr, nullptr, []() {
			IpcCommunication::last_instance->is_forked = true;
			shared_memory_unfork();
		});
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

void IpcCommunication::terminate() {
	/*
	fprintf(stderr, "[%d] terminate (%d,%d) ", getpid(), sending_semaphore, receiving_semaphore);
	if (semctl(sending_semaphore, 0, IPC_RMID)) perror("sending_semaphore");
	if (semctl(receiving_semaphore, 0, IPC_RMID)) perror("receiving_semaphore");
	fprintf(stderr, " terminated.\n");
	 */
	semctl(sending_semaphore, 0, IPC_RMID);
	semctl(receiving_semaphore, 0, IPC_RMID);
	shared_memory_terminate();
	if (sending_lock) sending_lock->terminate();
}

void IpcCommunication::sending_lock_lock() const {
	if (sending_lock) {
		// fprintf(stderr, "[%d] lock ...", getpid());
		sending_lock->lock();
		// fprintf(stderr, " taken by %d\n", getpid());
	}
}


static inline bool callback_needs_wrapping(void *callback) {
	return callback != nullptr && callback != (void *) -1 && (uintptr_t) callback > CALLBACK_MAX_RESERVED;
}


void *CallbackManager::wrap_outgoing_callback(void *callback, void (*wrapper)(void *, void *, void *)) {
	if (!callback_needs_wrapping(callback)) return callback;
	auto number = callback_numbers.find(callback);
	if (number != callback_numbers.end()) return (void *) number->second;
	callback_functions.emplace_back(callback, wrapper);
	// fprintf(stderr, "[%d] wrapping outgoing callback %p to %p\n", getpid(), callback, (void *) (callback_functions.size() - 1));
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

	// fprintf(stderr, "[%d] wrapping incoming callback %p to %p\n", getpid(), callback, t);
	return t;
#else
#error "Architecture is not supported!"
#endif
}


void CallbackManager::execute(void *callback, void *receiving_data, void *sending_data) {
	// auto entry = callback_functions[(uintptr_t) callback];
	if ((uintptr_t) callback >= callback_functions.size()) {
		fprintf(stderr, "INVALID CALLBACK: %p >= %ld (max)\n", callback, callback_functions.size());
	}
	auto entry = callback_functions.at((uintptr_t) callback);
	entry.wrapper(entry.callback, receiving_data, sending_data);
}

CallbackManager::CallbackManager() {
	callback_functions.emplace_back(nullptr, nullptr);
	for (uintptr_t i = 1; i <= CALLBACK_MAX_RESERVED; i++) {
		callback_functions.emplace_back(i);
	}
}
