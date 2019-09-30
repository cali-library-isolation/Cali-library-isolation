/* fxstat using FreeBSD fstat, nfstat system calls.
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
__fxstat (int vers, int fd, struct stat *buf)
{
  if (__builtin_expect (vers == _STAT_VER, 1))
    {
      struct stat16 buf16;
      int result = __syscall_fstat (fd, &buf16);
      if (result == 0)
	stat16_to_stat (&buf16, buf);
      return result;
    }
  else if (__builtin_expect (vers == _STAT_VER_stat, 1))
    return __syscall_fstat (fd, (struct stat16 *) buf);
  else
    {
      __set_errno (EINVAL);
      return -1;
    }
}
hidden_def (__fxstat)

weak_alias (__fxstat, _fxstat)
