/* pthread_once.  Generic version.
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
#include <bits/memory.h>

#include <pt-internal.h>

int
__pthread_once (pthread_once_t *once_control, void (*init_routine) (void))
{
  if (once_control->__run == 0)
    {
      __pthread_spin_lock (&once_control->__lock);

      if (once_control->__run == 0)
	{
	  init_routine ();
	  __memory_barrier ();
	  once_control->__run = 1;
	}

      __pthread_spin_unlock (&once_control->__lock);
    }

  return 0;
}
strong_alias (__pthread_once, pthread_once);
