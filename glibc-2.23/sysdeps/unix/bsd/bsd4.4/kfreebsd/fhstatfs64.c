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

#include <sys/mount.h>

#include "statfsconv.c"

int
fhstatfs64 (const fhandle_t *fhp, struct statfs64 *buf)
{
  struct statfs_fbsd5 kbuf;

  if (__syscall_fhstatfs (fhp, &kbuf) < 0)
    return -1;

  /* Convert a 'struct statfs' to 'struct statfs64'.  */
  statfs5_to_statfs64 (&kbuf, buf);

  return 0;
}
