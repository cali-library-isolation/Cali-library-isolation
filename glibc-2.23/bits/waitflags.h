/* Definitions of flag bits for `waitpid' et al.
   Copyright (C) 1992-2016 Free Software Foundation, Inc.
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

#if !defined _SYS_WAIT_H && !defined _STDLIB_H
# error "Never include <bits/waitflags.h> directly; use <sys/wait.h> instead."
#endif


/* Bits in the third argument to `waitpid'.  */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

/* The following values are used by the `waitid' function.  */
#if defined __USE_XOPEN || defined __USE_XOPEN2K8
# ifndef __ENUM_IDTYPE_T
# define __ENUM_IDTYPE_T 1
typedef enum
{
  P_ALL,		/* Wait for any child.  */
  P_PID,		/* Wait for specified process.  */
  P_PGID		/* Wait for members of process group.  */
} idtype_t;
# endif
#endif
