/* Call to terminate the current thread.  GNU/kFreeBSD i386 version
   Copyright (C) 2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>

/* This causes the current thread to exit, without affecting other
   threads in the process if there are any.  If there are no other
   threads left, then this has the effect of _exit (0).  */

static inline void __attribute__ ((noreturn, always_inline, unused))
__exit_thread (void)
{
  /* Doing this in a loop is mostly just to satisfy the compiler that the
     function really qualifies as noreturn.  It also means that in some
     pathological situation where the system call does not get made or does
     not work, the thread will simply spin rather than running off the end
     of the caller and doing unexpectedly strange things.  */
  while (1)
    {
      asm volatile (
	  "movl %%gs:0, %%edx\n\t"
	  "addl %0, %%edx\n\t"    /* should be KTID, but they are at the same place anyway */
	  "movl %%edx, 4(%%esp)\n\t"
	  "movl %1, %%eax\n\t"
	  "int $0x80\n\t"
	  /* It does return only for last thread of process */
	  "movl %2, %%eax\n\t"
	  "movl $0, 4(%%esp)\n\t"
	  "int $0x80\n\t"
	  :
	  : "i" (TID), "i" (SYS_ify(thr_exit)), "i" (SYS_ify(exit))
	  : "memory", "cc");
    }
}
