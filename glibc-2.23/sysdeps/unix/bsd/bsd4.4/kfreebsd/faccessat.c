/* Test for access to file, relative to open directory.  Linux version.
   Copyright (C) 2006 Free Software Foundation, Inc.
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
#include <kernel-features.h>
#include <sysdep.h>

extern int __syscall_faccessat (int fd, const char *path, int mode, int flag);
libc_hidden_proto (__syscall_faccessat)

/*
   The FreeBSD kernel do not test file access correctly when the 
   process' real user ID is superuser. In particular, they always return
   zero when testing execute permissions without regard to whether the 
   file is executable.

   While this behaviour conforms to POSIX.1-2008, it is explicitely 
   discouraged. This wrapper implements the recommended behaviour.
 */

int
faccessat (int fd, const char *file, int mode, int flag)
{
        int result = INLINE_SYSCALL (faccessat, 4, fd, file, mode, flag);
        if ((result == 0) && (mode & X_OK))
        {
          uid_t uid = (flag & AT_EACCESS) ? __geteuid () : __getuid ();
          if (uid == 0)
          {
            struct stat64 stats;
            if (fstatat64 (fd, file, &stats, flag & AT_SYMLINK_NOFOLLOW))
              return -1;
            if ((stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) == 0)
            {
              __set_errno (EACCES);
              return -1;
	    }
          }
	}
	return result;
}
