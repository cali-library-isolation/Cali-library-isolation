/* Get system load averages.  FreeBSD version.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <stdlib.h>
#include <sys/sysctl.h>

struct loadavg
  {
    unsigned int ldavg[3];
    long fscale;
  };

/* Put the 1 minute, 5 minute and 15 minute load averages into the first
   NELEM elements of LOADAVG.  Return the number written (never more than
   three, but may be less than NELEM), or -1 if an error occurred.  */
int
getloadavg (double loadavg[], int nelem)
{
  if (nelem > 3)
    nelem = 3;
  if (nelem > 0)
    {
      /* Fetch the "vm.loadavg" sysctl value.  */
      int request[2] = { CTL_VM, VM_LOADAVG };
      struct loadavg result;
      size_t result_len = sizeof (result);
      int i;

      if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
	return -1;

      for (i = 0; i < nelem; i++)
	loadavg[i] = (double) result.ldavg[i] / (double) result.fscale;
    }
  return nelem;
}
