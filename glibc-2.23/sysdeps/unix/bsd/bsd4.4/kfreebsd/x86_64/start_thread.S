/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.
   Modification for amd64 contributed by Petr Salinger, 2006.

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

/*
   The parameters are passed in registers:
   rdi: flags for rfork
   rsi: child_stack
   rdx: func
   rcx: arg
*/

#include <sysdep.h>
#include <asm-syntax.h>

#define SIG_SETMASK	3

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
*/

	.text
ENTRY (__start_thread)

        /* Insert the argument onto the new aligned stack.  */
        andq	$-16,%rsi
        subq    $16,%rsi
        movq    %rcx,8(%rsi)

        /* Save the function pointer.  It will be popped off in the child */
        movq    %rdx,0(%rsi)

	testq	$32, %rdi	/* flags & RFMEM */
	jnz	L(complex)

        /* Do the system call.  */
	movl    $SYS_ify(rfork),%eax
        /* End FDE now, because in the child the unwind info will be
           wrong.  */
        cfi_endproc;
        syscall			/* rdi and rsi are extra preserved */

        jb SYSCALL_ERROR_LABEL

	testq   %rdx,%rdx	/*  0 for the parent and 1 for the child */
	jz     L(pseudo_end)	/* just return in parent */

L(thread_start):

	/* set up stack */
	movq	%rsi, %rsp

        /* Clear the frame pointer.  The ABI suggests this be done, to mark
           the outermost frame obviously.  */
        xorl    %ebp, %ebp

L(thread_start2):

        /* Set up arguments for the function call.  */
        popq    %rax            /* Function to call.  */
        popq    %rdi            /* Argument.  */
        call    *%rax

        /* Call exit with return value from function call. */
        movq    %rax, %rdi
        call    HIDDEN_JUMPTARGET (_exit)


/******************************************************************************************************
 *
 * and now the complex one ...
 *
 ******************************************************************************************************/

L(complex):

        subq    $32, %rsp

        /* save arguments - flags and stack */
        movq    %rdi,0(%rsp)
        movq    %rsi,8(%rsp)

        /* Block all signals.  */
        orq     $-1, %rax
        movq    %rax, 16(%rsp)
        movq    %rax, 24(%rsp)

        leaq    16(%rsp), %rsi
        movl    $SIG_SETMASK, %edi
        movq    %rsi,%rdx
        DO_CALL (sigprocmask, 3)
        jb      L(error)

        /* restore arguments - flags and stack */
        movq    0(%rsp),%rdi
        movq    8(%rsp),%rsi

        /* Copy mask info into the child's stack.  */

        subq    $16,%rsi
        movq    16(%rsp),%rcx
        movq    24(%rsp),%rdx
        movq    %rcx, 0(%rsi)
        movq    %rdx, 8(%rsi)

        /* Perform the rfork system call.  */
        DO_CALL (rfork, 1)
        jb      L(error_unmask)

        /* %rdx is now 0 for the parent and 1 for the child.  */
        testq   %rdx, %rdx
        jnz     L(child)

        /* Save the child pid, currently in %rax.  */
        movq    %rax, 0(%rsp)

        /* Restore the previous signal mask.  */
        movl    $SIG_SETMASK, %edi
        leaq    16(%rsp), %rsi
        xorl    %edx,%edx
        DO_CALL (sigprocmask, 3)

        /* Return the child pid, saved on stack.  */
        movq    0(%rsp), %rax
        addq    $32, %rsp
L(pseudo_end):
        ret

L(error_unmask):

        /* Save the error code, currently in %rax.  */
        movq    %rax, 0(%rsp)

        /* Restore the previous signal mask.  */
        movq    $SIG_SETMASK, %rdi
        leaq    16(%rsp), %rsi
        xorl    %edx,%edx
        DO_CALL (sigprocmask, 3)

	/* load saved error code */
        movq    0(%rsp), %rax

L(error):
        addq    $32, %rsp
        jmp     SYSCALL_ERROR_LABEL

L(child):/* Here we are in the child thread.  */

        /* set up stack */
        movq    %rsi, %rsp

        /* Clear the frame pointer.  The ABI suggests this be done, to mark
           the outermost frame obviously.  */
        xorl    %ebp, %ebp

        /* Restore the previous signal mask.  */
        movq    $SIG_SETMASK, %rdi
        xorl    %edx,%edx
        DO_CALL (sigprocmask, 3)

        addq    $16, %rsp
        jmp     L(thread_start2)

        cfi_startproc;

PSEUDO_END (__start_thread)
