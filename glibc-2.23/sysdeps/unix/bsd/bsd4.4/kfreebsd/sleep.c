/* Implementation of the POSIX sleep function using nanosleep.
   Copyright (C) 1996-1999, 2002 Free Software Foundation, Inc.
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

#include <time.h>

/* The default implementation of sleep() in sysdeps/posix/sleep.c, based on
   SIGALRM, does not mix well with threads.  Therefore we use the nanosleep()
   system call.  */
unsigned int
__sleep (unsigned int seconds)
{
  struct timespec ts = { tv_sec: seconds, tv_nsec: 0 };
  int ret;

  /* This is not necessary but some buggy programs depend on this.  */
  if (__builtin_expect (seconds == 0, 0))
    {
#ifdef CANCELLATION_P
      CANCELLATION_P (THREAD_SELF);
#endif
      return 0;
    }
                        
  ret = __nanosleep (&ts, &ts);

  if (ret == 0)
    return 0;

  /* Round remaining time.  */
  return (unsigned int) ts.tv_sec + (ts.tv_nsec >= 500000000L);
}

weak_alias (__sleep, sleep)
