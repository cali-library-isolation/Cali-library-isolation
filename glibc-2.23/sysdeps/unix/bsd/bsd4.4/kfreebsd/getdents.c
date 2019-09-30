/* Read directory entries, 3 argument function.  FreeBSD version.
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

#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <sysdep.h>

/* Use the 3-argument system call.  */

extern int __syscall_getdents (int fd, char *buf, size_t nbytes);
libc_hidden_proto (__syscall_getdents)

/* Read directory entries from FD into BUF, reading at most NBYTES.
   Returns the number of bytes read; zero when at end of directory; or
   -1 for errors.  */
ssize_t
internal_function
__getdents (int fd, char *buf, size_t nbytes)
{
  return __syscall_getdents (fd, buf, nbytes);
}

/* Since 'struct dirent64' == 'struct dirent', the functions '__getdents64'
   and '__getdents' are equal.  */
strong_alias (__getdents, __getdents64)
