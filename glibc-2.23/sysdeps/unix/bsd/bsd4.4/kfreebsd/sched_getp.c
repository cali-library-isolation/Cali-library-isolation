/* Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <sched.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/rtprio.h>

/* Retrieve scheduling parameters for a particular process.  */
int
__sched_getparam (pid_t pid, struct sched_param *param)
{
  /* kFreeBSD return bogus values for SYS_sched_param (see PR kern/76485);
     fortunately the same information can be retrieved through the rtprio()
     system call.  */
  struct rtprio rtp;

  if (__rtprio (RTP_LOOKUP, pid, &rtp) >= 0)
    {
      if (RTP_PRIO_IS_REALTIME (rtp.type))
        param->sched_priority = RTP_PRIO_MAX - rtp.prio;
      else
        param->sched_priority = 0;
    }

  return 0;
}

weak_alias (__sched_getparam, sched_getparam)
