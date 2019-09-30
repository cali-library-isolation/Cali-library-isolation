/* writev for FreeBSD.
   Copyright (C) 1997-1998, 2000, 2002 Free Software Foundation, Inc.
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

#include <errno.h>
#include <stddef.h>
#include <sys/param.h>
#include <sys/uio.h>

#include <sysdep.h>
#include <sys/syscall.h>
#include <sysdep-cancel.h>

extern ssize_t __syscall_writev (int, const struct iovec *, int);

static ssize_t __atomic_writev_replacement (int, const struct iovec *,
					    int) internal_function;

ssize_t
__writev (int fd, const struct iovec *vector, int count)
{
  if (count <= UIO_MAXIOV)
  {
    if (SINGLE_THREAD_P)
      return INLINE_SYSCALL (writev, 3, fd, vector, count);

    int oldtype = LIBC_CANCEL_ASYNC ();
    ssize_t result =  INLINE_SYSCALL (writev, 3, fd, vector, count);
    LIBC_CANCEL_RESET (oldtype);
    return result;

  }
  else
    return __atomic_writev_replacement (fd, vector, count);
}
weak_alias (__writev, writev)

#undef weak_alias
#define weak_alias(a,b)
#define __writev static internal_function __atomic_writev_replacement
#include <sysdeps/posix/writev.c>

