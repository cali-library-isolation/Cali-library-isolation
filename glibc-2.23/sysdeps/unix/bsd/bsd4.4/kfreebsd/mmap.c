/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.

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

extern void *__syscall_mmap (void *__addr, size_t __len, int __prot,
			     int __flags, int __fd, __off_t __offset) __THROW;
libc_hidden_proto (__syscall_mmap)

void *
__mmap (void *addr, size_t len, int prot, int flags, int fd, __off_t offset)
{
  /* Validity checks not done by the kernel.  */
  if (offset != 0)
    {
      int pagesize = __getpagesize ();
      if ((__builtin_expect (pagesize & (pagesize - 1), 0)
        ? offset % pagesize
	: offset & (pagesize - 1)))
	{
	  __set_errno (EINVAL);
	  return (void *) (-1);
	}
    }

  /* for ANON mapping we must pass -1 in place of fd */
  if (flags & MAP_ANON)
    fd = -1;

  return INLINE_SYSCALL (mmap, 6, addr, len, prot, flags, fd, offset);
}

weak_alias (__mmap, mmap)

/* 'mmap64' is the same as 'mmap', because __off64_t == __off_t.  */
strong_alias (__mmap, __mmap64)
weak_alias (__mmap64, mmap64)
