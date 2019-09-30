/* Copyright (C) 1991,92,93,95,96,97,98,2004 Free Software Foundation, Inc.
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

#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static const char dev[] = "/dev";

/* Store at most BUFLEN character of the pathname of the terminal FD is
   open on in BUF.  Return 0 on success,  otherwise an error number.  */
int
__ttyname_r (int fd, char *buf, size_t buflen)
{
  int ret;
  struct fiodgname_arg fgn;

  /* Test for the absolute minimal size.  This makes life easier inside
     the loop.  */
  if (!buf)
    {
      __set_errno (EINVAL);
      return EINVAL;
    }

  if (buflen < (int) (sizeof (dev) + 1))
    {
      __set_errno (ERANGE);
      return ERANGE;
    }

  /* isatty check, tcgetattr is used because it sets the correct
     errno (EBADF resp. ENOTTY) on error.  */
  struct termios term;
  if (__builtin_expect (__tcgetattr (fd, &term) < 0, 0))
    return errno;

  /* Prepare the result buffer.  */
  memcpy (buf, dev, sizeof (dev) - 1);
  buf[sizeof (dev) - 1] = '/';

  fgn.buf = buf + sizeof (dev);
  fgn.len = buflen - sizeof (dev);

  ret = __ioctl(fd, FIODGNAME, &fgn);
  if (ret == -1)
    {
      __set_errno (EBADF);
      return EBADF;
    }

  return 0;
}

weak_alias (__ttyname_r, ttyname_r)
