/* Initialize the signal state.  Hurd on Mach version.
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
#include <pt-internal.h>

error_t
__pthread_sigstate_init (struct __pthread *thread)
{
  static int do_init_global;

  /* Mark the thread as a global signal receiver so as to conform with
     the pthread semantics.  However, we must be careful.  The first
     pthread created is the main thread, during libpthread initialization.
     We must not mark it, otherwise the sigprocmask call in
     __pthread_create would try to access _hurd_global_sigstate,
     which is not initialized yet.  When glibc runs _hurdsig_init later
     on, the message thread is created, which must not be marked either.  */
  if (do_init_global)
    {
      struct hurd_sigstate *ss = _hurd_thread_sigstate (thread->kernel_thread);
      _hurd_sigstate_set_global_rcv (ss);
    }
  else if (__pthread_num_threads >= 2)
    do_init_global = 1;

  return 0;
}
