/* Extra initializers for shared libc.
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

#include <ldsodefs.h>
#include <hp-timing.h>

/* This file is used from the shared libc, to initialize anything which
   ld.so should have initialized but didn't - for instance, if ld.so
   is built for a machine without HP_TIMING but libc.so is built for
   a machine with HP_TIMING, clock_gettime will expect dl_cpuclock_offset
   to be initialized.  */

static void
dlinit_hptiming (void)
{
#if HP_SMALL_TIMING_AVAIL
  if (GL(dl_cpuclock_offset) == 0)
    HP_TIMING_NOW (GL(dl_cpuclock_offset));
#endif
}

static void dlinit_alt (void) __attribute__((constructor));
static void
dlinit_alt (void)
{
  dlinit_hptiming ();
}
