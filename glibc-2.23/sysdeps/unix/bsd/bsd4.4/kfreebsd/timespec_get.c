/* Copyright (C) 2011-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <time.h>
#include <sysdep.h>
#include <kernel-features.h>

/* Set TS to calendar time based in time base BASE.  */

/* if the timespec_get function is successful 
   it returns the nonzero value base;
   otherwise, it returns zero.
 */

int __syscall_clock_gettime(clockid_t clock_id, struct timespec *tp);
libc_hidden_proto (__syscall_clock_gettime)

int
timespec_get (struct timespec *ts, int base)
{
  switch (base)
    {
    case TIME_UTC:
      if ( 0 != INLINE_SYSCALL (clock_gettime, 2, CLOCK_REALTIME, ts))
	return 0;
      break;

    default:
      return 0;
    }

  return base;
}
