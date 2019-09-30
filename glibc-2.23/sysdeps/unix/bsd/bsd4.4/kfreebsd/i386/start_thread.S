/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.

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

/* __start_thread (flags, stack, func, arg)
   calls __rfork (flags), and in the child sets the stack pointer and then
   calls _exit (func (arg)).
   It cannot be done in portable C.  */

#include <sysdep.h>
#include <asm-syntax.h>
#define SIG_SETMASK	3

	.text
ENTRY (__start_thread)
	/* End FDE now, because in the child the unwind info will be
	   wrong.  */
	cfi_endproc

	/* There is a window of a few instructions, right after the rfork
	   system call, where the handling of a signal would write garbage
	   into the stack shared by the parent and the child (assuming
	   RFMEM is set in flags).  To solve this: 1. We block all signals
	   around the rfork system call and unblock them afterwards in
	   the parent and in the child (but only after changing the stack
	   pointer).  2. The child accesses only values passed in registers
	   and on its own stack.  This way, if the parent is scheduled to
	   run first, and handles a signal, it will not affect the child;
	   and if the child runs first, and handles a signal, it will use
	   the child's stack and not affect the parent.
	   We need to pass 7 words of info to the child: stack, func, arg,
	   and the signal mask to restore.  Since we have only 4 call-saved
	   registers available (%ebx, %esi, %edi, %ebp), we pass only the
	   stack pointer in a register, and the rest through the child's
	   stack.  */
	pushl	%ebp
	movl	%esp, %ebp
	subl	$36, %esp
	movl	%ebx, 32(%esp)

	movl	8(%ebp), %eax	/* flags */
	testl	$32, %eax	/* flags & RFMEM */
	jz	L(simple)

	/* Block all signals.  */
	movl	$-1, %eax
	movl	%eax, 16(%esp)
	movl	%eax, 20(%esp)
	movl	%eax, 24(%esp)
	movl	%eax, 28(%esp)
	leal	16(%esp), %eax
	movl	$SIG_SETMASK, 4(%esp)
	movl	%eax, 8(%esp)
	movl	%eax, 12(%esp)
	DO_CALL (sigprocmask, 3)
	jb	L(error)

	/* Copy all info to the child's stack.  */
	movl	12(%ebp), %ebx	/* stack */
	subl	$32, %ebx	/* room for func, arg, sigset_t */
	andl	$-16, %ebx	/* make it 16-bytes aligned */
	movl	16(%ebp), %eax	/* func */
	movl	20(%ebp), %edx	/* arg */
	movl	%eax, 4(%ebx)
	movl	%edx, (%ebx)
	movl	16(%esp), %eax	/* sigset_t word 0 */
	movl	20(%esp), %edx	/* sigset_t word 1 */
	movl	%eax, 16(%ebx)
	movl	%edx, 20(%ebx)
	movl	24(%esp), %eax	/* sigset_t word 2 */
	movl	28(%esp), %edx	/* sigset_t word 3 */
	movl	%eax, 24(%ebx)
	movl	%edx, 28(%ebx)

	/* Perform the rfork system call.  */
	movl	8(%ebp), %eax	/* flags */
	movl	%eax, 4(%esp)
	DO_CALL (rfork, 1)
	jb	L(error_unmask)

	/* %edx is now 0 for the parent and 1 for the child.  */
	testl	%edx, %edx
	jnz	L(child)

	/* Save the child pid, currently in %eax.  */
	movl	%eax, %ebx

	/* Restore the previous signal mask.  */
	leal	16(%esp), %eax
	movl	$SIG_SETMASK, 4(%esp)
	movl	%eax, 8(%esp)
	movl	$0, 12(%esp)
	DO_CALL (sigprocmask, 3)

	/* Return the child pid, currently in %ebx.  */
	movl	%ebx, %eax
	addl	$32, %esp
	popl	%ebx
	popl	%ebp
	ret

L(child):/* Here we are in the child thread.  */

	/* Set the stack pointer.  */
	movl	%ebx, %esp
	/* Terminate the stack frame.  */
	subl	%ebp, %ebp

	movl	4(%esp), %edi

	/* Restore the previous signal mask.  */
	leal	16(%esp), %eax
	movl	$SIG_SETMASK, 4(%esp)
	movl	%eax, 8(%esp)
	movl	$0, 12(%esp)
	DO_CALL (sigprocmask, 3)

L(child1):
	/* Call func (arg).  */
	call	*%edi

	/* Call _exit.  */
#ifdef PIC
	call	L(here)
L(here):
	popl	%ebx
	addl	$_GLOBAL_OFFSET_TABLE_+[.-L(here)], %ebx
#endif
	pushl	%eax
	call	HIDDEN_JUMPTARGET (_exit)

L(simple):/* Simple case without signal mask handling.  */

	/* Copy all info to the child's stack.  */
	movl	12(%ebp), %ebx	/* stack */
	subl	$8, %ebx	/* room for func, arg */
	andl	$-16, %ebx	/* make it 16-bytes aligned */
	movl	16(%ebp), %eax	/* func */
	movl	20(%ebp), %edx	/* arg */
	movl	%eax, 4(%ebx)
	movl	%edx, (%ebx)

	/* Perform the rfork system call.  */
	movl	8(%ebp), %eax	/* flags */
	movl	%eax, 4(%esp)
	DO_CALL (rfork, 1)
	jb	L(error)

	/* %edx is now 0 for the parent and 1 for the child.  */
	testl	%edx, %edx
	jnz	L(simple_child)

	/* Return the child pid, currently in %eax.  */
	addl	$32, %esp
	popl	%ebx
	popl	%ebp
L(pseudo_end):
	ret

L(simple_child):/* Here we are in the child thread.  */

	/* Set the stack pointer.  */
	movl	%ebx, %esp
	/* Terminate the stack frame.  */
	subl	%ebp, %ebp

	movl	4(%esp), %edi

	jmp	L(child1)

L(error_unmask):

	/* Save the error code, currently in %eax.  */
	movl	%eax, %ebx

	/* Restore the previous signal mask.  */
	leal	16(%esp), %eax
	movl	$SIG_SETMASK, 4(%esp)
	movl	%eax, 8(%esp)
	movl	$0, 12(%esp)
	DO_CALL (sigprocmask, 3)

	/* load saved error code */
	movl	%ebx, %eax

L(error):
	addl	$32, %esp
	popl	%ebx
	popl	%ebp
	jmp	SYSCALL_ERROR_LABEL
	cfi_startproc
PSEUDO_END (__start_thread)
