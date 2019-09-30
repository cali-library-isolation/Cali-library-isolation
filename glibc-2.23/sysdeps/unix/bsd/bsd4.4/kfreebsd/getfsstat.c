/* Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <sys/mount.h>
#include <alloca.h>

#include "statfsconv.c"

int
__getfsstat (struct statfs *buf, long bufsize, int flags)
{
  long bufcount;
  struct statfs_fbsd5 *tmpbuf;
  int count, i;

  if (bufsize < 0)
    bufsize = 0;
  bufcount = bufsize / sizeof (struct statfs);

  if ((bufcount == 0) || (buf == NULL))
    tmpbuf = NULL;
  else
    tmpbuf = alloca(bufcount * sizeof (struct statfs_fbsd5));

  count = __syscall_getfsstat (tmpbuf, bufcount * sizeof (struct statfs_fbsd5), flags);
  if (tmpbuf && count > 0)
    for (i = count - 1; i >= 0; i--)
      statfs5_to_statfs (&tmpbuf[i], &buf[i]);

  return count;
}

weak_alias (__getfsstat, getfsstat)
