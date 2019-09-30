#include <cstdio>
#include <dlfcn.h>
#include <unistd.h>
#include "hook.hpp"


real_function(mmap, void* (void *addr, size_t length, int prot, int flags, int fd, off_t offset));
real_function(mmap64, void* (void *addr, size_t length, int prot, int flags, int fd, off64_t offset));

extern "C" {
	void* hook_malloc (size_t size);
	void* malloc (size_t size);
	void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
	void *mmap64(void *addr, size_t len, int prot, int flags, int fildes, off64_t off);
}


void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset){
	void* result = real(mmap)(addr, length, prot, flags, fd, offset);
	fprintf(stderr, "[CALL] mmap(%p, %zu, %d, %d, %d, %zd) => %p\n", addr, length, prot, flags, fd, offset, result);
	return result;
}

void *mmap64(void *addr, size_t length, int prot, int flags, int fd, off64_t offset){
	void* result = real(mmap64)(addr, length, prot, flags, fd, offset);
	fprintf(stderr, "[CALL] mmap64(%p, %zu, %d, %d, %d, %zd) => %p\n", addr, length, prot, flags, fd, offset, result);
	return result;
}

/*
char malloc_buffer[0x11c00]; // libstdc++ wants this one

real_function(malloc, void* (size_t));
void* malloc (size_t size){
	void* result = real_malloc.ptr ? real(malloc)(size) : malloc_buffer;
	fprintf(stderr, "[CALL] malloc(%zu) => %p\n", size, result);
	return result;
}
*/



