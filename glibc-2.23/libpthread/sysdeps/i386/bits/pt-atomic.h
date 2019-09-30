/* Atomic operations.  i386 version.
   Copyright (C) 2000 Free Software Foundation, Inc.
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

#ifndef _BITS_ATOMIC_H
#define _BITS_ATOMIC_H	1

typedef __volatile int __atomic_t;

static inline void
__atomic_inc (__atomic_t *__var)
{
  __asm__ __volatile ("lock; incl %0" : "=m" (*__var) : "m" (*__var));
}

static inline void
__atomic_dec (__atomic_t *__var)
{
  __asm__ __volatile ("lock; decl %0" : "=m" (*__var) : "m" (*__var));
}

static inline int
__atomic_dec_and_test (__atomic_t *__var)
{
  unsigned char __ret;

  __asm__ __volatile ("lock; decl %0; sete %1"
		      : "=m" (*__var), "=qm" (__ret) : "m" (*__var));
  return __ret != 0;
}

/* We assume that an __atomicptr_t is only used for pointers to
   word-aligned objects, and use the lowest bit for a simple lock.  */
typedef __volatile int * __atomicptr_t;

/* Actually we don't implement that yet, and assume that we run on
   something that has the i486 instruction set.  */
static inline int
__atomicptr_compare_and_swap (__atomicptr_t *__ptr, void *__oldval,
			      void * __newval)
{
  char __ret;
  int __dummy;

  __asm__ __volatile ("lock; cmpxchgl %3, %1; sete %0"
		      : "=q" (__ret), "=m" (*__ptr), "=a" (__dummy)
		      : "r" (__newval), "m" (*__ptr), "a" (__oldval));
  return __ret;
}

#endif
