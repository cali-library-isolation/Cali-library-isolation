/* Copyright (C) 2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Robert Millan <robertmh@gnu.org>

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
#include <fcntl.h>	/* open */
#include <unistd.h>	/* close */

static int __iopl_fd = -1;

int
iopl (int level)
  {
    switch (level)
      {
        case 3:
          if (__iopl_fd != -1)
            return 0;

          __iopl_fd = __open ("/dev/io", O_RDWR);
          if (__iopl_fd == -1)
            {
              if (errno == EACCES)
                __set_errno (EPERM);
              return -1;
            }
          return 0;

        case 0:
          if (__iopl_fd != -1)
            if (__close (__iopl_fd) == -1)
              return 1;
          return 0;

        default:
          __set_errno (EINVAL);
          return -1;
      }
  }
