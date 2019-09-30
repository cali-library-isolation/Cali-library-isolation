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
#include <limits.h>
#include <pthreadP.h>
#include <string.h>
#include <sysdep.h>
#include <sys/param.h>
#include <sys/types.h>
#include <shlib-compat.h>

#include "../cpuset-kern.h"

int
__pthread_getaffinity_new (pthread_t th, size_t cpusetsize, cpu_set_t *cpuset)
{
  const struct pthread *pd = (const struct pthread *) th;
  int64_t id = pd->tid;
  int res;
  
  if (cpusetsize > sizeof(cpu_set_t))
    {
      /* Clean the rest of the memory the kernel won't do.  */
      memset ((char *) cpuset + sizeof(cpu_set_t), '\0', cpusetsize - sizeof(cpu_set_t));

      cpusetsize = sizeof(cpu_set_t);
    }

  res = INLINE_SYSCALL (cpuset_getaffinity, 5, CPU_LEVEL_WHICH,
			CPU_WHICH_TID, id, cpusetsize, cpuset);

  if (res == 0)
      return 0;
      
  if (errno == ERANGE)
    {
      return EINVAL;
    }
  
  return errno;
}
strong_alias (__pthread_getaffinity_new, __pthread_getaffinity_np)
versioned_symbol (libpthread, __pthread_getaffinity_new,
		  pthread_getaffinity_np, GLIBC_2_3_4);


#if SHLIB_COMPAT (libpthread, GLIBC_2_3_3, GLIBC_2_3_4)
/* The old interface have not been really exposed  */
strong_alias (__pthread_getaffinity_new, __pthread_getaffinity_old)
compat_symbol (libpthread, __pthread_getaffinity_old, pthread_getaffinity_np,
	       GLIBC_2_3_3);
#endif
