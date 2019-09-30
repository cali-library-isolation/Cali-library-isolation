/* Copyright (C) 2007-2016 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern char **__libc_argv attribute_hidden;

void
__attribute__ ((noreturn)) internal_function
__fortify_fail (const char *msg)
{
  int do_abort;
  /* Disable backtraces for fortify failures on the stack.  */
  if (strncmp(msg, "stack", 5) == 0)
    do_abort = 1;
  else
    do_abort = 2;
  /* The loop is added only to keep gcc happy.  */
  while (1)
    __libc_message (do_abort, "*** %s ***: %s terminated\n",
		    msg, __libc_argv[0] ?: "<unknown>");
}
libc_hidden_def (__fortify_fail)
