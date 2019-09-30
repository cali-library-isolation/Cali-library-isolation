/* Copyright (C) 1998-2013 Free Software Foundation, Inc.
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

#include <errno.h>
#include <paths.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <termios.h>
#include <unistd.h>


/* Directory where we can find the slave pty nodes.  */
#define _PATH_DEVPTS "/dev/pts/"

/* Static buffer for `ptsname'.  */
static char buffer[sizeof (_PATH_DEVPTS) + 20];


/* Return the pathname of the pseudo terminal slave associated with
   the master FD is open on, or NULL on errors.
   The returned storage is good until the next call to this function.  */
char *
ptsname (int fd)
{
  return __ptsname_r (fd, buffer, sizeof (buffer)) != 0 ? NULL : buffer;
}


int
__isptymaster(int fd)
{
  if (0 == __ioctl(fd, TIOCPTMASTER))
    return 0;

  if (errno != EBADF)
    __set_errno (EINVAL);

  return -1;
}


int
__ptsname_internal (int fd, char *buf, size_t buflen, struct stat64 *stp)
{
  struct fiodgname_arg fiodgname;
  char *p;
  if (buf == NULL)
    {
      __set_errno (EINVAL);
      return EINVAL;
    }
    
  /* Check if FD really is a master pseudo terminal.  */
  if (0 != __isptymaster(fd))
  {
      __set_errno (ENOTTY);
      return ENOTTY;
  }

  if (buflen < sizeof (_PATH_DEV) + 5) /* "/dev/" + "pts/"   */
  {
      __set_errno (ERANGE);
      return ERANGE;
  }

  /* Construct the slave's pathname.  */
  /* instead of strlen(_PATH_DEV) we use (sizeof (_PATH_DEV) - 1)  */
  p = __mempcpy (buf, _PATH_DEV, sizeof (_PATH_DEV) - 1);
  buflen -= (sizeof (_PATH_DEV) - 1);

  fiodgname.buf = p;
  fiodgname.len = buflen;

  if (0 != __ioctl(fd, FIODGNAME, &fiodgname))
    return errno;

  if (__xstat64 (_STAT_VER, buf, stp) < 0)
    return errno;

  return 0;
}


/* Store at most BUFLEN characters of the pathname of the slave pseudo
   terminal associated with the master FD is open on in BUF.
   Return 0 on success, otherwise an error number.  */
int
__ptsname_r (int fd, char *buf, size_t buflen)
{
  struct stat64 st;
  return __ptsname_internal (fd, buf, buflen, &st);
}
weak_alias (__ptsname_r, ptsname_r)
