/* Save current context.
   Copyright (C) 2006 Free Software Foundation, Inc.
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

.text
ENTRY(__getcontext)
        clc
        movq 0(%rsp), %rsi	/* save the return PC value */

        DO_CALL (getcontext, 1)
        jb SYSCALL_ERROR_LABEL

        addq $8, %rsp		/* remove stale return address */
        jmp  *%rsi

L(pseudo_end):
	ret

PSEUDO_END(__getcontext)

weak_alias(__getcontext, getcontext)
