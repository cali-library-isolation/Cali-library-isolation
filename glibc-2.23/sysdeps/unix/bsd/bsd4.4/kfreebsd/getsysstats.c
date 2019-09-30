/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.

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

#include <sys/sysctl.h>
#include <unistd.h>
#include <stddef.h>


int
__get_nprocs (void)
{
  /* Fetch sysctl value of "hw.ncpu".  */
  int request[2] = { CTL_HW, HW_NCPU };
  int result;
  size_t result_len = sizeof (result);

  if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
    /* Dummy result.  */
    return 1;

  return result;
}

weak_alias (__get_nprocs, get_nprocs)


int
__get_nprocs_conf (void)
{
  /* We don't know how to distinguish between configured and active CPUs.  */
  return __get_nprocs ();
}

weak_alias (__get_nprocs_conf, get_nprocs_conf)


long int
__get_phys_pages (void)
{
  /* Fetch sysctl value of "hw.physmem".  This is a little smaller than
     the real installed memory size, but so what.  */
  int request[2] = { CTL_HW, HW_PHYSMEM };
  unsigned long int result;
  size_t result_len = sizeof (result);

  if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
    return -1;

  return result / __getpagesize();
}

weak_alias (__get_phys_pages, get_phys_pages)


long int
__get_avphys_pages (void)
{
#if 1
  int result1;
  int result2;
  {
    /* Fetch sysctl value of "vm.stats.vm.v_inactive_count".  */
    size_t result_len = sizeof (result1);

    if (__sysctlbyname ("vm.stats.vm.v_inactive_count", &result1, &result_len,
			NULL, 0) < 0)
      return -1;
  }
  {
    /* Fetch sysctl value of "vm.stats.vm.v_free_count".  */
    size_t result_len = sizeof (result2);

    if (__sysctlbyname ("vm.stats.vm.v_free_count", &result2, &result_len,
			NULL, 0) < 0)
      return -1;
  }

  return result1 + result2;
#else
  /* This does not appear to work.  */
  /* Fetch v_inactive_count field of sysctl value of "vm.vmmeter".  */
  int request[2] = { CTL_VM, VM_METER };
  struct vmmeter result;
  size_t result_len = sizeof (result);

  if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
    return -1;

  return result.v_inactive_count;
#endif
}

weak_alias (__get_avphys_pages, get_avphys_pages)
