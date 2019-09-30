/* Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <sys/perm.h>
#include <sysarch.h>

int
i386_set_ioperm (unsigned long int from, unsigned long int num, int turn_on)
{
  struct i386_ioperm_args args;

  args.start = from;
  args.length = num;
  args.enable = turn_on;

  return __sysarch (I386_SET_IOPERM, &args);
}

strong_alias (i386_set_ioperm, ioperm)
