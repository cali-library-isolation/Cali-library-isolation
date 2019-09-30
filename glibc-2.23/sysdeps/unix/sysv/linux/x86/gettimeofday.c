/* gettimeofday - get the time.  Linux/x86 version.
   Copyright (C) 2015-2016 Free Software Foundation, Inc.
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

#include <sys/time.h>

#ifdef SHARED

# include <dl-vdso.h>
# include <errno.h>

static int
__gettimeofday_syscall (struct timeval *tv, struct timezone *tz)
{
  return INLINE_SYSCALL (gettimeofday, 2, tv, tz);
}

void *gettimeofday_ifunc (void) __asm__ ("__gettimeofday");

void *
gettimeofday_ifunc (void)
{
  PREPARE_VERSION_KNOWN (linux26, LINUX_2_6);

  /* If the vDSO is not available we fall back to syscall.  */
  return (_dl_vdso_vsym ("__vdso_gettimeofday", &linux26)
	  ?: (void*) (&__gettimeofday_syscall));
}
asm (".type __gettimeofday, %gnu_indirect_function");

libc_ifunc_hidden_def(__gettimeofday)

#else

# include <sysdep.h>
# include <errno.h>

int
__gettimeofday (struct timeval *tv, struct timezone *tz)
{
  return INLINE_SYSCALL (gettimeofday, 2, tv, tz);
}
libc_hidden_def (__gettimeofday)

#endif
weak_alias (__gettimeofday, gettimeofday)
libc_hidden_weak (gettimeofday)
