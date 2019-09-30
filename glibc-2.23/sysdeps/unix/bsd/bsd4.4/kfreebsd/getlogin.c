/* Non-reentrant function to return the current login name.  FreeBSD version.
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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sysdep.h>

/* Defined in getlogin_r.c.  */
extern char *__getlogin_cache;
extern char __getlogin_cache_room[MAXLOGNAME];

extern int __syscall_getlogin (char *__name, size_t __name_len);
libc_hidden_proto (__syscall_getlogin)

/* Return the login name of the user, or NULL if it can't be determined.
   The returned pointer, if not NULL, is good only until the next call.  */

char *
getlogin (void)
{
  if (__getlogin_cache == NULL)
    {
      if (INLINE_SYSCALL (getlogin, 2, __getlogin_cache_room, MAXLOGNAME) < 0)
	return NULL;
      /* The system call should return a NULL terminated name.  */
      if (__memchr (__getlogin_cache_room, '\0', MAXLOGNAME) == NULL)
	abort ();
      __getlogin_cache = __getlogin_cache_room;
    }
  return (__getlogin_cache[0] ? __getlogin_cache : NULL);
}
