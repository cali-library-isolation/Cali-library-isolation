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

#include <errno.h>
#include <sys/eventfd.h>
#include <sysdep.h>
#include <kernel-features.h>


int
eventfd (unsigned int count, int flags)
{
#ifdef __NR_eventfd2
  int res = INLINE_SYSCALL (eventfd2, 2, count, flags);
# ifndef __ASSUME_EVENTFD2
  if (res != -1 || errno != ENOSYS)
# endif
    return res;
#endif

#ifndef __ASSUME_EVENTFD2
  /* The old system call has no flag parameter which is bad.  So we have
     to wait until we have to support to pass additional values to the
     kernel (sys_indirect) before implementing setting flags like
     O_NONBLOCK etc.  */
  if (flags != 0)
    return INLINE_SYSCALL_ERROR_RETURN_VALUE (EINVAL);

# ifdef __NR_eventfd
  return INLINE_SYSCALL (eventfd, 1, count);
# else
  return INLINE_SYSCALL_ERROR_RETURN_VALUE (ENOSYS);
# endif
#elif !defined __NR_eventfd2
# error "__ASSUME_EVENTFD2 defined but not __NR_eventfd2"
#endif
}
