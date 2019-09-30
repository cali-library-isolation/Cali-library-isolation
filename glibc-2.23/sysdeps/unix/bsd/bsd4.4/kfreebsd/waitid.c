/* 
   Copyright (C) 2004-2012 Free Software Foundation, Inc.
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

#include <shlib-compat.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sysdep.h>

extern int __syscall_wait6 (idtype_t itype, int64_t id,
			    int *status, int options,
			    struct rusage *rusage, siginfo_t *infop);
libc_hidden_proto (__syscall_wait6)


#define DO_WAITID simulated_waitid
static int simulated_waitid (idtype_t idtype, id_t id, siginfo_t *infop, int options);


#if !defined __ASSUME_WAIT6
static int __have_wait6;
#endif

static inline int
do_waitid (idtype_t idtype, id_t id, siginfo_t *infop, int options)
{
  int ret;
  int status;
#ifndef __ASSUME_WAIT6
  if (__have_wait6 >= 0)
#endif
  {
      ret = INLINE_SYSCALL (wait6, 6, idtype, id, &status, options, NULL, infop);
      
      if (ret == 0 && infop != NULL)
      {
          memset(infop, 0, sizeof(*infop));
      }
      if (ret >= 0)
          return 0;
#ifndef __ASSUME_WAIT6
      if (errno == ENOSYS)
      {
          __have_wait6 = -1;
      }
      else
#endif
      {
          return ret;
      }
   }
   return simulated_waitid(idtype, id, infop, options);
}

 
#define waitid __unused_waitid_alias
#include <sysdeps/posix/waitid.c>
#undef waitid

versioned_symbol (libc, __waitid, waitid, GLIBC_2_18);

#if SHLIB_COMPAT (libc, GLIBC_2_1, GLIBC_2_18)

/* it used to be: */

#define OLD_P_ALL	0
#define OLD_P_PID	1
#define OLD_P_PGID	2

int
__waitid_old (idtype_t oldtype, id_t id, siginfo_t *infop, int options)
{
  idtype_t newtype;

  switch (oldtype)
  {
      case OLD_P_ALL:
          newtype = P_ALL;
      break;
      case OLD_P_PID:
          newtype = P_PID;
      break;
      case OLD_P_PGID:
          newtype = P_PGID;
      break;
      default:
          newtype = oldtype;
   }
  return __waitid(newtype, id, infop, options);
}
compat_symbol (libc, __waitid_old, waitid, GLIBC_2_1);
#endif
