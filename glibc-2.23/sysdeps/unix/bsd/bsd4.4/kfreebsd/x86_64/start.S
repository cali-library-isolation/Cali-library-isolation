/* Startup code for FreeBSD/amd64 ABI.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Jaeger <aj@suse.de>, 2001.
   FreeBSD modification by Petr Salinger, 2006.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file with other
   programs, and to distribute those programs without any restriction
   coming from the use of this file. (The GNU Lesser General Public
   License restrictions do apply in other respects; for example, they
   cover modification of the file, and distribution when not linked
   into another program.)

   Note that people who make modified versions of this file are not
   obligated to grant this special exception for their modified
   versions; it is their choice whether to do so. The GNU Lesser
   General Public License gives permission to release a modified
   version without this exception; this exception also makes it
   possible to release a modified version which carries forward this
   exception.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* This is the canonical entry point, usually the first thing in the text
   segment.  The SVR4/i386 ABI (pages 3-31, 3-32) says that when the entry
   point runs, most registers' values are unspecified, except for a few.
   Blindly applied on amd64:

   %rdx		Contains a function pointer to be registered with `atexit'.
		This is how the dynamic linker arranges to have DT_FINI
		functions called for shared libraries that have been loaded
		before this code runs.

   %rsp		The stack contains the arguments and environment:
		0(%rsp)			argc
		8(%rsp)			argv[0]
		...
		(8*argc)(%rsp)		NULL
		(8*(argc+1))(%rsp)	envp[0]
		...
					NULL

   But on amd64 %rsp also have to be 16-byte aligned,
   standard C calling convention already passes arguments in registers.

   FreeBSD uses %edi as pointer to arguments and environment, %rsp is passed aligned.
   On entry from kernel, %rsp=%rdi or %rsp=%rdi-8,
   on entry from ld.so, glibc might set up it slightly differently.

   On FreeBSD, we use %rsi for passing function pointer to rtld_fini().
   On entry from FreeBSD kernel, %rsi is cleared, %rdx is not cleared,
   on entry from ld.so, glibc sets both %rsi and %rdx to point to rtld_fini().

   Used interface (via %rdi, %rsi) is equal to standard C calling interface for

   void _start(void *arg, void *rtld_fini());

*/

#include <sysdep.h>
#include <libc-symbols.h>

	weak_extern (_end)

	.text
	.globl _start
	.type _start,@function
_start:
	cfi_startproc
	cfi_undefined (rip)
	/* Clear the frame pointer.  The ABI suggests this be done, to mark
	   the outermost frame obviously.  */
	xorl %ebp, %ebp		/* zero extending clears whole rbp */

	/* Extract the arguments as encoded on the stack and set up
	   the arguments for __libc_start_main (int (*main) (int, char **, char **),
		   int argc, char *argv,
		   void (*init) (void), void (*fini) (void),
		   void (*rtld_fini) (void), void *stack_end).
	   The arguments are passed via registers and on the stack:
	main:		%rdi
	argc:		%rsi
	argv:		%rdx
	init:		%rcx
	fini:		%r8
	rtld_fini:	%r9
	stack_end:	stack.	*/

	movq %rsi, %r9		/* Address of the shared library termination
				   function.  */
	movq 0(%rdi), %rsi	/* argument count.  */
	leaq 8(%rdi), %rdx      /* argv starts just at above argc.  */

	/* Align the stack to a 16 byte boundary to follow the ABI.  */
	andq  $~15, %rsp

	pushq %rax		/* Push garbage because we push 8 more bytes.  */

	/* Provide the highest stack address to the user code (for stacks
	   which grow downwards).  */
	pushq %rsp

#ifdef SHARED
	/* Pass address of our own entry points to .fini and .init.  */
	movq __libc_csu_fini@GOTPCREL(%rip), %r8
	movq __libc_csu_init@GOTPCREL(%rip), %rcx

	movq main@GOTPCREL(%rip), %rdi

	/* Call the user's main function, and exit with its value.
	   But let the libc call main.	  */
	call __libc_start_main@PLT
#else
	/* Pass address of our own entry points to .fini and .init.  */
	movq $__libc_csu_fini, %r8
	movq $__libc_csu_init, %rcx

	movq $main, %rdi

	/* Call the user's main function, and exit with its value.
	   But let the libc call main.	  */
	call __libc_start_main
#endif

	hlt			/* Crash if somehow `exit' does return.	 */
	/* just reference _end, 
	   it is needed as global symbol for brk() usage,
	   it is a dead code on purpose.  */
#ifdef SHARED
	movq _end@GOTPCREL(%rip), %rax
#else
	movq $_end, %rax
#endif
	cfi_endproc
/* Define a symbol for the first piece of initialized data.  */
	.data
	.globl __data_start
__data_start:
	.long 0
	.weak data_start
	data_start = __data_start
