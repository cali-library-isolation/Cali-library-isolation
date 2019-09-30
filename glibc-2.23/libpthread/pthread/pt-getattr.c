/* Thread attributes retrieval.
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

#include <assert.h>
#include <errno.h>
#include <pthread.h>

#include <pt-internal.h>

/* Initialize thread attribute *ATTR with attributes corresponding to the
   already running thread THREAD.  It shall be called on an uninitialized ATTR
   and destroyed with pthread_attr_destroy when no longer needed.  */
int
pthread_getattr_np (pthread_t thread, pthread_attr_t *attr)
{
  struct __pthread *pthread;

  pthread = __pthread_getid(thread);
  if (pthread == NULL)
    return ESRCH;

  /* Some attributes (schedparam, inheritsched, contentionscope and schedpolicy)
     are not supported yet, so fill them with our default values.  */
  *attr = __pthread_default_attr;

  attr->__stackaddr = pthread->stackaddr +
		      ((pthread->guardsize + __vm_page_size-1)
		       / __vm_page_size * __vm_page_size);
  attr->__stacksize = pthread->stacksize;
  attr->__guardsize = pthread->guardsize;
  attr->__detachstate = (pthread->state == PTHREAD_DETACHED
		       ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);

  return 0;
}
