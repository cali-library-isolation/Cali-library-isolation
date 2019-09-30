/* Determine directory for shm/sem files.  GNU/kFreeBSD version.
   Copyright (C) 2000-2015 Free Software Foundation, Inc.
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

#include "shm-directory.h"

#include <errno.h>
#include <mntent.h>
#include <paths.h>
#include <stdio.h>
#include <string.h>
#include <sys/statfs.h>
#include <libc-lock.h>

/* Mount point of the shared memory filesystem.  */
static struct
{
  char *dir;
  size_t dirlen;
} mountpoint;

/* This is the default directory.  */
static const char defaultdir[] = "/dev/shm/";

/* This is the alternate debian directory.  */
static const char alternatedir[] = "/run/shm";

/* Protect the `mountpoint' variable above.  */
__libc_once_define (static, once);


/* Determine where the shmfs is mounted (if at all).  */
static void
where_is_shmfs (void)
{
  struct statfs f;

  /* The canonical place is /dev/shm.  This is at least what the
     documentation tells everybody to do.  */
  if (__statfs (defaultdir, &f) == 0)
    {
      /* It is in the normal place.  */
      mountpoint.dir = (char *) defaultdir;
      mountpoint.dirlen = sizeof (defaultdir) - 1;

      return;
    }

  /* The alternate place is /run/shm.  */
  if (__statfs (alternatedir, &f) == 0)
    {
      /* It is in the normal place.  */
      mountpoint.dir = (char *) alternatedir;
      mountpoint.dirlen = sizeof (alternatedir) - 1;

      return;
    }
}


const char *
__shm_directory (size_t *len)
{
  /* Determine where the shmfs is mounted.  */
  __libc_once (once, where_is_shmfs);

  /* If we don't know the mount points there is nothing we can do.  Ever.  */
  if (__glibc_unlikely (mountpoint.dir == NULL))
    {
      __set_errno (ENOSYS);
      return NULL;
    }

  *len = mountpoint.dirlen;
  return mountpoint.dir;
}
#if IS_IN (libpthread)
hidden_def (__shm_directory)
#endif


/* Make sure the table is freed if we want to free everything before
   exiting.  */
libc_freeres_fn (freeit)
{
  if (mountpoint.dir != defaultdir)
    free (mountpoint.dir);
}
