/* Return information about the filesystem on which FD resides.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <sys/statvfs.h>
#include <sys/statfs.h>

#include "statfsconv.c"

/* Return information about the filesystem on which FD resides.  */
int
__fstatvfs64 (int fd, struct statvfs64 *buf)
{
  struct statfs_fbsd5 kbuf;

  if (__syscall_fstatfs (fd, &kbuf) < 0)
    return -1;

  /* Convert a 'struct statfs' to 'struct statvfs64'.  */
  statfs5_to_statvfs64 (&kbuf, buf);

  return 0;
}

weak_alias (__fstatvfs64, fstatvfs64)
