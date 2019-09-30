/* Copyright (C) 1999, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

/* Clone the calling process, but without copying the whole address space.
   The calling process is suspended until the new process exits or is
   replaced by a call to `execve'.  Return -1 for errors, 0 to the new process,
   and the process ID of the new process to the old process.  */

ENTRY (__vfork)

	/* Pop the return PC value into RDI.  */
	popq	%rdi
	cfi_adjust_cfa_offset(-8)
	cfi_register(%rip, %rdi)

#ifdef SAVE_PID
        SAVE_PID
#endif
	/* Perform the system call.  */
	DO_CALL (vfork, 0)
	jb	L(error)	/* Branch forward if it failed.  */

	/* In the parent process, %rdx == 0, %rax == child pid.  */
	/* In the child process, %rdx == 1, %rax == parent pid.  */

	/* Change %rax to be 0 in the child process.  */
	decq	%rdx
	andq	%rdx, %rax

#ifdef RESTORE_PID_IN_PARENT
	jz L(norestore)
        RESTORE_PID_IN_PARENT
L(norestore):
#endif

	/* Jump to the return PC.  */
	jmp	*%rdi

L(error):
	/* Push back the return PC.  */
	pushq	%rdi
	cfi_adjust_cfa_offset(8)
#ifdef RESTORE_PID_IN_PARENT
        RESTORE_PID_IN_PARENT
#endif
	/* Branch to the error handler, hidden in PSEUDO_END.  */
	jmp	SYSCALL_ERROR_LABEL
L(pseudo_end):
	ret
PSEUDO_END (__vfork)
libc_hidden_def (__vfork)

weak_alias (__vfork, vfork)
