/* Block a thread.  Mach version.
   Copyright (C) 2000 Free Software Foundation, Inc.
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

#include <assert.h>
#include <errno.h>

#include <mach.h>
#include <mach/message.h>

#include <pt-internal.h>

/* Block THREAD.  */
void
__pthread_block (struct __pthread *thread)
{
  mach_msg_header_t msg;
  error_t err;

  err = __mach_msg (&msg, MACH_RCV_MSG, 0, sizeof msg,
		    thread->wakeupmsg.msgh_remote_port,
		    MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
  assert_perror (err);
}
