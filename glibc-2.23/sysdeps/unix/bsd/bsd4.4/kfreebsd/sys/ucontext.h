/* Data structures for user-level context switching.  Generic version.
   Copyright (C) 1997-1999, 2001-2002 Free Software Foundation, Inc.
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

/* This file's definitions suffice for any platform where all
   the machine-specific state is described in `struct sigcontext'.  */

#ifndef _SYS_UCONTEXT_H
#define _SYS_UCONTEXT_H	1

#include <features.h>
#include <signal.h>

/* We need the signal context definitions even if they are not used
   included in <signal.h>.  */
#include <bits/sigcontext.h>

/* This include file defines the type 'mcontext_t'.  */
#include <bits/mcontext.h>

/* Userlevel context.  */
typedef struct ucontext
  {
    __sigset_t uc_sigmask;
    mcontext_t uc_mcontext;
    struct ucontext *uc_link;
    stack_t uc_stack;
    int     uc_flags;
#define UCF_SWAPPED     0x00000001      /* Used by swapcontext(3). */
    int __unused1[4];
  } ucontext_t;

#endif /* sys/ucontext.h */
