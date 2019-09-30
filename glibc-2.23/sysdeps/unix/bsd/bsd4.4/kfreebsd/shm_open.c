/* Copyright (C) 2014 Free Software Foundation, Inc.
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

#include <sys/mman.h>
#include <fcntl.h>
#include <sysdep.h>

extern int __syscall_shm_open (const char *name, int flag, mode_t mode);
libc_hidden_proto (__syscall_shm_open)

int
shm_open (const char *name, int flag, mode_t mode)
{
  /*
   * O_CLOEXEC never has any effect on shm_open(). However, old kernel
   * versions (prior to rev 261138 in HEAD) return EINVAL when this
   * (otherwise harmless) flag is used.
   */
  flag &= ~O_CLOEXEC;

  return INLINE_SYSCALL (shm_open, 3, name, flag, mode);
}
