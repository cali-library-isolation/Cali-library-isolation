/* Copyright (C) 1997-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#ifndef NO_SGIDEFS_H
#include <sgidefs.h>
#endif
#include <unistd.h>
#include <endian.h>

#include <sysdep-cancel.h>
#include <sys/syscall.h>

#ifdef __NR_pread64             /* Newer kernels renamed but it's the same.  */
# ifdef __NR_pread
#  error "__NR_pread and __NR_pread64 both defined???"
# endif
# define __NR_pread __NR_pread64
#endif


ssize_t
__libc_pread64 (int fd, void *buf, size_t count, off64_t offset)
{
#if _MIPS_SIM == _ABIN32 || _MIPS_SIM == _ABI64
  return SYSCALL_CANCEL (pread, fd, buf, count, offset);
#else
  return SYSCALL_CANCEL (pread, fd, buf, count, 0,
			 __LONG_LONG_PAIR ((off_t) (offset >> 32),
					   (off_t) (offset & 0xffffffff)));
#endif
}

weak_alias (__libc_pread64, __pread64)
weak_alias (__libc_pread64, pread64)
