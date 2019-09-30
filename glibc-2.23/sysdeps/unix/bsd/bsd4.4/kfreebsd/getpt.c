/* Copyright (C) 1998-1999, 2000-2002 Free Software Foundation, Inc.
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
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sysdep.h>

/* The system call does not change the controlling terminal, so we have
 * to do it ourselves.  */
extern int __syscall_posix_openpt (int oflag);
libc_hidden_proto (__syscall_posix_openpt)

int
__posix_openpt (int oflag)
{
  int fd = INLINE_SYSCALL (posix_openpt, 1, oflag);
  if (fd >= 0)
  {
      if (!(oflag & O_NOCTTY))
        __ioctl (fd, TIOCSCTTY, NULL);
  }
  return fd;
}

weak_alias (__posix_openpt, posix_openpt)

int
__getpt (void)
{
  return __posix_openpt (O_RDWR | O_NOCTTY);
}

weak_alias (__getpt, getpt)
