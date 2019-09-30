/* Lock a mutex.  Generic version.
   Copyright (C) 2000, 2002, 2006 Free Software Foundation, Inc.
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
#include <assert.h>

#include <pt-internal.h>

/* Implemented in pt-mutex-timedlock.c.  */
extern int __pthread_mutex_timedlock_internal (struct __pthread_mutex *mutex,
					       const struct timespec *abstime);

/* Lock MUTEX, block if we can't get it.  */
int
__pthread_mutex_lock (struct __pthread_mutex *mutex)
{
  return __pthread_mutex_timedlock_internal (mutex, 0);
}

strong_alias (__pthread_mutex_lock, _pthread_mutex_lock);
strong_alias (__pthread_mutex_lock, pthread_mutex_lock);
