/* raise.c - Generic raise implementation.
   Copyright (C) 2008 Free Software Foundation, Inc.
   Written by Neal H. Walfield <neal@gnu.org>.

   This file is part of the GNU Hurd.

   The GNU Hurd is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 3 of
   the License, or (at your option) any later version.

   The GNU Hurd is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this program.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <pthread.h>

#pragma weak pthread_kill
#pragma weak pthread_self
int
raise (int signo)
{
  /* According to POSIX, if we implement threads (and we do), then
     "the effect of the raise() function shall be equivalent to
     calling: pthread_kill(pthread_self(), sig);"  */

  int err;
  if (pthread_kill)
    err = pthread_kill (pthread_self (), signo);
  else
    err = __kill (__getpid (), signo);
  if (err)
    {
      errno = err;
      return -1;
    }

  return 0;
}

