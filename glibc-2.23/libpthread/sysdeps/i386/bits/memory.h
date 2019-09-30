/* Memory barrier operations.  i386 version.
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

#ifndef _BITS_MEMORY_H
#define _BITS_MEMORY_H	1

/* Prevent read and write reordering across this function.  */
static inline void
__memory_barrier (void)
{
  int i;

  /* Any lock'ed instruction will do.  We just do a simple
     increment.  */
  __asm__ __volatile ("lock; incl %0" : "=m" (i) : "m" (i) : "memory");
}

/* Prevent read reordering across this function.  */
#define __memory_read_barrier __memory_barrier

/* Prevent write reordering across this function.  */
#define __memory_write_barrier __memory_barrier

#endif
