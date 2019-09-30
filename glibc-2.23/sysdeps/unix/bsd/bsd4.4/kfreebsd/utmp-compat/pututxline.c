/* Copyright (C) 1998, 2002 Free Software Foundation, Inc.
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

#include <utmpx.h>
#include <utmp.h>
#include <stddef.h>
#include <string.h>

extern int __utmp_to_utmpx (const struct utmp *, struct utmpx *);
extern int __utmpx_to_utmp (const struct utmpx *, struct utmp *);

/* Static buffer to store the result.  */
static struct utmpx buffer;

struct utmpx *
pututxline (const struct utmpx *utmpx)
{
  struct utmp tmp;

  if (__utmpx_to_utmp (utmpx, &tmp) >= 0)
    {
      struct utmp *tmpres = __pututline (&tmp);

      if (tmpres != NULL)
	{
	  if (__utmp_to_utmpx (tmpres, &buffer) >= 0)
	    return &buffer;

	  /* Hmm.  We wrote a 'struct utmp' that we cannot convert back
	     to 'struct utmpx'.  Shouldn't happen that often.  */
	  return NULL;
	}
    }
  return NULL;
}
