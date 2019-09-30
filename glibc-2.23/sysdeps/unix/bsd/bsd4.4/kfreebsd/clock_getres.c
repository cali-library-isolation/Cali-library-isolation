/* Copyright (C) 2006 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>

int __syscall_clock_getres(clockid_t clock_id, struct timespec *tp);
libc_hidden_proto (__syscall_clock_getres)

# define SYSDEP_GETRES \
	case CLOCK_REALTIME:		\
	case CLOCK_VIRTUAL:		\
	case CLOCK_PROF:		\
	case CLOCK_MONOTONIC:		\
	case CLOCK_UPTIME:		\
	case CLOCK_UPTIME_PRECISE:	\
	case CLOCK_UPTIME_FAST:		\
	case CLOCK_REALTIME_PRECISE:	\
	case CLOCK_REALTIME_FAST:	\
	case CLOCK_MONOTONIC_PRECISE:	\
	case CLOCK_MONOTONIC_FAST:	\
	case CLOCK_SECOND:		\
    retval = INLINE_SYSCALL (clock_getres, 2, clock_id, res);		      \
    break

/* We handled the REALTIME clock here.  */
# define HANDLED_REALTIME	1

#include <sysdeps/posix/clock_getres.c>
