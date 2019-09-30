/* Copyright (C) 2000, 2002, 2005, 2008 Free Software Foundation, Inc.
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
 *	POSIX Threads Extension: ???			<pthreadtypes.h>
 */

#ifndef _PTHREADTYPES_H
#define _PTHREADTYPES_H	1

#include <features.h>

#include <bits/types.h>

__BEGIN_DECLS

#include <bits/pthread.h>
typedef __pthread_t pthread_t;

/* Possible values for the process shared attribute.  */
enum __pthread_process_shared
  {
    __PTHREAD_PROCESS_PRIVATE = 0,
    __PTHREAD_PROCESS_SHARED
  };

/* Possible values for the inheritsched attribute.  */
enum __pthread_inheritsched
  {
    __PTHREAD_EXPLICIT_SCHED = 0,
    __PTHREAD_INHERIT_SCHED
  };

/* Possible values for the `contentionscope' attribute.  */
enum __pthread_contentionscope
  {
    __PTHREAD_SCOPE_SYSTEM = 0,
    __PTHREAD_SCOPE_PROCESS
  };

/* Possible values for the `detachstate' attribute.  */
enum __pthread_detachstate
  {
    __PTHREAD_CREATE_JOINABLE = 0,
    __PTHREAD_CREATE_DETACHED
  };

#include <bits/thread-attr.h>
typedef struct __pthread_attr pthread_attr_t;

enum __pthread_mutex_protocol
  {
    __PTHREAD_PRIO_NONE= 0,
    __PTHREAD_PRIO_INHERIT,
    __PTHREAD_PRIO_PROTECT
  };

enum __pthread_mutex_type
  {
    __PTHREAD_MUTEX_TIMED,
    __PTHREAD_MUTEX_ERRORCHECK,
    __PTHREAD_MUTEX_RECURSIVE
  };

#include <bits/mutex-attr.h>
typedef struct __pthread_mutexattr pthread_mutexattr_t;

#include <bits/mutex.h>
typedef struct __pthread_mutex pthread_mutex_t;

#include <bits/condition-attr.h>
typedef struct __pthread_condattr pthread_condattr_t;

#include <bits/condition.h>
typedef struct __pthread_cond pthread_cond_t;

#ifdef __USE_XOPEN2K
# include <bits/spin-lock.h>
typedef __pthread_spinlock_t pthread_spinlock_t;
#endif /* XPG6.  */

#if defined __USE_UNIX98 || defined __USE_XOPEN2K

#include <bits/rwlock-attr.h>
typedef struct __pthread_rwlockattr pthread_rwlockattr_t;

#include <bits/rwlock.h>
typedef struct __pthread_rwlock pthread_rwlock_t;

#endif /* __USE_UNIX98 || __USE_XOPEN2K */

#ifdef __USE_XOPEN2K

#include <bits/barrier-attr.h>
typedef struct __pthread_barrierattr pthread_barrierattr_t;

#include <bits/barrier.h>
typedef struct __pthread_barrier pthread_barrier_t;

#endif /* __USE_XOPEN2K */

#include <bits/thread-specific.h>
typedef __pthread_key pthread_key_t;

#include <bits/once.h>
typedef struct __pthread_once pthread_once_t;

__END_DECLS

#endif /* pthreadtypes.h */
