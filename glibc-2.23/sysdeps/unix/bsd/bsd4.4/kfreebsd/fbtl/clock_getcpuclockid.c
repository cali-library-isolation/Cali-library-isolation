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
#include <time.h>
#include <unistd.h>
#include <kernel-features.h>

int
__clock_getcpuclockid (pid_t pid, clockid_t *clock_id)
{
   return INLINE_SYSCALL(clock_getcpuclockid2, 3, pid, CPUCLOCK_WHICH_PID, clock_id);
}
strong_alias (__clock_getcpuclockid, clock_getcpuclockid)
