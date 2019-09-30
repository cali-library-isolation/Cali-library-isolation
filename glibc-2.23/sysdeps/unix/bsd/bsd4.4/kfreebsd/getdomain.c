/* Copyright (C) 2009 Free Software Foundation, Inc.
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

#include <unistd.h>
#include <sys/sysctl.h>
#include <errno.h>

/* Put the name of the current domain in no more than LEN bytes of NAME.
   The result is null-terminated if LEN is large enough for the full
   name and the terminator.  */

int
getdomainname (char *name, size_t len)
{
  /* Fetch the "kern.domainname" sysctl value.  */
  int request[2] = { CTL_KERN, KERN_NISDOMAINNAME };
  size_t result_len = len;

  if (__sysctl (request, 2, name, &result_len, NULL, 0) < 0)
    {
      if (errno == ENOMEM)
	__set_errno (ENAMETOOLONG);
      return -1;
    }

  if (result_len >= len)
    {
      __set_errno (ENAMETOOLONG);
      return -1;
    }

  name[result_len] = '\0';
  return 0;
}
libc_hidden_def (getdomainname)
