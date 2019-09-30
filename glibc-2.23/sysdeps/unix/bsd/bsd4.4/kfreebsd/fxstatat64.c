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

#include "stat16conv.c"

extern int __syscall_fstatat (int fd, const char *path,
			      struct stat16 *buf, int flag);
libc_hidden_proto (__syscall_fstatat)

/* Get information about the file NAME relative to FD in ST.  */
int
__fxstatat64 (int vers, int fd, const char *file, struct stat64 *st, int flag)
{
      int result;

      if (__builtin_expect (vers == _STAT_VER, 1))
	{
	  struct stat16 buf16;
	  result =
	    INLINE_SYSCALL (fstatat, 4, fd, file,
			    &buf16, flag);
	  if (result == 0)
	    stat16_to_stat64 (&buf16, st);
	}
      else
	{
	  __set_errno (EINVAL);
	  return -1;
	}
	return result;
}

libc_hidden_def (__fxstatat64)
