#include "seccomp.h"
#include <cstdio>
#include <sys/prctl.h>
#include <cstdlib>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <unistd.h>
#include "shared_memory_manger.h"


#ifdef SEQUENTIAL_MODE
#ifdef MPROTECT_MODE
#error "sequential and mprotect mode must not be combined!"
#endif
#endif


extern "C" {
// Section "text_mprotect" is allowed to issue mprotect calls on shared memory
extern char __start_text_mprotect;
extern char __stop_text_mprotect;
// Section "text" contains our code
extern char __start_text asm("__executable_start");
extern char __stop_text asm("_etext");
}


namespace {

#define lower(x) ((__u32) (__u64) (x))
#define higher(x) ((__u32) (((__u64) (x)) >> 32u))
#define bpf_ident_0001 lower(&__start_text_mprotect)
#define bpf_ident_0002 higher(&__start_text_mprotect)
#define bpf_ident_0003 lower(&__stop_text_mprotect)
#define bpf_ident_0004 higher(&__stop_text_mprotect)
#define bpf_ident_0011 lower(&__start_text)
#define bpf_ident_0012 higher(&__start_text)
#define bpf_ident_0013 lower(&__stop_text)
#define bpf_ident_0014 higher(&__stop_text)
#define bpf_ident_0020 ((__u32) SHARED_MEMORY_FD)


	// static_assert(bpf_ident_0002 == bpf_ident_0004, "text_mprotect invalid location");

	struct sock_filter basic_filter[] = {
#ifdef SEQUENTIAL_MODE
#include "bpf/syscall_filter_sequential.h"
#else
#include "bpf/syscall_filter_mprotect.h"
#endif
	};

}


SeccompFilterBuilder::SeccompFilterBuilder() {
	filter = basic_filter;
	filter_count = sizeof(basic_filter) / sizeof(basic_filter[0]);

	// fprintf(stderr, "text_mprotect start %p\n", &__start_text_mprotect);
	// fprintf(stderr, "text_mprotect stop  %p\n", &__stop_text_mprotect);
	// fprintf(stderr, "text start %p\n", &__start_text);
	// fprintf(stderr, "text stop  %p\n", &__stop_text);
	// fprintf(stderr, "%d / 4096 seccomp instructions\n", filter_count);

#ifdef MPROTECT_MODE
	if (higher(&__start_text_mprotect) != higher(&__stop_text_mprotect)) {
		fprintf(stderr, "Invalid text_mprotect segment, crossing 32bit boundary\n0x%x 0x%x\n", higher(&__start_text_mprotect), higher(&__stop_text_mprotect));
		_exit(1);
	}
#endif
	// sleep(3);
}

void SeccompFilterBuilder::install_filter() {
	if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("prctl(NO_NEW_PRIVS)");
		exit(1);
	}

	struct sock_fprog prog = {
			.len = filter_count,
			.filter = filter,
	};

	if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
		perror("prctl(SECCOMP)");
		exit(1);
	}
	// fprintf(stderr, "seccomp has been installed.\n");
}
