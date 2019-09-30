/* Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.
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
#include <sched.h>
#include <string.h>
#include <sysdep.h>
#include <unistd.h>
#include <sys/types.h>

#include "cpuset-kern.h"

int
__libc_sched_setaffinity (pid_t pid, size_t cpusetsize, const cpu_set_t *cpuset)
{
  int res;
  int64_t id;

  if (pid < 0)
    {
      __set_errno(ESRCH);
      return -1;
    }

  if (pid == 0)		// user   level "self"
      id = -1;		// kernel level "self"
  else
      id = pid;

  res = INLINE_SYSCALL (cpuset_setaffinity, 5, CPU_LEVEL_WHICH,
			CPU_WHICH_PID, id, cpusetsize, cpuset);

  if (errno == ERANGE || errno == EDEADLK)
    {
      __set_errno(EINVAL);
    }

  return res;
}

strong_alias (__libc_sched_setaffinity, __sched_setaffinity)
weak_alias (__sched_setaffinity, sched_setaffinity)
