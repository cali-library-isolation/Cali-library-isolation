/* Copyright (C) 2007-2013 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <fcntl.h>
#include <kernel-features.h>
#include <sysdep.h>

extern int __syscall_posix_fallocate(int fd, off_t offset, off_t len);
libc_hidden_proto (__syscall_posix_fallocate)

#define posix_fallocate static internal_fallocate
#include <sysdeps/posix/posix_fallocate.c>
#undef posix_fallocate

#if !defined __ASSUME_FALLOCATE
static int __have_fallocate;
#endif


/* Reserve storage for the data of the file associated with FD.  */
int
__posix_fallocate (int fd, __off_t offset, __off_t len)
{
#ifndef __ASSUME_FALLOCATE
    if (__have_fallocate >= 0)
#endif
    {
        int res = INLINE_SYSCALL (posix_fallocate, 3, fd, offset, len);
      
        if (res == -1)
            res = errno;
#ifndef __ASSUME_FALLOCATE
        if (res == ENOSYS)
        {
            __have_fallocate = -1;
        }
        else
#endif
        {
            if (res != EOPNOTSUPP)
                return res;
        }
    }      
    return internal_fallocate (fd, offset, len);
}
strong_alias (__posix_fallocate, posix_fallocate)

/* 'posix_fallocate64' is the same as 'posix_fallocate', because __off64_t == __off_t.  */
/*  but previous prototype have different size of len parameter */

#include <shlib-compat.h>
#include <bits/wordsize.h>

#if __WORDSIZE == 32 && SHLIB_COMPAT(libc, GLIBC_2_2, GLIBC_2_3_3)

int
attribute_compat_text_section
__posix_fallocate64_l32 (int fd, off64_t offset, size_t len)
{
  return __posix_fallocate (fd, offset, len);
}

versioned_symbol (libc, __posix_fallocate, posix_fallocate64, GLIBC_2_3_3);
compat_symbol (libc, __posix_fallocate64_l32, posix_fallocate64, GLIBC_2_2);
#else
weak_alias (__posix_fallocate, posix_fallocate64)
#endif
