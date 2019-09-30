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

#include <alloca.h>
#include <errno.h>
#include <pthreadP.h>
#include <sysdep.h>
#include <sys/types.h>
#include <shlib-compat.h>

#include "../cpuset-kern.h"


size_t __kernel_cpumask_size attribute_hidden;

/* Determine the current affinity.  As a side affect we learn
   about the size of the cpumask_t in the kernel.  */
int
__determine_cpumask_size (pid_t tid)
{
  /* not yet dynamical */
  __kernel_cpumask_size = sizeof(cpu_set_t);
  return 0;
}

int
__pthread_setaffinity_new (pthread_t th, size_t cpusetsize,
			   const cpu_set_t *cpuset)
{
  const struct pthread *pd = (const struct pthread *) th;
  int64_t id = pd->tid;
  int res;

  res = INLINE_SYSCALL (cpuset_setaffinity, 5, CPU_LEVEL_WHICH,
			CPU_WHICH_TID, id, cpusetsize, cpuset);

  if (res == 0)
      return 0;
      
  if ((errno == ERANGE) || (errno == EDEADLK))
    {
      return EINVAL;
    }
  
  return errno;
}
versioned_symbol (libpthread, __pthread_setaffinity_new,
		  pthread_setaffinity_np, GLIBC_2_3_4);


#if SHLIB_COMPAT (libpthread, GLIBC_2_3_3, GLIBC_2_3_4)
/* The old interface have not been really exposed  */
strong_alias (__pthread_setaffinity_new, __pthread_setaffinity_old)
compat_symbol (libpthread, __pthread_setaffinity_old, pthread_setaffinity_np,
	       GLIBC_2_3_3);
#endif
