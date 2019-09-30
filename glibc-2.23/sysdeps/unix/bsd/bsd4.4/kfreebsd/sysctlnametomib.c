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

#include <sys/types.h>
#include <sys/sysctl.h>
#include <string.h>

int
__sysctlnametomib (const char *name, int *mibp, size_t *sizep)
{
  /* Convert the string NAME to a binary encoded request.  The kernel
     contains a routine for doing this, called "name2oid".  But the way
     to call it is a little bit strange.  */
  int name2oid_request[2] = { 0, 3 };
  int retval;
  
  *sizep *= sizeof (int);
  retval = __sysctl (name2oid_request, 2, mibp, sizep, (void *) name, strlen (name));
  *sizep /= sizeof (int);
  return retval;
}

weak_alias (__sysctlnametomib, sysctlnametomib)
