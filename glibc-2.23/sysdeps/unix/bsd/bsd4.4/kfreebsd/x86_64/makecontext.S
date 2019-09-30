/* makecontext() for FreeBSD/amd64.
   Copyright (C) 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Petr Salinger, 2006.

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
#include <ucontext_i.h>

/*
   void makecontext(ucontext_t *ucp, void *func(), int argc, ...);


   input parameters:

   rdi		ucp
   rsi		func
   edx		argc
   rcx		arg1	-> ucp.rdi
   r8		arg2	-> ucp.rsi
   r9		arg3	-> ucp.rdx
    8(%rsp)	arg4	-> ucp.rcx
   16(%rsp)	arg5	-> ucp.r8
   24(%rsp)	arg6	-> ucp.r9
   i*8+24(%rsp) arg(6+i)-> future stack

*/

ENTRY(__makecontext)

	/* Compute the address of the stack.
	   The information comes from us_stack element. */
	movq	oSS_SP(%rdi), %rax
	addq	oSS_SIZE(%rdi), %rax

	/* Put start addr into ucp */
	movq	%rsi, oRIP(%rdi)

	/* properly align future stack */
	andq $-16, %rax
	subq   $8, %rax

	/* Put the next context into preserved ucp.rbx
	   (from the uc_link element).  */
	movq	oLINK(%rdi), %rsi
	movq	%rsi, oRBX(%rdi)

	/* save future registers arguments */

	orl %edx, %edx
	jz L(param_done)

	decl %edx
	movq %rcx, oRDI(%rdi)
	jz L(param_done)

	decl %edx
	movq %r8,  oRSI(%rdi)
	jz L(param_done)

	decl %edx
	movq %r9,  oRDX(%rdi)
	jz L(param_done)

	movq 8(%rsp), %rsi
	decl %edx
	movq %rsi, oRCX(%rdi)
	jz L(param_done)

	movq 16(%rsp), %rcx
	decl %edx
	movq %rcx, oR8(%rdi)
	jz L(param_done)

	movq 24(%rsp), %rsi
	decl %edx
	movq %rsi, oR9(%rdi)
	jz L(param_done)

	/* we have to copy some arguments also on the future stack */
	movl %edx, %ecx
	negq %rdx
	leaq -16(%rax,%rdx,8), %rax /* we a litle bit waste stack space */
	orq  $8, %rax

1:	movq 24(%rsp,%rcx,8), %rsi
	movq %rsi,(%rax,%rcx,8)
	loop 1b

L(param_done):

	/* Store the future stack pointer and chain helper code. */
	leaq	L(exitcode)(%rip), %rdx
	movq	%rax, oRSP(%rdi)
	movq	%rdx, (%rax)

	/* 'makecontext' returns no value.  */
	ret

/***************************************************************************/

	/* This is the helper code which gets called if a function which
	   is registered with 'makecontext' returns.  In this case we
	   have to install the context listed in the uc_link element of
	   the context 'makecontext' manipulated at the time of the
	   'makecontext' call.  If the pointer is NULL the process must
	   terminate.  */

L(exitcode):
	/* we don't bother with extra arguments left on the stack
		- more than 6 arguments is rare
		- setcontext changes stacks immediately
		- exit will call _exit in near future
	*/

	movq %rbx, %rdi		/* argument for either syscall */
	testq %rbx, %rbx	/* If it is zero exit.  */
	jz 2f

	call	JUMPTARGET(__setcontext)
	/* If this returns (which can happen if the syscall fails) we'll
	   exit the program with the return error value (-1).  */

	movq %rax, %rdi
2:	call	HIDDEN_JUMPTARGET(exit)
	/* The 'exit' call should never return.  In case it does cause
	   the process to terminate.  */
	hlt
END(__makecontext)

weak_alias(__makecontext, makecontext)

