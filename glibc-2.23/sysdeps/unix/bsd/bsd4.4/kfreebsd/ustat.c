/* Return info on filesystem.
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

#include <sys/ustat.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
ustat (dev_t dev, struct ustat *ubuf)
{
  int mntcount;
  struct statfs *mntlist;

  /* Search for the device among the f_mntfromname fields of all mounted
     filesystems.  */
  mntcount = __getmntinfo (&mntlist, 0);
  if (mntcount == 0 && errno != 0)
    return -1;
  if (mntcount > 0)
    {
      int i;

      for (i = 0; i < mntcount; i++)
	{
	  struct statfs *mnt = &mntlist[i];
	  struct stat statbuf;

	  if ((__strnlen (mnt->f_mntfromname, sizeof (mnt->f_mntfromname))
	       < sizeof (mnt->f_mntfromname))
	      && __xstat (_STAT_VER, mnt->f_mntfromname, &statbuf) >= 0
	      && S_ISBLK (statbuf.st_mode)
	      && statbuf.st_rdev == dev)
	    {
	      /* Found the device.  Now produce the result.  */
	      memset (ubuf, '\0', sizeof (struct ustat));
	      ubuf->f_tfree = mnt->f_bfree;
	      ubuf->f_tinode = mnt->f_ffree;
	      return 0;
	    }
	}
    }

  /* DEV is not among the mounted devices.  */
  __set_errno (EINVAL);
  return -1;
}
