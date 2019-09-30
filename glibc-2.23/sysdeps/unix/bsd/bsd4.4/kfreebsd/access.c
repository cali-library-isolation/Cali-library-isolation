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
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sysdep.h>

/*
   The FreeBSD kernel do not test file access correctly when the 
   process' real user ID is superuser. In particular, they always return
   zero when testing execute permissions without regard to whether the 
   file is executable.

   While this behaviour conforms to POSIX.1-2008, it is explicitely 
   discouraged. This wrapper implements the recommended behaviour.
 */

extern int __syscall_access (const char *path, int mode);
libc_hidden_proto (__syscall_access)

int
__access (const char *path, int mode)
{
  struct stat64 stats;

  if ((__getuid() != 0) || !(mode & X_OK))
    return __syscall_access (path, mode);

  /* Althought the super-user can read and write any file, 
     the file-system might be i.e. read-only. Do the check. */
     
  if (__syscall_access (path, mode))
    return -1;
    
  if (stat64 (path, &stats))
    return -1;

  /* The super-user can execute any file that anyone can execute. */
  if (stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
    return 0;

  __set_errno (EACCES);
  return -1;
}

weak_alias (__access, access)
