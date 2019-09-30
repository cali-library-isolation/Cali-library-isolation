/* Copyright (C) 1991-1992, 1994-1995, 1997, 2002 Free Software Foundation, Inc.
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
ENTRY (__rfork)
	/* save the return PC value into ECX,
	   preserve stack frame with argument  */
	movl 0(%esp), %ecx;
	DO_CALL (rfork, 1)
	jb SYSCALL_ERROR_LABEL;
	/* R1 is now 0 for the parent and 1 for the child.  Decrement it to
	   make it -1 (all bits set) for the parent, and 0 (no bits set)
	   for the child.  Then AND it with R0, so the parent gets
	   R0&-1==R0, and the child gets R0&0==0.  */
	decl %edx
	andl %edx, %eax

	popl %edx
	cfi_adjust_cfa_offset(-4)
	jmp  *%ecx

L(pseudo_end):
	ret
PSEUDO_END (__rfork)

weak_alias (__rfork, rfork)

