/* Return list of mounted filesystems.  FreeBSD version.
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

#include <sys/mount.h>
#include <stdlib.h>
#include <errno.h>

static int mntbufsize;
static struct statfs *mntbuf;

/* Return list of mounted filesystems.
   Inherently not multithread-safe.  */
int
__getmntinfo (struct statfs **mntbufp, int flags)
{
  for (;;)
    {
      int count = __getfsstat (NULL, 0, flags);
      int count2;

      if (count < 0)
	return 0;

      if (count == 0)
	{
	  __set_errno (0);
	  return 0;
	}

      if (count > mntbufsize)
	{
	  if (mntbuf != NULL)
	    free (mntbuf);
	  mntbufsize = 0;
	  mntbuf = (struct statfs *) malloc (count * sizeof (struct statfs));
	  if (mntbuf == NULL)
	    {
	      __set_errno (ENOMEM);
	      return 0;
	    }
	  mntbufsize = count;
	}

      count2 = __getfsstat (mntbuf, count * sizeof (struct statfs), flags);

      if (count2 < 0)
	return 0;

      if (count2 == 0)
	{
	  __set_errno (0);
	  return 0;
	}

      if (count2 <= count)
	{
	  *mntbufp = mntbuf;
	  return count2;
	}
    }
}

weak_alias (__getmntinfo, getmntinfo)
