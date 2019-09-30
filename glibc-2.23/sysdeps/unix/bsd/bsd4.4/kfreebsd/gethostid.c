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

#include <unistd.h>
#include <sys/sysctl.h>

/* Return the current machine's Internet number.  */
long int
gethostid (void)
{
  /* Fetch sysctl value of "kern.hostid".  */
  int request[2] = { CTL_KERN, KERN_HOSTID };
  int result;
  size_t result_len = sizeof (result);

  if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
    /* Dummy result.  */
    return 0;

  return result;
}
