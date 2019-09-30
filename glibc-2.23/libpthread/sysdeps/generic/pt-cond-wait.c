/* Wait on a condition.  Generic version.
   Copyright (C) 2000,02 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <pthread.h>

#include <pt-internal.h>

/* Implemented in pt-cond-timedwait.c.  */
extern int __pthread_cond_timedwait_internal (pthread_cond_t *cond,
					      pthread_mutex_t *mutex,
					      const struct timespec *abstime);


/* Block on condition variable COND.  MUTEX should be held by the
   calling thread.  On return, MUTEX will be held by the calling
   thread.  */
int
__pthread_cond_wait (pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  return __pthread_cond_timedwait_internal (cond, mutex, 0);
}

strong_alias (__pthread_cond_wait, pthread_cond_wait);
