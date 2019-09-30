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

/* FIXME */

	.text
ENTRY (__start_thread)
PSEUDO_END (__start_thread)
