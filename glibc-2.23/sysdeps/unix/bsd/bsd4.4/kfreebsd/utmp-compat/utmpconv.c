/* Convert between 'struct utmp' and 'struct utmx'.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

/* We reuse the 'struct utmp' file format also for 'struct utmpx' records.
   The mapping from 'struct utmp' to 'struct utmpx' is injective; the
   reverse mapping succeeds only when common conventions are respected.  */

#include <utmp.h>
#include <utmpx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Compare two 'struct utmpx' records.  */
int
__utmpx_equal (const struct utmpx *u1, const struct utmpx *u2)
{
  return (u1->ut_type == u2->ut_type)
	 && (u1->ut_pid == u2->ut_pid)
	 && (strncmp (u1->ut_line, u2->ut_line, __UT_LINESIZE) == 0)
	 && (strncmp (u1->ut_id, u2->ut_id, sizeof (u1->ut_id)) == 0)
	 && (strncmp (u1->ut_user, u2->ut_user, __UT_NAMESIZE) == 0)
	 && (strncmp (u1->ut_host, u2->ut_host, __UT_HOSTSIZE) == 0)
	 && (u1->ut_exit.e_termination == u2->ut_exit.e_termination)
	 && (u1->ut_exit.e_exit == u2->ut_exit.e_exit)
	 && (u1->ut_session == u2->ut_session)
	 && (u1->ut_tv.tv_sec == u2->ut_tv.tv_sec)
	 && (u1->ut_tv.tv_usec == u2->ut_tv.tv_usec)
	 && (memcmp (u1->ut_addr_v6, u2->ut_addr_v6, sizeof (u1->ut_addr_v6))
	     == 0);
}

int
__utmp_to_utmpx (const struct utmp *u, struct utmpx *ux)
{
  memset (ux, '\0', sizeof (struct utmpx));

  if (u->ut_line[0] == '\0' && u->ut_name[0] == '\0' && u->ut_host[0] == '\0')
    {
      ux->ut_type = EMPTY;
    }
  else
    {
      if (u->ut_line[0] == '~' && u->ut_line[1] == '\0')
	{
	  if (strncmp (u->ut_name, "runlevel", UT_NAMESIZE) == 0)
	    ux->ut_type = RUN_LVL;
	  else if (strncmp (u->ut_name, "reboot", UT_NAMESIZE) == 0)
	    ux->ut_type = BOOT_TIME;
	  else if (strncmp (u->ut_name, "acct", UT_NAMESIZE) == 0)
	    ux->ut_type = ACCOUNTING;
	  else
	    return -1;
	}
      else if (u->ut_line[0] == '{' && u->ut_line[1] == '\0')
	{
	  if (strncmp (u->ut_name, "date", UT_NAMESIZE) == 0)
	    ux->ut_type = NEW_TIME;
	  else
	    return -1;
	}
      else if (u->ut_line[0] == '|' && u->ut_line[1] == '\0')
	{
	  if (strncmp (u->ut_name, "date", UT_NAMESIZE) == 0)
	    ux->ut_type = OLD_TIME;
	  else
	    return -1;
	}
      else
	{
	  if (strncmp (u->ut_name, "INIT", UT_NAMESIZE) == 0)
	    ux->ut_type = INIT_PROCESS;
	  else if (strncmp (u->ut_name, "LOGIN", UT_NAMESIZE) == 0)
	    ux->ut_type = LOGIN_PROCESS;
	  else if (strncmp (u->ut_name, "", UT_NAMESIZE) == 0)
	    ux->ut_type = DEAD_PROCESS;
	  else
	    ux->ut_type = USER_PROCESS;

	  if (ux->ut_type == LOGIN_PROCESS || ux->ut_type == USER_PROCESS)
	    strncpy (ux->ut_user, u->ut_name, UT_NAMESIZE);

	  if (strncmp (u->ut_line, "tty", 3) == 0)
	    {
	      strncpy (ux->ut_line, u->ut_line, UT_LINESIZE);
	      strncpy (ux->ut_id, u->ut_line + 3, sizeof (ux->ut_id));
	    }
	  else
	    {
	      strncpy (ux->ut_id, u->ut_line, sizeof (ux->ut_id));
	    }
	}

      strncpy (ux->ut_host, u->ut_host, UT_HOSTSIZE);

      /* Hack: Recover the ut_pid from the hidden place after the host.  */
      if (__strnlen (u->ut_host, UT_HOSTSIZE) < UT_HOSTSIZE - 2)
	{
	  const char *hidden = u->ut_host + strlen (u->ut_host) + 1;

	  if (hidden[0] != '\0')
	    {
	      size_t n = UT_HOSTSIZE - strlen (u->ut_host) - 1;
	      char buf[UT_HOSTSIZE];
	      unsigned long pid;
	      char *endp;

	      strncpy (buf, hidden, n);
	      buf[n] = '\0';

	      pid = strtoul (buf, &endp, 10);
	      if (endp != buf && *endp == '\0')
		ux->ut_pid = pid;
	    }
	}

      ux->ut_tv.tv_sec = u->ut_time;
      ux->ut_tv.tv_usec = 0;
    }

  return 0;
}

int
__utmpx_to_utmp (const struct utmpx *ux, struct utmp *u)
{
  char buf[10+1];

  switch (ux->ut_type)
    {
    case EMPTY:
      strncpy (u->ut_line, "", UT_LINESIZE);
      strncpy (u->ut_name, "", UT_NAMESIZE);
      strncpy (u->ut_host, "", UT_HOSTSIZE);
      break;

    case RUN_LVL:
      strncpy (u->ut_line, "~", UT_LINESIZE);
      strncpy (u->ut_name, "runlevel", UT_NAMESIZE);
      strncpy (u->ut_host, ux->ut_host, UT_HOSTSIZE);
      break;

    case BOOT_TIME:
      strncpy (u->ut_line, "~", UT_LINESIZE);
      strncpy (u->ut_name, "reboot", UT_NAMESIZE);
      strncpy (u->ut_host, ux->ut_host, UT_HOSTSIZE);
      break;

    case NEW_TIME:
      strncpy (u->ut_line, "{", UT_LINESIZE);
      strncpy (u->ut_name, "date", UT_NAMESIZE);
      strncpy (u->ut_host, ux->ut_host, UT_HOSTSIZE);
      break;

    case OLD_TIME:
      strncpy (u->ut_line, "|", UT_LINESIZE);
      strncpy (u->ut_name, "date", UT_NAMESIZE);
      strncpy (u->ut_host, ux->ut_host, UT_HOSTSIZE);
      break;

    case INIT_PROCESS:
    case LOGIN_PROCESS:
    case USER_PROCESS:
    case DEAD_PROCESS:
      if (ux->ut_line[0] != '\0')
	strncpy (u->ut_line, ux->ut_line, UT_LINESIZE);
      else
	strncpy (u->ut_line, ux->ut_id, sizeof (ux->ut_id));
      switch (ux->ut_type)
	{
	case INIT_PROCESS:
	  strncpy (u->ut_name, "INIT", UT_NAMESIZE);
	  break;
	case LOGIN_PROCESS:
	  strncpy (u->ut_name, "LOGIN", UT_NAMESIZE);
	  break;
	case USER_PROCESS:
	  strncpy (u->ut_name, ux->ut_user, UT_NAMESIZE);
	  break;
	case DEAD_PROCESS:
	  strncpy (u->ut_name, "", UT_NAMESIZE);
	  break;
	}
      strncpy (u->ut_host, ux->ut_host, UT_HOSTSIZE);
      break;

    case ACCOUNTING:
      strncpy (u->ut_line, "~", UT_LINESIZE);
      strncpy (u->ut_name, "acct", UT_NAMESIZE);
      strncpy (u->ut_host, ux->ut_host, UT_HOSTSIZE);
      break;

    default:
      return -1;
    }

  u->ut_time = ux->ut_tv.tv_sec;

  /* Hack: Put the ut_pid at a hidden place where there is likely room.  */
  if (ux->ut_pid != 0)
    {
      size_t room = UT_HOSTSIZE - strlen (u->ut_host) - 1;

      sprintf (buf, "%lu", (unsigned long) ux->ut_pid);
      if (strlen (buf) <= room)
	strncpy (u->ut_host + strlen (u->ut_host) + 1, buf, room);
    }

  return 0;
}
