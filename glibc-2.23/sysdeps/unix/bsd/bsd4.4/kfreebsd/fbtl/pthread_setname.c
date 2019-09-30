/* pthread_setname_np -- Set  thread name
   Copyright (C) 2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <fcntl.h>
#include <pthreadP.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
pthread_setname_np (pthread_t th, const char *name)
{
  const struct pthread *pd = (const struct pthread *) th;

  return INLINE_SYSCALL(thr_set_name, 2, pd->tid, name);
}
