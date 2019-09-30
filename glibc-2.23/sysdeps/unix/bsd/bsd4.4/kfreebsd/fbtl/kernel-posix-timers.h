/* Copyright (C) 2003, 2007, 2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2003.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>


/* Nonzero if the system calls are not available.  */
extern int __no_posix_timers attribute_hidden;

/* Callback to start helper thread.  */
extern void __start_helper_thread (void) attribute_hidden;

/* Control variable for helper thread creation.  */
extern pthread_once_t __helper_once attribute_hidden;

/* TID of the helper thread.  */
extern pid_t __helper_tid attribute_hidden;

/* List of active SIGEV_THREAD timers.  */
extern struct timer *__active_timer_sigev_thread attribute_hidden;
/* Lock for the __active_timer_sigev_thread.  */
extern pthread_mutex_t __active_timer_sigev_thread_lock attribute_hidden;


/* Type of timers in the kernel.  */
typedef int kernel_timer_t;


/* Internal representation of timer.  */
struct timer
{
  /* Notification mechanism.  */
  int sigev_notify;

  /* Timer ID returned by the kernel.  */
  kernel_timer_t ktimerid;

  /* All new elements must be added after ktimerid.  And if the thrfunc
     element is not the third element anymore the memory allocation in
     timer_create needs to be changed.  */

  /* Parameters for the thread to be started for SIGEV_THREAD.  */
  void (*thrfunc) (sigval_t);
  sigval_t sival;
  pthread_attr_t attr;

  /* Next element in list of active SIGEV_THREAD timers.  */
  struct timer *next;
};

extern struct timer *__all_timers[TIMER_MAX];

static inline struct timer *
__kfreebsd_timer_alloc (void)
{
  unsigned int i;
  struct timer *timer = malloc (sizeof (struct timer));
  
  if (timer == NULL)
      goto fail;

  /* Find a free slot (and reserve it atomically).  */
  for (i = 0; i < TIMER_MAX; i++)
    if (atomic_compare_and_exchange_val_acq (&__all_timers[i],
					     timer, NULL) == NULL)
      return timer;
      
  free (timer);    

fail:
  errno = EAGAIN;
  return NULL;
}

static inline struct timer *
__kfreebsd_timer_id2ptr (timer_t id)
{
  void *ret = NULL;

  if (id >= 0 && id < TIMER_MAX)
    ret = __all_timers[id];

  if (! ret)
    errno = EINVAL;

  return ret;
}

static inline timer_t
__kfreebsd_timer_ptr2id (struct timer *ptr)
{
  unsigned int i;
  for (i = 0; i < TIMER_MAX; i++)
    if (__all_timers[i] == ptr)
      return i;

  return -1;
}

void static inline
__kfreebsd_timer_free (struct timer *ptr)
{
  __all_timers[__kfreebsd_timer_ptr2id (ptr)] = NULL;
  free (ptr);
}

/* used syscalls */

int __syscall_ktimer_create (clockid_t clockid, struct sigevent *evp, kernel_timer_t *timerid);
int __syscall_ktimer_delete (kernel_timer_t timerid);
int __syscall_ktimer_getoverrun (kernel_timer_t timerid);
int __syscall_ktimer_gettime (kernel_timer_t timerid, struct itimerspec *value);
int __syscall_ktimer_settime (kernel_timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *ovalue);

libc_hidden_proto (__syscall_ktimer_create)
libc_hidden_proto (__syscall_ktimer_delete)
libc_hidden_proto (__syscall_ktimer_getoverrun)
libc_hidden_proto (__syscall_ktimer_gettime)
libc_hidden_proto (__syscall_ktimer_settime)
