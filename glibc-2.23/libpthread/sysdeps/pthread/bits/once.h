/* Dynamic package initialization data structures.  Generic version.
   Copyright (C) 2002, 2009 Free Software Foundation, Inc.
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

#ifndef _BITS_ONCE_H
#define _BITS_ONCE_H	1

#include <bits/spin-lock.h>

struct __pthread_once
{
  int __run;
  __pthread_spinlock_t __lock;
};

#define __PTHREAD_ONCE_INIT \
	{ 0, __PTHREAD_SPIN_LOCK_INITIALIZER }

#endif /* bits/once.h */
