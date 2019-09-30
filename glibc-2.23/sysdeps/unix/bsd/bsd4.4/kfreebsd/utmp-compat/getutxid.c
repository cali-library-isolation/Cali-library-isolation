/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.

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
#include <stddef.h>
#include <string.h>

extern struct utmpx *__getutxent (void);

struct utmpx *
getutxid (const struct utmpx *id)
{
  switch (id->ut_type)
    {
    case INIT_PROCESS:
    case LOGIN_PROCESS:
    case USER_PROCESS:
    case DEAD_PROCESS:
      for (;;)
	{
	  struct utmpx *next = __getutxent ();

	  if (next == NULL)
	    return NULL;

	  switch (next->ut_type)
	    {
	    case INIT_PROCESS:
	    case LOGIN_PROCESS:
	    case USER_PROCESS:
	    case DEAD_PROCESS:
	      if (strncmp (next->ut_id, id->ut_id, sizeof (id->ut_id)) == 0)
		return next;
	      break;
	    default:
	      break;
	    }
	}

    default:
      for (;;)
	{
	  struct utmpx *next = __getutxent ();

	  if (next == NULL)
	    return NULL;

	  if (next->ut_type == id->ut_type)
	    return next;
	}
    }
}
