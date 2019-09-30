/* Copyright (C) 2009 Free Software Foundation, Inc.
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
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sysdep.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <kernel-features.h>

extern int __syscall_mknodat (int fd, const char *path,
			      mode_t mode, dev_t dev);
libc_hidden_proto (__syscall_mknodat)

extern int __syscall_mkfifoat (int fd, const char *path,
			       mode_t mode);
libc_hidden_proto (__syscall_mkfifoat)

/* Create a device file named PATH relative to FD, with permission and
   special bits MODE and device number DEV (which can be constructed
   from major and minor device numbers with the `makedev' macro above).  */
int
__xmknodat (int vers, int fd, const char *file, mode_t mode, dev_t * dev)
{
  if (vers != _MKNOD_VER)
    {
      __set_errno (EINVAL);
      return -1;
    }

      /* The FreeBSD mknod() system call cannot be used to create FIFOs; we
         must use the mkfifo() system call for this purpose.  */
      if (S_ISFIFO (mode))
	return INLINE_SYSCALL (mkfifoat, 3, fd, file, mode);
      else
	return INLINE_SYSCALL (mknodat, 4, fd, file, mode, *dev);
}

libc_hidden_def (__xmknodat)
