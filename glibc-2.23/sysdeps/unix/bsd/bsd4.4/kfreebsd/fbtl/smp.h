/* Determine whether the host has multiple processors.  FreeBSD version.
   Copyright (C) 1996, 2002 Free Software Foundation, Inc.
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

#include <sys/sysctl.h>

/* Test whether the machine has more than one processor.  */
static inline int
is_smp_system (void)
{
  /* Fetch sysctl value of "hw.ncpu".  */
  int request[] = { CTL_HW, HW_NCPU };
  int result;
  size_t result_len = sizeof (result);

  if (__sysctl (request, 2, &result, &result_len, NULL, 0) < 0)
    /* Dummy result.  */
    return 0;

  return (result > 1);
}
