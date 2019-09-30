/* Copyright (C) 1993, 1995-1998, 2002 Free Software Foundation, Inc.
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

	  .text;
ENTRY (__syscall)
	popl %ecx		/* Pop return address into %ecx.  */
	cfi_adjust_cfa_offset (-4)
	movl 0(%esp), %eax	/* Load syscall number into %eax. */
	int $0x80		/* Do the system call.  */
	pushl %ecx		/* Push back return address.  */
	cfi_adjust_cfa_offset (4)
	jb SYSCALL_ERROR_LABEL;	/* Jump to error handler if error.  */

L(pseudo_end):
	ret			/* Return to caller.  */
PSEUDO_END (__syscall)
weak_alias (__syscall, syscall)
