/* Copyright (C) 2013 Free Software Foundation, Inc.
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

extern int __syscall_posix_fadvise(int fd, off_t offset, off_t len, int advice);
libc_hidden_proto (__syscall_posix_fadvise)

/* the syscall is available in 8.x since 8.3 and in 9.1 and above */
/* i.e. it is not supported in 9.0 kernel */

int
posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
    int rv;
    rv = INLINE_SYSCALL (posix_fadvise, 4, fd, offset, len, advice);
    if (rv == -1)
    {
        if (errno == ENOSYS)	/* cheat under old kernels as successfull */
            return 0;
        return errno;
    }
    return rv;
}

weak_alias (posix_fadvise, posix_fadvise64)
