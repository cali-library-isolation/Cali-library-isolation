/* Copyright (C) 2002-2013 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _LOWLEVELLOCK_H
#define _LOWLEVELLOCK_H	1

/* Values for 'private' parameter of locking macros.  Yes, the
   definition seems to be backwards.  But it is not.  
   They are the same as linux one's
 */  

#define FUTEX_PRIVATE_FLAG 128

#define LLL_PRIVATE	0
#define LLL_SHARED	FUTEX_PRIVATE_FLAG
#define KTID_TERMINATED 1
#include <stap-probe.h>

#ifndef __ASSEMBLER__

#include <time.h>
#include <sys/param.h>
#include <bits/pthreadtypes.h>
#include <kernel-features.h>
#include <tcb-offsets.h>
#include <atomic.h>
#include <lowlevelumtx.h>

/* Initializer for lock.  */
#define LLL_LOCK_INITIALIZER		(0)
#define LLL_LOCK_INITIALIZER_LOCKED	(1)
#define LLL_LOCK_INITIALIZER_WAITERS	(2)

#define lll_futex_wait(futex, val, private) \
  lll_futex_timed_wait(futex, val, NULL, private)

#define lll_futex_timed_wait(futex, val, timeout, private) 		\
  ({									\
      int __status;							\
      if ((private) == LLL_PRIVATE)					\
          __status = lll_umtx_int_wait_private (futex, val, timeout);	\
      else     								\
          __status = lll_umtx_int_wait_shared (futex, val, timeout);	\
    __status;								\
  })

#define lll_futex_wake(futex, nr, private) \
  ({									\
      int __status;							\
      if ((private) == LLL_PRIVATE)					\
          __status = lll_umtx_int_wake_private (futex, nr);		\
      else     								\
          __status = lll_umtx_int_wake_shared (futex, nr);		\
    __status;								\
  })

#define lll_trylock(lock)	\
  atomic_compare_and_exchange_val_acq(&(lock), 1, 0)

#define lll_cond_trylock(lock)	\
  atomic_compare_and_exchange_val_acq(&(lock), 2, 0)

extern void __lll_lock_wait_private (int *futex) attribute_hidden;
extern void __lll_lock_wait_shared  (int *futex) attribute_hidden;

#define __lll_lock_wait(futex, private)				\
  ((void) ({								\
	if ((private) == LLL_PRIVATE)	      				\
	  __lll_lock_wait_private (futex);				\
	else								\
	  __lll_lock_wait_shared  (futex);				\
  }))

#define __lll_lock(futex, private)					\
  ((void) ({								      \
    int *__futex = (futex);						      \
    if (__builtin_expect (atomic_compare_and_exchange_val_acq (__futex,       \
								1, 0), 0))    \
      {									      \
	  __lll_lock_wait (__futex, private);				      \
      }									      \
  }))

#define lll_lock(futex, private) __lll_lock (&(futex), private)


#define __lll_cond_lock(futex, private)					      \
  ((void) ({								      \
    int *__futex = (futex);						      \
    if (__builtin_expect (atomic_exchange_acq (__futex, 2), 0))		      \
      __lll_lock_wait (__futex, private);				      \
  }))
  
#define lll_cond_lock(futex, private) __lll_cond_lock (&(futex), private)


extern int __lll_timedlock_wait (int *futex, const struct timespec *,
				 int private) attribute_hidden;

#define __lll_timedlock(futex, abstime, private)			      \
  ({									      \
     int *__futex = (futex);						      \
     int __val = 0;							      \
									      \
     if (__builtin_expect (atomic_exchange_acq (__futex, 1), 0))	      \
       __val = __lll_timedlock_wait (__futex, abstime, private);	      \
     __val;								      \
  })
  
#define lll_timedlock(futex, abstime, private) \
  __lll_timedlock (&(futex), abstime, private)


#define __lll_unlock(futex, private) \
  (void)							\
    ({ int *__futex = (futex);					\
       int __oldval = atomic_exchange_rel (__futex, 0);		\
       if (__builtin_expect (__oldval > 1, 0))			\
	 lll_futex_wake (__futex, 1, private);			\
    })
    
#define lll_unlock(futex, private) __lll_unlock(&(futex), private)


#define lll_islocked(futex) \
  (futex != 0)


/* The kernel notifies a process which uses CLONE_CHILD_CLEARTID via futex
   wakeup when the clone terminates.  The memory location contains the
   thread ID while the clone is running and is reset to one (not zero as on linux)
   afterwards.	*/
#define lll_wait_tid(tid) \
  do {					\
    __typeof (tid) __tid;		\
    while ((__tid = (tid)) != KTID_TERMINATED)	\
      lll_umtx_long_wait_shared (&(tid), __tid, NULL);\
  } while (0)

extern int __lll_timedwait_tid (long *, const struct timespec *)
     attribute_hidden;

#define lll_timedwait_tid(tid, abstime) \
  ({							\
    int __res = 0;					\
    if ((tid) != KTID_TERMINATED)			\
      __res = __lll_timedwait_tid (&(tid), (abstime));	\
    __res;						\
  })
  
#endif  /* !__ASSEMBLER__ */

#endif	/* lowlevellock.h */
