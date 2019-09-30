/* Copyright (C) 1999, 2000, 2002 Free Software Foundation, Inc.
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

#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sysdep.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sysdep-cancel.h>

int
__libc_open (const char *file, int oflag, ...)
{
  int mode = 0;
  int fd;

  if (oflag & O_CREAT)
    {
      va_list arg;
      va_start (arg, oflag);
      mode = va_arg (arg, int);
      va_end (arg);
    }

  if (SINGLE_THREAD_P)
  {
    fd = INLINE_SYSCALL (open, 3, file, oflag, mode);
  }
  else
  {
    int oldtype = LIBC_CANCEL_ASYNC ();
    fd = INLINE_SYSCALL (open, 3, file, oflag, mode);
    LIBC_CANCEL_RESET (oldtype);
  }
  return fd;
}
libc_hidden_def (__libc_open)

weak_alias (__libc_open, __open)
libc_hidden_weak (__open)

weak_alias (__libc_open, open)

strong_alias (__libc_open, __libc_open64)
weak_alias (__libc_open64, __open64)
weak_alias (__libc_open64, open64)
