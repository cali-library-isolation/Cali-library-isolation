/* Copyright (C) 1991,92,93,96,97,98,2000,2002 Free Software Foundation, Inc.
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

#include <stddef.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* Static buffer in `ttyname'.  */
libc_freeres_ptr (static char *ttyname_buf);

static const char dev[] = "/dev";

/* Return the pathname of the terminal FD is open on, or NULL on errors.
   The returned storage is good only until the next call to this function.  */
char *
ttyname (int fd)
{
  static size_t buflen;
  struct fiodgname_arg fgn;

  /* isatty check, tcgetattr is used because it sets the correct
     errno (EBADF resp. ENOTTY) on error.  */
  struct termios term;
  if (__builtin_expect (__tcgetattr (fd, &term) < 0, 0))
    return NULL;

  if (buflen == 0)
    {
      buflen = 4095;
      ttyname_buf = (char *) malloc (buflen + 1);
      if (ttyname_buf == NULL)
	{
	  buflen = 0;
	  return NULL;
	}
    }

  /* Prepare the result buffer.  */
  memcpy (ttyname_buf, dev, sizeof (dev) - 1);
  ttyname_buf[sizeof (dev) - 1] = '/';

  fgn.buf = ttyname_buf + sizeof (dev);
  fgn.len = buflen - sizeof (dev);

  if (__ioctl(fd, FIODGNAME, &fgn) == -1)
    {
      return NULL; 
    }

  return ttyname_buf;
}
