/* Copyright (C) 2006 Free Software Foundation, Inc.
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

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <sysdep.h>

extern int __syscall_munmap (void *__addr, size_t __len) __THROW;
libc_hidden_proto (__syscall_munmap)

int
__munmap (void *addr, size_t len)
{
    int pagesize = __getpagesize ();
    if ((unsigned long) addr & (pagesize - 1))
    {
	__set_errno (EINVAL);
	return -1;
    }
  return INLINE_SYSCALL (munmap, 2, addr, len);
}

weak_alias (__munmap, munmap)
