/* pthread_mutexattr_setprotocol.  Generic version.
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
#include <pt-internal.h>

int
pthread_mutexattr_setprotocol (pthread_mutexattr_t *attr,
			       int protocol)
{
  if (protocol == __pthread_default_mutexattr.__protocol)
    {
      attr->__protocol = protocol;
      return 0;
    }

  switch (protocol)
    {
    case PTHREAD_PRIO_NONE:
    case PTHREAD_PRIO_INHERIT:
    case PTHREAD_PRIO_PROTECT:
      return ENOTSUP;
    default:
      return EINVAL;
    }
}
