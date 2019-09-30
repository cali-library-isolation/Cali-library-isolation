/* Pthread data structures.  Generic version.
   Copyright (C) 2002, 2008 Free Software Foundation, Inc.
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

#ifndef _BITS_PTHREAD_H
#define _BITS_PTHREAD_H	1

typedef int __pthread_t;

/* Return true if __T1 and __T2 both name the same thread.  Otherwise,
   false.  */
extern int
__pthread_equal (__pthread_t __t1, __pthread_t __t2);

#ifdef __USE_EXTERN_INLINES
__extern_inline int
__pthread_equal (__pthread_t __t1, __pthread_t __t2)
{
  return __t1 == __t2;
}
#endif

#endif /* bits/pthread.h */
