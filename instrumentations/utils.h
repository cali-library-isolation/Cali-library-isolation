#ifndef CALI_UTILS_H
#define CALI_UTILS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include "read_mprotect.h"

extern char *program_invocation_name;
extern char *program_invocation_short_name;

// #define debug_printf(s, ...) fprintf(stderr, "[%d] " s "\n", getpid(), ##__VA_ARGS__)
#define debug_printf(s, ...) fprintf(stderr, "[%d %s] " s "\n", getpid(), program_invocation_short_name, ##__VA_ARGS__)

void __attribute__((noinline)) gdb_breakpoint() {
	//fprintf(stderr, "Halted process with cmdline ");
	//system(asprintf("cat /proc/%d/cmdline", getpid()));
	/*
	fprintf(stderr, "Use gdb: attach %d\n", getpid());
	while (1) {
		sleep(120);
	} // */
}

inline int is_clearly_shared(const void* p) {
	return (0xffff00000000u & (uintptr_t) p) == 0x3f0000000000 || (0xffff00000000u & (uintptr_t) p) == 0x3e0000000000;
}

inline void assert_clearly_shared(const void* p) {
	if (p && !is_clearly_shared(p)) {
		debug_printf("WARNING! NOT CLEARLY SHARED (%p)!", p);
		gdb_breakpoint();
	}
}

int is_likely_shared(const void* p) {
	// shared memory in 0x3f or 0x3e
	if (p == 0 || is_clearly_shared(p)) return 1;
	// argv is set before fork
	uintptr_t u = (uintptr_t) p;
	uintptr_t argv0 = (uintptr_t) program_invocation_name;
	if ((u & 0xfffffffff000u) == (argv0 & 0xfffffffff000u) && u >= argv0)
		return 1;
	// read-only memory
	unsigned int prot = read_mprotection(p);
	if (prot == -1) {
		debug_printf("Pointer not mapped: %p", p);
		char buffer[64];
		sprintf(buffer, "pmap %d >&2", getpid());
		if (system(buffer)) {}
	}
	//if (0x100000000 < (unsigned long) p)
	//	gdb_breakpoint();
	if ((prot & PROT_SHARED) == PROT_SHARED || (prot & PROT_WRITE) == 0)
		return 1;
	return 0;
}

static unsigned do_check_ptr(const void* p, const char* function, const char* what) {
	if (p == 0 || is_likely_shared(p)) return 0;
	debug_printf("[SHARING ERROR] %s = %p in %s", what, p, function);
	char buffer[256];
	sprintf(buffer, "pmap %d | grep %lx >&2", getpid(), ((uintptr_t) p) >> 12u);
	debug_printf(" > %s", buffer);
	if (system(buffer)) {
		/*
		sprintf(buffer, "pmap %d >&2", getpid());
		debug_printf(" > %s", buffer);
		if (system(buffer)) {} // */
	}
	gdb_breakpoint();
	return 1;
}

#define check_ptr(p) (do_check_ptr(p, __func__, #p))

#endif //CALI_UTILS_H
