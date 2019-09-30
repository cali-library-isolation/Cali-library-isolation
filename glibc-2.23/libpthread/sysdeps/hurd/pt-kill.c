/* pthread_kill.  Hurd version.
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <hurd/signal.h>

#include <pt-internal.h>

int
pthread_kill (pthread_t thread, int sig)
{
  struct __pthread *pthread;
  struct hurd_signal_detail detail;
  struct hurd_sigstate *ss;

  /* Lookup the thread structure for THREAD.  */
  pthread = __pthread_getid (thread);
  if (pthread == NULL)
    return ESRCH;

  ss = _hurd_thread_sigstate (pthread->kernel_thread);
  assert (ss);

  if (!sig)
    return 0;

  detail.exc = 0;
  detail.code = sig;
  detail.error = 0;

  __spin_lock (&ss->lock);
  return _hurd_raise_signal (ss, sig, &detail);
}
