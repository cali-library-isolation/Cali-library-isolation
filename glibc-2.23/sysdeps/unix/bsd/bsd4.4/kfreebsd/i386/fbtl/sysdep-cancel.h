/* Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2002.
   Modification for FreeBSD by Petr Salinger, 2005.

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

#include <sysdep.h>
#include <tls.h>
#ifndef __ASSEMBLER__
# include <fbtl/pthreadP.h>
#endif

#if IS_IN (libc) || IS_IN (libpthread) || IS_IN (librt)

# undef PSEUDO
# define PSEUDO(name, syscall_name, args)				      \
  .text;								      \
  ENTRY (name)								      \
    cmpl $0, %gs:MULTIPLE_THREADS_OFFSET;				      \
    jne L(pseudo_cancel);						      \
  .type __##syscall_name##_nocancel,@function;				      \
  .globl __##syscall_name##_nocancel;					      \
  __##syscall_name##_nocancel:						      \
    DO_CALL (syscall_name, args);					      \
    jb SYSCALL_ERROR_LABEL;						      \
    ret;								      \
  .size __##syscall_name##_nocancel,.-__##syscall_name##_nocancel;	      \
  L(pseudo_cancel):							      \
    CENABLE								      \
    movl %eax, %ecx;							      \
    movl $SYS_ify (syscall_name), %eax;					      \
    int $0x80;								      \
    PUSHRESULT;							      	      \
    movl %ecx, %eax; 							      \
    CDISABLE;							 	      \
    POPRESULT;							      	      \
    jb SYSCALL_ERROR_LABEL;						      \
  L(pseudo_end):

/*
  on FreeBSD some syscalls return result in pair edx+eax,
  therefore proper way would be

# define PUSHRESULT	pushl %edx; pushl %eax; pushfl
# define POPRESULT	popfl; popl %eax; popl %edx

  for FreeBSD 5.4 affected syscalls are

	lseek()
	fork()
	vfork()
	rfork()
	pipe()

   none of them is cancelable, therefore
*/

# define PUSHRESULT	pushl %eax; cfi_adjust_cfa_offset (4);  pushfl;    cfi_adjust_cfa_offset (4)
# define POPRESULT	popfl;      cfi_adjust_cfa_offset (-4); popl %eax; cfi_adjust_cfa_offset (-4)

# if IS_IN (libpthread)
#  define CENABLE	call __pthread_enable_asynccancel;
#  define CDISABLE	call __pthread_disable_asynccancel
# elif IS_IN (libc)
#  define CENABLE	call __libc_enable_asynccancel;
#  define CDISABLE	call __libc_disable_asynccancel
# elif IS_IN (librt)
#  define CENABLE	call __librt_enable_asynccancel;
#  define CDISABLE	call __librt_disable_asynccancel
# else
#  error Unsupported library
# endif


# ifndef __ASSEMBLER__
#  define SINGLE_THREAD_P \
  __builtin_expect (THREAD_GETMEM (THREAD_SELF, \
				   header.multiple_threads) == 0, 1)
# else
#  define SINGLE_THREAD_P cmpl $0, %gs:MULTIPLE_THREADS_OFFSET
# endif

#elif !defined __ASSEMBLER__

# define SINGLE_THREAD_P (1)
# define NO_CANCELLATION 1

#endif

#ifndef __ASSEMBLER__
# define RTLD_SINGLE_THREAD_P \
  __builtin_expect (THREAD_GETMEM (THREAD_SELF, \
				   header.multiple_threads) == 0, 1)
#endif
