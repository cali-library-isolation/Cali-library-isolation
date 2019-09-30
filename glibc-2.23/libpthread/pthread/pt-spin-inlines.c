/* Copyright (C) 2000 Free Software Foundation, Inc.
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

/* <bits/spin-lock.h> declares some extern inline functions.  These
   functions are declared additionally here for use when inlining is
   not possible.  */

#define _FORCE_INLINES
#define __PT_SPIN_INLINE /* empty */

#include <pthread.h>

/* Weak aliases for the spin lock functions.  Note that
   pthread_spin_lock is left out deliberately.  We already provide an
   implementation for it in pt-spin.c.  */
weak_alias (__pthread_spin_destroy, pthread_spin_destroy);
weak_alias (__pthread_spin_init, pthread_spin_init);
weak_alias (__pthread_spin_trylock, pthread_spin_trylock);
weak_alias (__pthread_spin_unlock, pthread_spin_unlock);
