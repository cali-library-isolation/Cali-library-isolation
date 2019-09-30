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
#include <string.h>

#include <sysdep-cancel.h>
#include <sys/syscall.h>
#include <fbtl/pthreadP.h>

static int
do_sigtimedwait (const sigset_t *set, siginfo_t *info,
		 const struct timespec *timeout)
{
#ifdef SIGCANCEL
  sigset_t tmpset;
  if (set != NULL
      && (__builtin_expect (__sigismember (set, SIGCANCEL), 0)
# ifdef SIGSETXID
	  || __builtin_expect (__sigismember (set, SIGSETXID), 0)
# endif
	  ))
    {
      /* Create a temporary mask without the bit for SIGCANCEL set.  */
      tmpset = *set;
      __sigdelset (&tmpset, SIGCANCEL);
# ifdef SIGSETXID
      __sigdelset (&tmpset, SIGSETXID);
# endif
      set = &tmpset;
    }
#endif

  return INLINE_SYSCALL (sigtimedwait, 3, set, info, timeout);
}


/* Return any pending signal or wait for one for the given time.  */
int
__sigtimedwait (const sigset_t *set, siginfo_t *info,
		const struct timespec *timeout)
{
  if (SINGLE_THREAD_P)
    return do_sigtimedwait (set, info, timeout);

  int oldtype = LIBC_CANCEL_ASYNC ();

  int result = do_sigtimedwait (set, info, timeout);

  LIBC_CANCEL_RESET (oldtype);

  return result;
}
libc_hidden_def (__sigtimedwait)
weak_alias (__sigtimedwait, sigtimedwait)
