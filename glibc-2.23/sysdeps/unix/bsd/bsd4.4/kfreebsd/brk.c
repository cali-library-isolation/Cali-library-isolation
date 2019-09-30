/* Copyright (C) 2004, 2010 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Robert Millan

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

#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

extern int __syscall_obreak (void *addr);
libc_hidden_proto (__syscall_obreak)

extern char _end[];

/* sbrk.c expects this.  */
void *__curbrk = _end;

/* Set the end of the process's data space to ADDR.
   Return 0 if successful, -1 if not.  */
int
__brk (void *addr)
{
  if ((char*)addr < _end)
    return 0;

  if (INLINE_SYSCALL (obreak, 1, addr) == -1)
    {
      __set_errno (ENOMEM);
      return -1;
    }

  __curbrk = addr;
  return 0;
}
weak_alias (__brk, brk)
