/* pthread_attr_setstack.  Generic version.
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
#include <assert.h>

#include <pt-internal.h>

int
pthread_attr_setstack (pthread_attr_t *attr,
		       void *stackaddr,
		       size_t stacksize)
{
  int err;
  size_t s;

  /* pthread_attr_setstack should always succeed, thus we set the size
     first as it is more discriminating.  */
  pthread_attr_getstacksize (attr, &s);

  err = pthread_attr_setstacksize (attr, stacksize);
  if (err)
    return err;

  err = pthread_attr_setstackaddr (attr, stackaddr);
  if (err)
    {
      int e = pthread_attr_setstacksize (attr, s);
      assert_perror (e);

      return err;
    }
      
  return 0;
}
