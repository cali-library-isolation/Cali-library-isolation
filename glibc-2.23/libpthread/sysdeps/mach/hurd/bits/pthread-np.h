/* Non-portable functions. Hurd on Mach version.
   Copyright (C) 2008 Free Software Foundation, Inc.
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

/*
 * Never include this file directly; use <pthread.h> or <cthreads.h> instead.
 */

#ifndef _BITS_PTHREAD_NP_H
#define _BITS_PTHREAD_NP_H	1

/* Same as pthread_cond_wait, but for Hurd-specific cancellation.
   See hurd_thread_cancel.  */
extern int pthread_hurd_cond_wait_np (pthread_cond_t *__restrict __cond,
				      pthread_mutex_t *__restrict __mutex);

/* Same as pthread_cond_timedwait, but for Hurd-specific cancellation.
   See hurd_thread_cancel.  */
extern int pthread_hurd_cond_timedwait_np (pthread_cond_t *__restrict __cond,
					   pthread_mutex_t *__restrict __mutex,
					   const struct timespec *__abstime);

#endif /* bits/pthread-np.h */
