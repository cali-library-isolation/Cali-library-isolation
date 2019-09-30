/* Copyright (C) 2007-2016 Free Software Foundation, Inc.
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

#define posix_fallocate static internal_fallocate
#include <sysdeps/posix/posix_fallocate.c>
#undef posix_fallocate

/* The alpha architecture introduced the fallocate system call in
   2.6.33-rc1, so we still need the fallback code.  */
#if !defined __ASSUME_FALLOCATE && defined __NR_fallocate
static int __have_fallocate;
#endif


/* Reserve storage for the data of the file associated with FD.  */
int
posix_fallocate (int fd, __off_t offset, __off_t len)
{
#ifdef __NR_fallocate
# ifndef __ASSUME_FALLOCATE
  if (__glibc_likely (__have_fallocate >= 0))
# endif
    {
      INTERNAL_SYSCALL_DECL (err);
# ifdef INTERNAL_SYSCALL_TYPES
      int res = INTERNAL_SYSCALL_TYPES (fallocate, err, 4, int, fd,
					int, 0, off_t, offset,
					off_t, len);
# else
      int res = INTERNAL_SYSCALL (fallocate, err, 4, fd, 0, offset, len);
# endif

      if (! INTERNAL_SYSCALL_ERROR_P (res, err))
	return 0;

# ifndef __ASSUME_FALLOCATE
      if (__glibc_unlikely (INTERNAL_SYSCALL_ERRNO (res, err) == ENOSYS))
	__have_fallocate = -1;
      else
# endif
	if (INTERNAL_SYSCALL_ERRNO (res, err) != EOPNOTSUPP)
	  return INTERNAL_SYSCALL_ERRNO (res, err);
    }
#endif

  return internal_fallocate (fd, offset, len);
}
weak_alias (posix_fallocate, posix_fallocate64)
