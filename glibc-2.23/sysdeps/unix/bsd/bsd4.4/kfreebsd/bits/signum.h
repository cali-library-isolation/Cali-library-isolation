/* Signal number definitions.  FreeBSD version.
   Copyright (C) 1991-1993, 1996, 1998, 2002 Free Software Foundation, Inc.
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

#ifdef	_SIGNAL_H

/* This file defines the fake signal functions and signal
   number constants for 4.2 or 4.3 BSD-derived Unix system.  */

/* Fake signal functions.  */
#define	SIG_ERR	((__sighandler_t) -1) /* Error return.  */
#define	SIG_DFL	((__sighandler_t) 0) /* Default action.  */
#define	SIG_IGN	((__sighandler_t) 1) /* Ignore signal.  */

#define	SIG_CATCH ((__sighandler_t) 2) /* FreeBSD specific ? */
#define	SIG_HOLD  ((__sighandler_t) 3) /* Add signal to hold mask.  */

/* Signals.  */
#define	SIGHUP		1	/* Hangup (POSIX).  */
#define	SIGINT		2	/* Interrupt (ANSI).  */
#define	SIGQUIT		3	/* Quit (POSIX).  */
#define	SIGILL		4	/* Illegal instruction (ANSI).  */
#define	SIGABRT		SIGIOT	/* Abort (ANSI).  */
#define	SIGTRAP		5	/* Trace trap (POSIX).  */
#define	SIGIOT		6	/* IOT trap (4.2 BSD).  */
#define	SIGEMT		7	/* EMT trap (4.2 BSD).  */
#define	SIGFPE		8	/* Floating-point exception (ANSI).  */
#define	SIGKILL		9	/* Kill, unblockable (POSIX).  */
#define	SIGBUS		10	/* Bus error (4.2 BSD).  */
#define	SIGSEGV		11	/* Segmentation violation (ANSI).  */
#define	SIGSYS		12	/* Bad argument to system call (4.2 BSD).  */
#define	SIGPIPE		13	/* Broken pipe (POSIX).  */
#define	SIGALRM		14	/* Alarm clock (POSIX).  */
#define	SIGTERM		15	/* Termination (ANSI).  */
#define	SIGURG		16	/* Urgent condition on socket (4.2 BSD).  */
#define	SIGSTOP		17	/* Stop, unblockable (POSIX).  */
#define	SIGTSTP		18	/* Keyboard stop (POSIX).  */
#define	SIGCONT		19	/* Continue (POSIX).  */
#define	SIGCHLD		20	/* Child status has changed (POSIX).  */
#define	SIGCLD		SIGCHLD	/* Same as SIGCHLD (System V).  */
#define	SIGTTIN		21	/* Background read from tty (POSIX).  */
#define	SIGTTOU		22	/* Background write to tty (POSIX).  */
#define	SIGIO		23	/* I/O now possible (4.2 BSD).  */
#define	SIGPOLL		SIGIO	/* Pollable event occurred (System V).  */
#define	SIGXCPU		24	/* CPU limit exceeded (4.2 BSD).  */
#define	SIGXFSZ		25	/* File size limit exceeded (4.2 BSD).  */
#define	SIGVTALRM	26	/* Virtual alarm clock (4.2 BSD).  */
#define	SIGPROF		27	/* Profiling alarm clock (4.2 BSD).  */
#define	SIGWINCH	28	/* Window size change (4.3 BSD, Sun).  */
#define SIGINFO		29	/* Information request (4.4 BSD).  */
#define	SIGUSR1		30	/* User-defined signal 1 (POSIX).  */
#define	SIGUSR2		31	/* User-defined signal 2 (POSIX).  */
/* Signals 32 and 33 are reserved for system libraries. */

/* Signal 34 is used (but not reserved) by thread library.
   See PTHREAD_SIGBASE in kernel-features.h.  */

#define	_NSIG		129	/* Biggest signal number + 1
				   (including real-time signals).  */

#define SIGRTMIN	(__libc_current_sigrtmin ())
#define SIGRTMAX	(__libc_current_sigrtmax ())

/* These are the hard limits of the kernel.  These values should not be
   used directly at user level.  */
#define __SIGRTMIN	65	/* be in sync with FreeBSD kernel */
#define __SIGRTMAX	126	/* be in sync with FreeBSD kernel */

#endif	/* <signal.h> included.  */
