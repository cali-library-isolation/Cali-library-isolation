/* Compatibility routines for cthreads.
   Copyright (C) 2000, 2002, 2008 Free Software Foundation, Inc.
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

#include <assert.h>
#include <pthread.h>

typedef void *cthread_t;
typedef void *(*cthread_fn_t) (void *arg);
typedef int cthread_key_t;

#define	CTHREAD_KEY_INVALID (cthread_key_t) -1

void
cthread_detach (cthread_t thread)
{
  int err;

  err = pthread_detach ((pthread_t) thread);
  assert_perror (err);
}

cthread_t
cthread_fork (cthread_fn_t func, void *arg)
{
  pthread_t thread;
  int err;

  err = pthread_create (&thread, NULL, func, arg);
  assert_perror (err);

  return (cthread_t) thread;
}

int
cthread_keycreate (cthread_key_t *key)
{
  error_t err;

  err = pthread_key_create (key, 0);
  if (err)
    {
      errno = err;
      *key = CTHREAD_KEY_INVALID;
      err = -1;
    }

  return err;
}

int
cthread_getspecific (cthread_key_t key, void **val)
{
  *val = pthread_getspecific (key);
  return 0;
}

void *
__libc_getspecific (cthread_key_t key)
{
  return pthread_getspecific (key);
}

int
cthread_setspecific (cthread_key_t key, void *val)
{
  error_t err;

  err = pthread_setspecific (key, (const void *) val);
  if (err)
    {
      errno = err;
      err = -1;
    }

  return err;
}

void
__mutex_lock_solid (void *lock)
{
  __pthread_mutex_lock (lock);
}

void
__mutex_unlock_solid (void *lock)
{
  if (__pthread_spin_trylock (lock) != 0)
    /* Somebody already got the lock, that one will manage waking up others */
    return;
  __pthread_mutex_unlock (lock);
}
