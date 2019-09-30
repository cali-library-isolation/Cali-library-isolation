/* System-dependent timing definitions.  FreeBSD version.
   Copyright (C) 1996-1997, 1999, 2000, 2002 Free Software Foundation, Inc.
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

/*
 * Never include this file directly; use <time.h> instead.
 */

#ifndef __need_timeval
# ifndef _BITS_TIME_H
#  define _BITS_TIME_H	1

/* ISO/IEC 9899:1990 7.12.1: <time.h>
   The macro `CLOCKS_PER_SEC' is the number per second of the value
   returned by the `clock' function. */
/* CAE XSH, Issue 4, Version 2: <time.h>
   The value of CLOCKS_PER_SEC is required to be 1 million on all
   XSI-conformant systems. */
#  define CLOCKS_PER_SEC  ((clock_t) 1000000l)

#  if (!defined __STRICT_ANSI__ || defined __USE_POSIX) \
   && !defined __USE_XOPEN2K
/* Even though CLOCKS_PER_SEC has such a strange value CLK_TCK
   presents the real value for clock ticks per second for the system.  */
#   define CLK_TCK 128
#  endif

#  ifdef __USE_POSIX199309
/* Identifier for system-wide realtime clock.  */
#   define CLOCK_REALTIME		0
/* Monotonic system-wide clock.  */
#   define CLOCK_MONOTONIC		4
/* These are BSD specific clocks.  */
#   ifdef __USE_MISC
#    define CLOCK_VIRTUAL		1
#    define CLOCK_PROF			2
#    define CLOCK_UPTIME    5               /* FreeBSD-specific. */
#    define CLOCK_UPTIME_PRECISE    7       /* FreeBSD-specific. */
#    define CLOCK_UPTIME_FAST       8       /* FreeBSD-specific. */
#    define CLOCK_REALTIME_PRECISE  9       /* FreeBSD-specific. */
#    define CLOCK_REALTIME_FAST     10      /* FreeBSD-specific. */
#    define CLOCK_MONOTONIC_PRECISE 11      /* FreeBSD-specific. */
#    define CLOCK_MONOTONIC_FAST    12      /* FreeBSD-specific. */
#    define CLOCK_SECOND    13              /* FreeBSD-specific. */
#   endif
/* Thread-specific CPU-time clock.  */
#   define CLOCK_THREAD_CPUTIME_ID	14
/* High-resolution timer from the CPU.  */
#   define CLOCK_PROCESS_CPUTIME_ID	15
#   ifdef __USE_MISC
#     define CPUCLOCK_WHICH_PID 0
#     define CPUCLOCK_WHICH_TID 1
#   endif

/* Flag to indicate time is absolute.  */
#   define TIMER_RELTIME	0   /* relative timer */
#   define TIMER_ABSTIME	1   /* absolute timer */
#  endif


/* Getkerninfo clock information structure */
struct clockinfo
  {
    int hz;		/* clock frequency */
    int tick;		/* micro-seconds per hz tick */
    int spare;
    int stathz;		/* statistics clock frequency */
    int profhz;		/* profiling clock frequency */
  };

# endif	/* bits/time.h */
#endif

#ifdef __need_timeval
# undef __need_timeval
# ifndef _STRUCT_TIMEVAL
#  define _STRUCT_TIMEVAL	1
#  include <bits/types.h>

/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
struct timeval
  {
    __time_t tv_sec;		/* Seconds.  */
    __suseconds_t tv_usec;	/* Microseconds.  */
  };
# endif	/* struct timeval */
#endif	/* need timeval */
