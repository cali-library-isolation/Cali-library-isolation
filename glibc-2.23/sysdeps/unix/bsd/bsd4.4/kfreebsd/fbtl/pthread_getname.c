/* pthread_getname_np -- Get  thread name.
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
#include <sys/ptrace.h>

int
pthread_getname_np (pthread_t th, char *buf, size_t len)
{
  const struct pthread *pd = (const struct pthread *) th;
  struct ptrace_lwpinfo ti;
  int res;
  
  ti.pl_tdname[0] = 0;
  res = ptrace(PT_LWPINFO, pd->tid, (caddr_t)&ti, sizeof(struct ptrace_lwpinfo));

  if (res != 0)
      return errno;

  res = strlen(ti.pl_tdname);

  if (len < (res+1))
      return ERANGE;
      
  memcpy(buf, ti.pl_tdname, res+1);
  return 0;    
}
