/* Copyright (C) 2003, 2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2003.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <errno.h>
#include <time.h>
#include <sysdep.h>
#include <kernel-features.h>
#include "kernel-posix-timers.h"


int
timer_getoverrun (timer_t timerid)
{
      struct timer *kt = __kfreebsd_timer_id2ptr (timerid);
      if (! kt)
	return -1;

      /* Get the information from the kernel.  */
      return INLINE_SYSCALL (ktimer_getoverrun, 1, kt->ktimerid);
}
