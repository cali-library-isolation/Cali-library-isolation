/* Copyright (C) 1996-1999,2000-2002 Free Software Foundation, Inc.
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

#include <string.h>
#include <utmp.h>

#include <utmp-private.h>

/* Test whether two entries match.  */
static int
__utmp_equal (const struct utmp *entry, const struct utmp *match)
{
  /* This implementation is consistent with the __utmp_to_utmpx function.  */
  return
    (!((entry->ut_line[0] == '\0' && entry->ut_name[0] == '\0'
	&& entry->ut_host[0] == '\0')
       || ((entry->ut_line[0] == '~' || entry->ut_line[0] == '{'
	    || entry->ut_line[0] == '|')
	   && entry->ut_line[1] == '\0'))
     &&
     !((match->ut_line[0] == '\0' && match->ut_name[0] == '\0'
	&& match->ut_host[0] == '\0')
       || ((match->ut_line[0] == '~' || match->ut_line[0] == '{'
	    || match->ut_line[0] == '|')
	   && match->ut_line[1] == '\0'))
     &&
#if _HAVE_UT_ID - 0
     (entry->ut_id[0] && match->ut_id[0]
      ? strncmp (entry->ut_id, match->ut_id, sizeof match->ut_id) == 0
      : strncmp (entry->ut_line, match->ut_line, sizeof match->ut_line) == 0)
#else
     strncmp (entry->ut_line, match->ut_line, sizeof match->ut_line) == 0
#endif
     );
}
