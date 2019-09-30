#include <cstdio>
#include <iostream>
#include <csignal>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "nsjail/fork.h"
#include "nsjail/common.h"
#include "shared_memory_manger.h"
#include "ipc_constants.h"


//#include "ipcmodule.h"
struct __ipc_params;
struct __ipc_result;

extern "C" {
	void progress_ipc_call();
	void trigger_ipc_call();
}




key_t sema_blockouter;
key_t sema_blockinner;
static struct sembuf sema_inc = {0, 1, 0};
static struct sembuf sema_dec = {0, -1, 0};


void semaphore_init(){
	sema_blockouter = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_RMID | 0600);
	if (sema_blockouter < 0) perror("semget() 1");
	sema_blockinner = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_RMID | 0600);
	if (sema_blockinner < 0) perror("semget() 2");

	/* union semun {
		int              val;    // Value for SETVAL
		struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET
		unsigned short  *array;  // Array for GETALL, SETALL
		struct seminfo  *__buf;  // Buffer for IPC_INFO (Linux-specific)
	} setzero;
	setzero.val = 0;
	semctl(sema_blockinner, 0, SETVAL, setzero);
	semctl(sema_blockouter, 0, SETVAL, setzero); */

	printf("Initialized\n");
	/*int v1 = semctl(sema_blockinner, 0, GETVAL, setzero);
	if (v1 < 0) perror("semctl getval 1");
	int v2 = semctl(sema_blockouter, 0, GETVAL, setzero);
	if (v2 < 0) perror("semctl getval 2");
	printf("%d %d\n", v1, v2);*/
}

static int ipc_count = 0;

void semaphore_free(){
	semctl(sema_blockouter, 0, IPC_RMID);
	semctl(sema_blockinner, 0, IPC_RMID);
	printf("IPC calls: %d\n", ipc_count);
}


void trigger_ipc_call() {
	ipc_count++;
	// printf("Trigger\n");
	semop(sema_blockinner, &sema_inc, 1);
	// TODO callback loop
	semop(sema_blockouter, &sema_dec, 1);
	// printf("Done\n");
}


static void library_eventloop(){
	printf("Eventloop start...\n");
	while (true) {
		if (semop(sema_blockinner, &sema_dec, 1) < 0){
			if (errno == EIDRM) break;
			perror("semop() wait in thread");
		}

		progress_ipc_call();

		if (semop(sema_blockouter, &sema_inc, 1) < 0){
			if (errno == EIDRM) break;
			perror("semop() wait for result in thread");
		}
	}
	printf("Eventloop stopped.\n");
}


static void add_parent_dependency(){
	if (prctl(PR_SET_PDEATHSIG, SIGTERM) == -1) perror("prctl() - no parent dependency set");
}


static void child_process(){
	add_parent_dependency();
	register_shared_memory_as_default();
	library_eventloop();
}


static struct nsjconf_t nsjail_config;

void fork_event_loop(){
	/*
	pid_t pid = fork();
	if (pid < 0) perror("fork()");
	else if (pid == 0){
		child_process();
		exit(0);
	}
	return;
	// */
	nsjail_default_config(&nsjail_config);
	// keep shm open
	nsjail_config_add_open_fd(&nsjail_config, *shared_memory_fd);
	// max file size -> max shm (1GB)
	nsjail_config.rl_fsize = 1024*1024*1024;
	nsjail_fork(&nsjail_config, &child_process);
}



// SHARED MEMORY

__ipc_params *ipc_function_call_params = NULL;
__ipc_result *ipc_function_call_result = NULL;

//void* shared_memory;

void shared_memory_ipccomm_init() {
	/*size_t ps = getpagesize();
	void *shared = real(mmap)(NULL, ps, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //  | MAP_HUGETLB
	if (shared == (void *) -1) perror("mmap() shared structs");
	ipc_function_call_params = (__ipc_params *) shared;
	ipc_function_call_result = (__ipc_result *) ((char *) shared + ps/2);*/
	size_t partsize = (getpagesize() / 2) - 32;
	ipc_function_call_params = (__ipc_params *) shm_malloc(partsize);
	ipc_function_call_result = (__ipc_result *) shm_malloc(partsize);
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", partsize, ipc_function_call_params, pthread_self());
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", partsize, ipc_function_call_result, pthread_self());
}











