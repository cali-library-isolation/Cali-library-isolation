/* pthread_setspecific.  Generic version.
   Copyright (C) 2002 Free Software Foundation, Inc.
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
#include <hurd/ihash.h>

#include <pt-internal.h>

int
__pthread_setspecific (pthread_key_t key, const void *value)
{
  error_t err;
  struct __pthread *self = _pthread_self ();

  if (key < 0 || key >= __pthread_key_count
      || __pthread_key_destructors[key] == PTHREAD_KEY_INVALID)
    return EINVAL;

  if (! self->thread_specifics)
    {
      err = hurd_ihash_create (&self->thread_specifics, HURD_IHASH_NO_LOCP);
      if (err)
	return ENOMEM;
    }

  err = hurd_ihash_add (self->thread_specifics, key, (void *) value);
  if (err)
    return ENOMEM;
      
  return 0;
}
strong_alias (__pthread_setspecific, pthread_setspecific);
