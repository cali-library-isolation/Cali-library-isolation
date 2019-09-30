/* Initialization code run first thing by the ELF startup code.
   Copyright (C) 1995-2004, 2005, 2007 Free Software Foundation, Inc.
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sysdep.h>
#include <signal.h>
#include <fpu_control.h>
#include <sys/param.h>
#include <sys/types.h>
#include <libc-internal.h>

#include <ldsodefs.h>

extern int __syscall_sigaction (int __sig,  const struct sigaction *__act, struct sigaction *__oact) __THROW;
libc_hidden_proto (__syscall_sigaction)

/* Set nonzero if we have to be prepared for more then one libc being
   used in the process.  Safe assumption if initializer never runs.  */
int __libc_multiple_libcs attribute_hidden = 1;

/* Remember the command line argument and enviroment contents for
   later calls of initializers for dynamic libraries.  */
int __libc_argc attribute_hidden;
char **__libc_argv attribute_hidden;


void
__libc_init_first (int argc, char **argv, char **envp)
{
#ifdef SHARED
  /* For DSOs we do not need __libc_init_first but instead _init.  */
}

void
attribute_hidden
_init (int argc, char **argv, char **envp)
{
#endif
#ifdef USE_NONOPTION_FLAGS
  extern void __getopt_clean_environment (char **);
#endif

  __libc_multiple_libcs = &_dl_starting_up && !_dl_starting_up;

  /* Make sure we don't initialize twice.  */
  if (!__libc_multiple_libcs)
    {
      /* Set the FPU control word to the proper default value if the
	 kernel would use a different value.  (In a static program we
	 don't have this information.)  */
#if 0
      /* at least on kFreeBSD set it even if SHARED,
      fixes "make check" failures like math/test-fpucw.out */
#ifdef SHARED
      if (__fpu_control != GLRO(dl_fpu_control))
#endif
#endif
	__setfpucw (__fpu_control);
    }

  /* By default on kFreeBSD when a call to non existing syscall is
     made, the program is terminated. As we want to be able to detect
     missing syscalls and provide a fallback code, we ignore the SIGSYS
     signal. */
  {
    struct sigaction act;
    
    act.sa_handler = SIG_IGN;
    __sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    
    INLINE_SYSCALL (sigaction, 3, SIGSYS, &act, NULL); 
  }  

  /* Save the command-line arguments.  */
  __libc_argc = argc;
  __libc_argv = argv;
  __environ = envp;

#ifndef SHARED
  __libc_init_secure ();

  /* First the initialization which normally would be done by the
     dynamic linker.  */
  _dl_non_dynamic_init ();
#endif

#ifdef VDSO_SETUP
  VDSO_SETUP ();
#endif

  __init_misc (argc, argv, envp);

#ifdef USE_NONOPTION_FLAGS
  /* This is a hack to make the special getopt in GNU libc working.  */
  __getopt_clean_environment (envp);
#endif

  /* Initialize ctype data.  */
  __ctype_init ();

#if defined SHARED && !defined NO_CTORS_DTORS_SECTIONS
  __libc_global_ctors ();
#endif
}


/* This function is defined here so that if this file ever gets into
   ld.so we will get a link error.  Having this file silently included
   in ld.so causes disaster, because the _init definition above will
   cause ld.so to gain an init function, which is not a cool thing. */

extern void _dl_start (void) __attribute__ ((noreturn));

void
_dl_start (void)
{
  abort ();
}
