/* Reentrant function to return the current login name.  FreeBSD version.
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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sysdep.h>

/* Cache the system call's return value.  */
char *__getlogin_cache;
/* The kernel never returns more than MAXLOGNAME bytes, therefore we don't
   need more than that either.  */
char __getlogin_cache_room[MAXLOGNAME];

extern int __syscall_getlogin (char *__name, size_t __name_len);
libc_hidden_proto (__syscall_getlogin)

/* Return at most NAME_LEN characters of the login name of the user in NAME.
   If it cannot be determined or some other error occurred, return the error
   code.  Otherwise return 0.
   Note that the getlogin_r function in FreeBSD libc returns a 'char *',
   but SUSV2 wants a return type of 'int'.  */

int
__getlogin_r (char *name, size_t name_len)
{
  size_t len;

  if (__getlogin_cache == NULL)
    {
      if (INLINE_SYSCALL (getlogin, 2, __getlogin_cache_room, MAXLOGNAME) < 0)
	return errno;
      /* The system call should return a NULL terminated name.  */
      if (__memchr (__getlogin_cache_room, '\0', MAXLOGNAME) == NULL)
	abort ();
      __getlogin_cache = __getlogin_cache_room;
    }

  len = strlen (__getlogin_cache);
  if (__builtin_expect (len < name_len, 1))
    {
      memcpy (name, __getlogin_cache, len + 1);
      return 0;
    }
  else
    {
      __set_errno (ERANGE);
      return ERANGE;
    }
}
libc_hidden_def (__getlogin_r)
weak_alias (__getlogin_r, getlogin_r)
libc_hidden_weak (getlogin_r)

