/* Copyright (C) 2002 Free Software Foundation, Inc.
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

/* lseek returns a 64-bit result in %eax, %edx.  This means that in the
   error case we have to set both %eax and %edx to -1.  */

PSEUDO_NOERRNO(__lseek, lseek, 3)
        jnb L(no_error)
	call SYSCALL_ERROR_LABEL;
	orl $-1, %edx;
L(pseudo_end):
L(no_error):
	ret
PSEUDO_END(__lseek)
libc_hidden_def (__lseek)

strong_alias (__lseek, __libc_lseek)
libc_hidden_def (__libc_lseek)

strong_alias (__lseek, __libc_lseek64)
libc_hidden_def (__libc_lseek64)

strong_alias (__lseek, __lseek64)
libc_hidden_def (__lseek64)

strong_alias (__lseek, __llseek)
libc_hidden_def (__llseek)

weak_alias (__lseek, lseek)
libc_hidden_weak (lseek)

weak_alias (__lseek, lseek64)
libc_hidden_weak (lseek64)

weak_alias (__lseek, llseek)
libc_hidden_weak (llseek)
