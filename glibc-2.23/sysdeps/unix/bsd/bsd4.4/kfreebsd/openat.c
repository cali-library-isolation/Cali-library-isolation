/* Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.
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
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sysdep.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/user.h>
#include <kernel-features.h>
#include <sysdep-cancel.h>
#include <not-cancel.h>

extern int __syscall_openat (int fd, const char *path, int flag, mode_t mode);
libc_hidden_proto (__syscall_openat)

/* Open FILE with access OFLAG.  Interpret relative paths relative to
   the directory associated with FD.  If OFLAG includes O_CREAT, a
   third argument is the file protection.  */
int
__openat (int fd, const char *file, int oflag, ...)
{
  int mode = 0;
  int result;

  if (oflag & O_CREAT)
    {
      va_list arg;
      va_start (arg, oflag);
      mode = va_arg (arg, int);
      va_end (arg);
    }
  
  if (SINGLE_THREAD_P)
    return INLINE_SYSCALL (openat, 4, fd, file, oflag, mode);
  else
    {
      int oldtype = LIBC_CANCEL_ASYNC ();
      result = INLINE_SYSCALL (openat, 4, fd, file, oflag, mode);
      LIBC_CANCEL_RESET (oldtype);
    }
  return result;
}

libc_hidden_def (__openat)
weak_alias (__openat, openat)

/* 'openat64' is the same as 'openat', because __off64_t == __off_t.  */
strong_alias (__openat, __openat64)
libc_hidden_def (__openat64)
weak_alias (__openat64, openat64)
