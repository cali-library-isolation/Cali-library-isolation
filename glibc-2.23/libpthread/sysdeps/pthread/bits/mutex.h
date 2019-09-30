/* Mutex type.  Generic version.

   Copyright (C) 2000, 2002, 2005, 2006, 2007, 2008, 2009
     Free Software Foundation, Inc.

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

#ifndef _BITS_MUTEX_H

#ifndef __need_pthread_mutex
# define _BITS_MUTEX_H	1
#endif

#ifndef __pthread_mutex_defined
# if defined __need_pthread_mutex || defined _BITS_MUTEX_H
#  undef __need_pthread_mutex
#  define __pthread_mutex_defined

#  include <bits/spin-lock.h>
#  include <bits/mutex-attr.h>

/* User visible part of a mutex.  */
struct __pthread_mutex
  {
    __pthread_spinlock_t __held;
    __pthread_spinlock_t __lock;
    /* In cthreads, mutex_init does not initialized thre third
       pointer, as such, we cannot rely on its value for anything.  */
    char *__cthreadscompat1;
    struct __pthread *__queue;
    struct __pthread_mutexattr *__attr;
    void *__data;
    /*  Up to this point, we are completely compatible with cthreads
	and what libc expects.  */
    void *__owner;
    unsigned __locks;
    /* If NULL then the default attributes apply.  */
  };

/* Initializer for a mutex.  N.B.  this also happens to be compatible
   with the cthread mutex initializer.  */
#  define __PTHREAD_MUTEX_INITIALIZER \
    { __PTHREAD_SPIN_LOCK_INITIALIZER, __PTHREAD_SPIN_LOCK_INITIALIZER, 0, 0, 0, 0, 0, 0 }

#  define __PTHREAD_ERRORCHECK_MUTEXATTR ((struct __pthread_mutexattr *) ((unsigned long) __PTHREAD_MUTEX_ERRORCHECK + 1))

#  define __PTHREAD_ERRORCHECK_MUTEX_INITIALIZER \
    { __PTHREAD_SPIN_LOCK_INITIALIZER, __PTHREAD_SPIN_LOCK_INITIALIZER, 0, 0,	\
	__PTHREAD_ERRORCHECK_MUTEXATTR, 0, 0, 0 }

#  define __PTHREAD_RECURSIVE_MUTEXATTR ((struct __pthread_mutexattr *) ((unsigned long) __PTHREAD_MUTEX_RECURSIVE + 1))

#  define __PTHREAD_RECURSIVE_MUTEX_INITIALIZER \
    { __PTHREAD_SPIN_LOCK_INITIALIZER, __PTHREAD_SPIN_LOCK_INITIALIZER, 0, 0,	\
	__PTHREAD_RECURSIVE_MUTEXATTR, 0, 0, 0 }

# endif
#endif /* Not __pthread_mutex_defined.  */

#endif /* bits/mutex.h */
