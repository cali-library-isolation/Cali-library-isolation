/* Copyright (C) 2001,02,03,04 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Modification for FreeBSD contributed by Petr Salinger, 2006.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _KFREEBSD_X86_64_SYSDEP_H
#define _KFREEBSD_X86_64_SYSDEP_H 1

/* There is some commonality.  */
#include <sysdeps/unix/x86_64/sysdep.h>
/* Defines RTLD_PRIVATE_ERRNO and USE_DL_SYSINFO.  */
#include <dl-sysdep.h>
#include <tls.h>

#ifdef __ASSEMBLER__

/* We don't want the label for the error handle to be global when we define
   it here.  */
#ifdef PIC
# define SYSCALL_ERROR_LABEL 0f
#else
# define SYSCALL_ERROR_LABEL syscall_error
#endif

#undef	PSEUDO
#define	PSEUDO(name, syscall_name, args)				      \
  .text;								      \
  ENTRY (name)								      \
    DO_CALL (syscall_name, args);					      \
    jb SYSCALL_ERROR_LABEL;						      \
  L(pseudo_end):

#undef	PSEUDO_END
#define	PSEUDO_END(name)						      \
  SYSCALL_ERROR_HANDLER							      \
  END (name)

#undef	PSEUDO_NOERRNO
#define	PSEUDO_NOERRNO(name, syscall_name, args) \
  .text;								      \
  ENTRY (name)								      \
    DO_CALL (syscall_name, args)

#undef	PSEUDO_END_NOERRNO
#define	PSEUDO_END_NOERRNO(name) \
  END (name)

#define ret_NOERRNO ret

#undef	PSEUDO_ERRVAL
#define	PSEUDO_ERRVAL(name, syscall_name, args) \
  .text;								      \
  ENTRY (name)								      \
    DO_CALL (syscall_name, args);					      \

#undef	PSEUDO_END_ERRVAL
#define	PSEUDO_END_ERRVAL(name) \
  END (name)

#define ret_ERRVAL ret

#ifndef PIC
#define SYSCALL_ERROR_HANDLER	/* Nothing here; code in sysdep.S is used.  */
#elif RTLD_PRIVATE_ERRNO
# define SYSCALL_ERROR_HANDLER			\
0:						\
  leaq rtld_errno(%rip), %rcx;			\
  movl %eax, (%rcx);				\
  orq $-1, %rax;				\
  jmp L(pseudo_end);
#else
# if IS_IN (libc)
#  define SYSCALL_ERROR_ERRNO __libc_errno
# else
#  define SYSCALL_ERROR_ERRNO errno
# endif
# define SYSCALL_ERROR_HANDLER			\
0:						\
  movq SYSCALL_ERROR_ERRNO@GOTTPOFF(%rip), %rcx;\
  movl %eax, %fs:(%rcx);			\
  orq $-1, %rax;				\
  jmp L(pseudo_end);
#endif	/* PIC */

/* The FreeBSD/amd64 kernel expects the system call parameters in
   registers according to the following table:

    syscall number	rax
    arg 1		rdi
    arg 2		rsi
    arg 3		rdx
    arg 4		r10
    arg 5		r8
    arg 6		r9

    The FreeBSD kernel internal syscall usage of registers:

    preserved:
	stack pointer			rsp
	C preserved			rbx, rbp, r12-r15
	some input params		rdi, rsi

    destroyed:
	return value(s)			rax, rdx, eflags
	return address from syscall	rcx
	eflags from syscall		r11
	used internally			r8-r10

    Normal function call, including calls to the system call stub
    functions in the libc, get the first six parameters passed in
    registers and the seventh parameter and later on the stack.  The
    register use is as follows:

     system call number	in the DO_CALL macro
     arg 1		rdi
     arg 2		rsi
     arg 3		rdx
     arg 4		rcx
     arg 5		r8
     arg 6		r9

    We have to take care that the stack is aligned to 16 bytes.  When
    called the stack is not aligned since the return address has just
    been pushed.

*/

#undef	DO_CALL
#define DO_CALL(syscall_name, args)		\
    DOARGS_##args				\
    movl $SYS_ify (syscall_name), %eax;		\
    syscall;

#define DOARGS_0 /* nothing */
#define DOARGS_1 /* nothing */
#define DOARGS_2 /* nothing */
#define DOARGS_3 /* nothing */
#define DOARGS_4 movq %rcx, %r10;
#define DOARGS_5 DOARGS_4
#define DOARGS_6 DOARGS_5
#define DOARGS_7 DOARGS_6
#define DOARGS_8 DOARGS_7

#else   /* !__ASSEMBLER__ */

#include <syscalls-inline.h>

#endif	/* __ASSEMBLER__ */


/* Pointer mangling support.  */
#if IS_IN (rtld)
/* We cannot use the thread descriptor because in ld.so we use setjmp
   earlier than the descriptor is initialized.  */
# ifdef __ASSEMBLER__
#  define PTR_MANGLE(reg)	xorq __pointer_chk_guard_local(%rip), reg;    \
				rolq $17, reg
#  define PTR_DEMANGLE(reg)	rorq $17, reg;				      \
				xorq __pointer_chk_guard_local(%rip), reg
# else
#  define PTR_MANGLE(reg)	asm ("xorq __pointer_chk_guard_local(%%rip), %0\n" \
				     "rolq $17, %0"			      \
				     : "=r" (reg) : "0" (reg))
#  define PTR_DEMANGLE(reg)	asm ("rorq $17, %0\n"			      \
				     "xorq __pointer_chk_guard_local(%%rip), %0" \
				     : "=r" (reg) : "0" (reg))
# endif
#else
# ifdef __ASSEMBLER__
#  define PTR_MANGLE(reg)	xorq %fs:POINTER_GUARD, reg;		      \
				rolq $17, reg
#  define PTR_DEMANGLE(reg)	rorq $17, reg;				      \
				xorq %fs:POINTER_GUARD, reg
# else
#  define PTR_MANGLE(var)	asm ("xorq %%fs:%c2, %0\n"		      \
				     "rolq $17, %0"			      \
				     : "=r" (var)			      \
				     : "0" (var),			      \
				       "i" (offsetof (tcbhead_t,	      \
						      pointer_guard)))
#  define PTR_DEMANGLE(var)	asm ("rorq $17, %0\n"			      \
				     "xorq %%fs:%c2, %0"		      \
				     : "=r" (var)			      \
				     : "0" (var),			      \
				       "i" (offsetof (tcbhead_t,	      \
						      pointer_guard)))
# endif
#endif

#endif /* _KFREEBSD_X86_64_SYSDEP_H  */
