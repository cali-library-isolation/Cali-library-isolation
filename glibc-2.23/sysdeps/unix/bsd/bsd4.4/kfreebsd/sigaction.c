/* Copyright (C) 1991,1995,1996,1997,2002,2004 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>
#include <errno.h>
#include <stddef.h>
#include <signal.h>

extern int __syscall_sigaction (int __sig,  const struct sigaction *__act, struct sigaction *__oact) __THROW;
libc_hidden_proto (__syscall_sigaction)

int
__libc_sigaction (int sig, const struct sigaction *act, struct sigaction *oact)
{
  if (sig <= 0 || sig >= NSIG)
    {
      __set_errno (EINVAL);
      return -1;
    }

  return INLINE_SYSCALL (sigaction, 3, sig, act, oact);
}
libc_hidden_def (__libc_sigaction)

#ifdef WRAPPER_INCLUDE
# include WRAPPER_INCLUDE
#endif

#ifndef LIBC_SIGACTION
weak_alias (__libc_sigaction, __sigaction)
libc_hidden_weak (__sigaction)
weak_alias (__libc_sigaction, sigaction)
#endif
