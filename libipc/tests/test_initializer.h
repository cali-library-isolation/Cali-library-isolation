#ifndef LIBIPC_TEST_INCLUDE_H
#define LIBIPC_TEST_INCLUDE_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "../shm_malloc.h"
#include "../shared_memory_manger.h"

extern "C"{
extern size_t _dl_pagesize;
}

void shared_memory_init();
void shared_memory_ipccomm_init(){
	size_t partsize = (size_t) (getpagesize() / 2) - 32;
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", partsize, shm_malloc(partsize), pthread_self());
	shm_debug("shm_malloc(%zu); // %p from thread %ld\n", partsize, shm_malloc(partsize), pthread_self());
}

void init(){
	_dl_pagesize = (size_t) getpagesize();
	shm_register_fork_handler();
	shared_memory_init();
	shared_memory_ipccomm_init();
}

#endif //LIBIPC_TEST_INCLUDE_H
