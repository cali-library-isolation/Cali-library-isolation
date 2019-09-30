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

/* Set the name of the current domain to NAME, which is LEN bytes long
   (excluding a possible trailing NUL byte).  This call is restricted to
   the super-user.  */

int
setdomainname (const char *name, size_t len)
{
  /* Set the "kern.domainname" sysctl value.  */
  int request[2] = { CTL_KERN, KERN_NISDOMAINNAME };

  if (__sysctl (request, 2, NULL, NULL, (void *) name, len) < 0)
    return -1;

  return 0;
}
