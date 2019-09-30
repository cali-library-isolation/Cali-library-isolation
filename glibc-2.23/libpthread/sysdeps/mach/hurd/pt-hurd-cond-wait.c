/* pthread_hurd_cond_wait.  Hurd-specific wait on a condition.
   Copyright (C) 2012 Free Software Foundation, Inc.
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
#include <hurd/signal.h>

#include <pt-internal.h>

#include <shlib-compat.h>

/* Implemented in pt-hurd-cond-timedwait.c.  */
extern int __pthread_hurd_cond_timedwait_internal (pthread_cond_t *cond,
						   pthread_mutex_t *mutex,
						   const struct timespec *abstime);

int
__pthread_hurd_cond_wait_np (pthread_cond_t *cond,
			     pthread_mutex_t *mutex)
{
  error_t err;

  err = __pthread_hurd_cond_timedwait_internal (cond, mutex, NULL);
  return (err == EINTR);
}
versioned_symbol (libpthread, __pthread_hurd_cond_wait_np, pthread_hurd_cond_wait_np, GLIBC_2_21);

#if SHLIB_COMPAT (libpthread, GLIBC_2_13, GLIBC_2_21)
int
__pthread_hurd_cond_wait_np_2_13 (pthread_cond_t *cond,
			     pthread_mutex_t *mutex)
{
  error_t err;

  err = __pthread_hurd_cond_timedwait_internal (cond, mutex, NULL);
  return (err == EINTR);
}
compat_symbol (libpthread, __pthread_hurd_cond_wait_np_2_13, pthread_hurd_cond_wait_np, GLIBC_2_13_DEBIAN_38);
#endif
