/* High precision, low overhead timing functions.  i386 version.
   Copyright (C) 2003 Free Software Foundation, Inc.
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

#ifndef _i386_HP_TIMING_H
#define _i386_HP_TIMING_H	1

#define hp_timing_t hp_timing_t__
#include <sysdeps/generic/hp-timing.h>
#undef hp_timing_t

/* We don't use high-precision timers, but we might load an i686 libpthread
   which does.  */
#define HP_TIMING_PAD 1

/* i686 uses 64bit values for the times.  */
typedef unsigned long long int hp_timing_t;

#endif	/* hp-timing.h */
