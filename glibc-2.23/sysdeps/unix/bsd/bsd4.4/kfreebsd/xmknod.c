/* xmknod call using old-style Unix mknod and mkfifo system calls.
   Copyright (C) 1991, 1993, 1995-1997, 2002 Free Software Foundation, Inc.
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
#include <sys/types.h>
#include <sys/stat.h>

extern int __syscall_mknod (const char *, mode_t, dev_t);
extern int __mkfifo (const char *, mode_t);

/* Create a device file named PATH, with permission and special bits MODE
   and device number DEV (which can be constructed from major and minor
   device numbers with the `makedev' macro above).  */
int
__xmknod (int vers, const char *path, mode_t mode, dev_t *dev)
{
  if (vers != _MKNOD_VER)
    {
      __set_errno (EINVAL);
      return -1;
    }

  /* The FreeBSD mknod() system call cannot be used to create FIFOs; we
     must use the mkfifo() system call for this purpose.  */
  if (S_ISFIFO (mode))
    return __mkfifo (path, mode & ~S_IFMT);

  return __syscall_mknod (path, mode, *dev);
}
libc_hidden_def (__xmknod)

weak_alias (__xmknod, _xmknod)
