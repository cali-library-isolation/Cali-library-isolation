/* Copyright (C) 1997-2013 Free Software Foundation, Inc.
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

#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <sysdep.h>
#include <sys/syscall.h>

#include <kernel-features.h>

#include <pthreadP.h>

/* Get and/or change the set of blocked signals.  */
int
#ifdef BUILD_AS_pthread_sigmask
pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
#else
__sigprocmask (int how, const sigset_t *set, sigset_t *oset)
#endif
{
  sigset_t local_newmask;
  int rv;

  /* The only thing we have to make sure here is that SIGCANCEL and
     SIGSETXID are not blocked.  */
  if (set != NULL
      && (__builtin_expect (__sigismember (set, SIGCANCEL), 0)
	  || __builtin_expect (__sigismember (set, SIGSETXID), 0)
	  ))
    {
      local_newmask = *set;
      __sigdelset (&local_newmask, SIGCANCEL);
      __sigdelset (&local_newmask, SIGSETXID);
      set = &local_newmask;
    }

  rv = INLINE_SYSCALL (sigprocmask, 3, how, set, oset);
#ifdef BUILD_AS_pthread_sigmask
  return rv;
}  
#else
  if (rv)
  {
      errno = rv;
      return -1;
  }
  return 0;        
}
weak_alias (__sigprocmask, sigprocmask)
#endif
