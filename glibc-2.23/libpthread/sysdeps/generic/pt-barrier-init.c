/* pthread_barrier_init.  Generic version.
   Copyright (C) 2002, 2005 Free Software Foundation, Inc.
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
#include <string.h>
#include <assert.h>

#include <pt-internal.h>

int
pthread_barrier_init (pthread_barrier_t *barrier,
		      const pthread_barrierattr_t *attr,
		      unsigned count)
{
  if (count == 0)
    return EINVAL;

  memset (barrier, 0, sizeof *barrier);

  barrier->__lock = PTHREAD_SPINLOCK_INITIALIZER;
  barrier->__pending = count;
  barrier->__count = count;

  if (! attr
      || memcmp (attr, &__pthread_default_barrierattr, sizeof (*attr) == 0))
    /* Use the default attributes.  */
    return 0;

  /* Non-default attributes.  */

  barrier->__attr = malloc (sizeof *attr);
  if (! barrier->__attr)
    return ENOMEM;

  *barrier->__attr = *attr;
  return 0;
}
