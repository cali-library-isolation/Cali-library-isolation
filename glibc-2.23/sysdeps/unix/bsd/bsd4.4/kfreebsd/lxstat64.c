/* lxstat using FreeBSD lstat, nlstat system calls.
   Copyright (C) 1991,1995-1997,2000,2002 Free Software Foundation, Inc.
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
#include <sys/stat.h>

#include "stat16conv.c"

int
__lxstat64 (int vers, const char *file, struct stat64 *buf)
{
  if (__builtin_expect (vers == _STAT_VER, 1))
    {
      struct stat16 buf16;
      int result = __syscall_lstat (file, &buf16);
      if (result == 0)
	stat16_to_stat64 (&buf16, buf);
      return result;
    }
  else
    {
      __set_errno (EINVAL);
      return -1;
    }
}
hidden_def (__lxstat64)
