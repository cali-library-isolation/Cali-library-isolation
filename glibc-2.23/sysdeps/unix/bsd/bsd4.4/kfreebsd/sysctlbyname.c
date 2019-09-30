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
#include <string.h>

/* Read or write system parameters.  */
int
__sysctlbyname (const char *name, void *oldval, size_t *oldlenp, const void *newval, size_t newlen)
{
  int request[CTL_MAXNAME+2];
  size_t requestlen = CTL_MAXNAME+2;

  if (__sysctlnametomib(name, request, &requestlen) < 0)
    return -1;

  /* Now call sysctl using the binary encoded request.  */
  return __sysctl (request, requestlen,
		   oldval, oldlenp, (void *)newval, newlen);
}

weak_alias (__sysctlbyname, sysctlbyname)
