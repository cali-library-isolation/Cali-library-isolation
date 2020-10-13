#ifndef CALI_SECCOMP_CONSTANTS_H
#define CALI_SECCOMP_CONSTANTS_H



/* Valid values for seccomp.mode and prctl(PR_SET_SECCOMP, <mode>) */
#define SECCOMP_MODE_DISABLED    0 /* seccomp is not in use. */
#define SECCOMP_MODE_STRICT      1 /* uses hard-coded filter. */
#define SECCOMP_MODE_FILTER      2 /* uses user-supplied filter. */

/* Valid operations for seccomp syscall. */
#define SECCOMP_SET_MODE_STRICT        0
#define SECCOMP_SET_MODE_FILTER        1
#define SECCOMP_GET_ACTION_AVAIL       2

/* Valid flags for SECCOMP_SET_MODE_FILTER */
#define SECCOMP_FILTER_FLAG_TSYNC         1
#define SECCOMP_FILTER_FLAG_LOG           2
#define SECCOMP_FILTER_FLAG_SPEC_ALLOW    4

/*
 * All BPF programs must return a 32-bit value.
 * The bottom 16-bits are for optional return data.
 * The upper 16-bits are ordered from least permissive values to most,
 * as a signed value (so 0x8000000 is negative).
 *
 * The ordering ensures that a min_t() over composed return values always
 * selects the least permissive choice.
 */
#define SECCOMP_RET_KILL_PROCESS 0x80000000 /* kill the process */
#define SECCOMP_RET_KILL_THREAD     0x00000000 /* kill the thread */
#define SECCOMP_RET_KILL     SECCOMP_RET_KILL_THREAD
#define SECCOMP_RET_TRAP     0x00030000 /* disallow and force a SIGSYS */
#define SECCOMP_RET_ERRNO     0x00050000 /* returns an errno */
#define SECCOMP_RET_TRACE     0x7ff00000 /* pass to a tracer or disallow */
#define SECCOMP_RET_LOG         0x7ffc0000 /* allow after logging */
#define SECCOMP_RET_ALLOW     0x7fff0000 /* allow */

/* Masks for the return value sections. */
#define SECCOMP_RET_ACTION_FULL    0xffff0000
#define SECCOMP_RET_ACTION    0x7fff0000
#define SECCOMP_RET_DATA    0x0000ffff



#define AUDIT_ARCH_X86_64 0xc000003e




#define PROT_READ	0x1		/* Page can be read.  */
#define PROT_WRITE	0x2		/* Page can be written.  */
#define PROT_EXEC	0x4		/* Page can be executed.  */



#endif //CALI_SECCOMP_CONSTANTS_H
