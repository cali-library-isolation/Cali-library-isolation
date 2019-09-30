#include <sys/mman.h>
#include <cstring>
#include <cstdio>
#include <dlfcn.h>
#include <cstdint>
#include <zconf.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <csignal>
#include "mappings.h"
#include <malloc.h>
#include "shared_memory_manger.h"
#include "hook.hpp"
#include "debug.h"

using namespace std;

static inline bool contains(string haystack, string needle) {
	return haystack.find(needle) != std::string::npos;
}

static long memory_checksum(const char *addr, int len) {
	long sum = 0;
	for (int i = 0; i < len; i++)
		sum = 31 * sum + addr[i];
	return sum;
}

static inline void my_memcpy(void *dest, const void *src, size_t size) {
	long *d = (long *) dest;
	const long *s = (const long *) src;
	const long *end = s + (size / sizeof(long));
	while (s != end) *(d++) = *(s++);
}


// static char __attribute__((section("shm_data"))) dummy;
extern "C" {
extern char __start_shm_data;
extern char __stop_shm_data;
extern char __last_shm_data_variable;
extern char __start_data asm ("__data_start");
}


void remap_shared(void *addr, size_t len, int permissions) {
	void *(*my_mmap)(void *addr, size_t length, int prot, int flags, int fd, off_t offset) = real(mmap);
	debug_printf("Sharing %p - %p (%ld bytes)\n", addr, (void *) (len + (char *) addr), len);
	// move to tmp storage
	void *tmp = my_mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	memcpy(tmp, addr, len);
	if (!munmap(addr, len)) {
		perror("[remap_shared] unmap");
	}
	// remap and move back
	if (my_mmap(addr, len, permissions, MAP_SHARED | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
		perror("[remap_shared] mmap");
	}
	memcpy(addr, tmp, len);
	munmap(tmp, len);
}


static inline uintptr_t page(void *ptr) {
	uintptr_t mask = ~(((uintptr_t) getpagesize()) - 1);
	return mask & (uintptr_t) ptr;
}

static inline uintptr_t upper_page(uintptr_t ptr) {
	uintptr_t page = (uintptr_t) getpagesize();
	uintptr_t mask = ~(page - 1); // 0xfffff000
	return ((ptr - 1) & mask) + page;
}


void remap_data_shared(void *addr, size_t len, int permissions) asm("remap_data_shared");
void remap_data_shared(void *addr, size_t len, int permissions) {
	void *(*my_mmap)(void *addr, size_t length, int prot, int flags, int fd, off_t offset) = real(mmap);
	// int (*my_system)(const char *) = system;

	// std::string cmd = "pmap " + std::to_string(getpid());
	// const char *cmd2 = cmd.c_str();
	// my_system(cmd2);

	// Resolve some PLT entries
	// fprintf(stderr, nullptr);
	my_mmap(nullptr, 0, 0, 0, -1, 0);
	real(mremap)(nullptr, 0, 0, 0);
	munmap(nullptr, 0);


	// printf("Checksum 1a: %lx\n", memory_checksum((char *) 0x62d000, 0x2000));
	// printf("Checksum 1b: %lx\n", memory_checksum((char *) 0x62f000, 0x1000));

	//TODO breaks with PIE executables
	// move to tmp storage
	// fprintf(stderr, "Sharing ELF segment %p - %p (%ld bytes)\n", addr, (void *) (len + (char *) addr), len);
	void *tmp = my_mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	my_memcpy(tmp, addr, len);

	uintptr_t start = page(&__start_data);
	size_t remaining_elfsize = ((uintptr_t) addr) - start;
	// printf("mremap(..., 0x%lx, 0x%lx)\n", remaining_elfsize + len, remaining_elfsize);
	void *r = real(mremap)((void *) start, remaining_elfsize + len, remaining_elfsize, MREMAP_MAYMOVE);
	if (r == MAP_FAILED) {
		perror("[remap_shared] mremap");
	}
	// printf("Checksum 4: %lx\n", memory_checksum((char *) 0x62d000, 0x2000));
	// printf("%p  %s %p %p\n", r, cmd2, &my_system, &system);

	// remap and move back
	if (my_mmap(addr, len, permissions, MAP_SHARED | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
		perror("[remap_shared] mmap");
	}
	my_memcpy(addr, tmp, len);
	munmap(tmp, len);

	// printf("Checksum 2a: %lx\n", memory_checksum((char *) 0x62d000, 0x2000));
	// printf("Checksum 2b: %lx\n", memory_checksum((char *) 0x62f000, 0x1000));
	// my_system(cmd2);
	// std::raise(SIGTRAP);
}


void find_mappings() {
	ifstream infile("/proc/self/maps");
	string line;
	while (getline(infile, line)) {
		istringstream iss(line);
		string pointers, permissions, offset, x, y, name;
		if (iss >> pointers >> permissions >> offset >> x >> y >> name) {
			if (permissions == "rw-p" && !name.empty() && name[0] != '['
				&& !contains(name, "libc-")) {
				uintptr_t start = 0, end = 0;
				char c;
				istringstream iss2(pointers);
				if (iss2 >> std::hex >> start >> c >> end) {
					if (start > 0x700000000000) {
						void *ptr = (void *) start;
						size_t len = end - start;
						remap_shared(ptr, len);
					}
				}
			}
		}
	}
}


static void assertNotShared(char* start, size_t size, void* ptr) {
	if (start <= ptr && ptr < start+size)
		throw runtime_error("Compiler error: Symbol must not be shared");
}


void remap_library_segments() {
	// Find sqlite
	/*uintptr_t ptr = page(dlsym(nullptr, "sqlite3_free"));
	ptr += 0x298000;
	printf("sqlite3 = %p\n", ptr);
	remap_shared((void *) ptr, 0x2000);*/
	// TODO find_mappings();

	debug_printf("shm-data ptr: %p - %p ( - %p)\n", &__start_shm_data, &__last_shm_data_variable, &__stop_shm_data);
	if (&__last_shm_data_variable < &__stop_shm_data-1)
		throw runtime_error("__last_shm_data_variable is not the last symbol in the shared memory section");

	if (&__start_shm_data != &__last_shm_data_variable && &__start_shm_data != &__stop_shm_data) {
		uintptr_t shm_data_page = page(&__start_shm_data);
		size_t size = upper_page(((uintptr_t) &__last_shm_data_variable) - shm_data_page);

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		assertNotShared((char*) shm_data_page, size, &shared_memory_fd);
		assertNotShared((char*) shm_data_page, size, (void*) &__malloc_hook);
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

		remap_data_shared((void *) shm_data_page, size, PROT_READ + PROT_WRITE);
		// print_mapping();
	} else {
		debug_printf("... no global data to be shared.\n");
	}
}


void *get_new_stack() {
	void *ptr = (void *) 0x370000000000;
	size_t len = 0x100000;
	ptr = real(mmap)(ptr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK | MAP_GROWSDOWN | MAP_FIXED, -1, 0);
	if (ptr == MAP_FAILED)
		perror("mmap stack");
	return reinterpret_cast<void *>((uintptr_t) ptr + len - 0x1000);
}

void unmap_stack(void *sp) {
	debug_printf("Unmapping %p...\n", sp);
	size_t len = 144 * 1024;
	if (munmap((void *) (page(sp) - len), len + 0x8000) != 0)
		perror("munmap(stack)");
}

void print_mapping(void) {
	// Print memory layout after fork
	std::string cmd = "pmap " + std::to_string(getpid());
	system(cmd.c_str());
}
