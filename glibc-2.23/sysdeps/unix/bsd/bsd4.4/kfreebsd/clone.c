/* Create a thread.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

#define __clone __no_broken_clone_decl
#include <sched.h>
#include <sys/rfork.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <getosreldate.h>
#undef __clone

/* __start_thread (flags, child_stack, fn, arg)
   is roughly equivalent to

     int retval = __rfork (flags);
     if (retval == 0)
       {
         // Here we are in the child thread.
         %stackpointer = child_stack;
         _exit (fn (arg));
       }
     return retval;

   but it cannot be done in portable C because it must access fn and arg
   after having replaced the stack pointer.  */

extern int __start_thread (int flags, void *child_stack,
			   int (*fn) (void *), void *arg);

int __clone (int (*fn) (void *), void *child_stack, int flags, void *arg)
{
  int rfork_flags = RFPROC;

  if (fn == NULL || child_stack == NULL)
    {
      __set_errno (EINVAL);
      return -1;
    }

  /* This implementation of clone() does not support all Linux flags.  */
  if (flags & ~(CSIGNAL | CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
		| CLONE_VFORK | CLONE_SYSVSEM))
    {
      __set_errno (EINVAL);
      return -1;
    }

  if ((flags & CSIGNAL) != SIGCHLD)
  {
    if (__kernel_getosreldate() >= 802510)
                /* we slightly cheat here, */
                /* the 9.x snapshot prior to r223966 does not support it too */
    {
      if ((flags & CSIGNAL) & ~RFTSIGMASK)
	{
	  __set_errno (EINVAL);
	  return -1;
	}
        rfork_flags |= (RFTSIGZMB | RFTSIGFLAGS(flags & CSIGNAL));
    }
    else
    {
      if ((flags & CSIGNAL) & ~RFTHPNMASK)
	{
	  __set_errno (EINVAL);
	  return -1;
	}
      if ((flags & CSIGNAL) == 0)
        rfork_flags |= (RFLINUXTHPN | ((SIGCHLD) <<  RFTHPNSHIFT));
      else
        rfork_flags |= (RFLINUXTHPN | ((flags & CSIGNAL) <<  RFTHPNSHIFT));
    }
  } 
  if (flags & CLONE_VM)
    rfork_flags |= RFMEM;

  if (flags & CLONE_FS)
    /* Sharing the filesystem related info (umask, cwd, root dir)
       is not supported by rfork.  Ignore this; let's hope programs
       will set their umask and cwd before spawning threads.  */
    ;

  if (flags & CLONE_SYSVSEM)
    /* Ignore this; it has been introduced into linuxthreads in post 2.4 glibc */
    ;

  if (!(flags & CLONE_FILES))
    rfork_flags |= RFFDG;

  if (flags & CLONE_SIGHAND)
    {
      rfork_flags |= RFSIGSHARE;
      /* Also set the undocumented flag RFTHREAD.  It has the effect that when
	 the thread leader exits, all threads belonging to it are killed.  */
      rfork_flags |= RFTHREAD;
    }

  if (flags & CLONE_VFORK)
    rfork_flags |= RFPPWAIT;

  return __start_thread (rfork_flags, child_stack, fn, arg);
}

weak_alias (__clone, clone)
