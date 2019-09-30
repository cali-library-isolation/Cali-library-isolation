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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <libc-internal.h>
#include <ldsodefs.h>
#include <sysdep.h>

int __syscall_clock_getres(clockid_t clock_id, struct timespec *tp);
libc_hidden_proto (__syscall_clock_getres)

int __syscall_clock_gettime(clockid_t clock_id, struct timespec *tp);
libc_hidden_proto (__syscall_clock_gettime)

/* Get resolution of clock.  */
int
__clock_getres (clockid_t clock_id, struct timespec *tp)
{
  /* we could try to provide fallback
     via ??? for CLOCK_REALTIME
     via HP_TIMING for CLOCK_PROCESS_CPUTIME_ID, 
       CLOCK_THREAD_CPUTIME_ID and related timers

     for now just pass it to kernel       
   */
  /* the negative clock_id means a CPU-timer, the resolution is same for all of them
     the kernel returns resolution regardless whether the timer is accessible,
     but POSIX/testsuite expects EINVAL
   */
   
  if (clock_id < 0)
  {
    int rv;
    /* we reuse user provided struct timespec */
    rv = INLINE_SYSCALL (clock_gettime, 2, clock_id, tp);
    if (rv != 0)
      return rv;
    /* valid, now really get the resolution  */
  }

  return INLINE_SYSCALL (clock_getres, 2, clock_id, tp);
}
strong_alias (__clock_getres, clock_getres)
