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
#include <sys/time.h>
#include <sys/user.h>
#include <kernel-features.h>

extern int __syscall_futimesat (int fd, const char *path,
				const struct timeval *times);
libc_hidden_proto (__syscall_futimesat)

/* Change the access time of FILE relative to FD to TVP[0] and
   the modification time of FILE to TVP[1].  */
int
futimesat (int fd, const char *file, const struct timeval tvp[2])
{
      if (file == NULL)
	return __futimes (fd, tvp);

      return INLINE_SYSCALL (futimesat, 3, fd, file, tvp);
}
