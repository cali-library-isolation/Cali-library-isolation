/* Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <libc-internal.h>
#include <stddef.h>
#include <sys/sysctl.h>

struct clockinfo
{
  int hz;		/* Clock frequency.  */
  int tick;		/* Microseconds per hz tick, = 1000000 / hz.  */
  int tickadj;		/* Clock skew rate for adjtime().  */
  int stathz;		/* Statistics clock frequency.  */
  int profhz;		/* Profiling clock frequency.  */
};

int
__profile_frequency (void)
{
  /* Fetch the "kern.clockrate" sysctl value.  */
  int request[2] = { CTL_KERN, KERN_CLOCKRATE };
  struct clockinfo result;
  size_t result_len = sizeof (result);

  if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
    /* Dummy result.  */
    return 1;

  /* Yes, hz, not profhz.  On i386, the value is 100, not 1024.  */
  return result.hz;
}
libc_hidden_def (__profile_frequency)
