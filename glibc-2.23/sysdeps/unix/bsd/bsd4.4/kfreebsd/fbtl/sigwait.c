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
#define __need_NULL
#include <stddef.h>
#include <string.h>

#include <sysdep-cancel.h>
#include <sys/syscall.h>
#include <fbtl/pthreadP.h>


/* Return any pending signal or wait for one for the given time.  */
static int
do_sigwait (const sigset_t *set, int *sig)
{
  int ret;

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
  do {
      ret = INLINE_SYSCALL (sigwait, 2, set, sig);
  } while (ret == EINTR);
  
  return ret;
}

int
__sigwait (const sigset_t *set, int *sig)
{
  if (SINGLE_THREAD_P)
    return do_sigwait (set, sig);

  int oldtype = LIBC_CANCEL_ASYNC ();

  int result = do_sigwait (set, sig);

  LIBC_CANCEL_RESET (oldtype);

  return result;
}
libc_hidden_def (__sigwait)
weak_alias (__sigwait, sigwait)
strong_alias (__sigwait, __libc_sigwait)
