/* Get the value of a semaphore.  Generic version.
   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
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

#include <semaphore.h>
#include <pt-internal.h>

int
__sem_getvalue (sem_t *restrict sem, int *restrict value)
{
  __pthread_spin_lock (&sem->__lock);
  *value = sem->__value;
  __pthread_spin_unlock (&sem->__lock);

  return 0;
}

strong_alias (__sem_getvalue, sem_getvalue);
