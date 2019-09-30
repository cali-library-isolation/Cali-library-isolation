/* Definitions of flag bits for `waitpid' et al.
   Copyright (C) 1992, 1996-1997, 2000, 2002 Free Software Foundation, Inc.
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

#if !defined _SYS_WAIT_H && !defined _STDLIB_H
# error "Never include <bits/waitflags.h> directly; use <sys/wait.h> instead."
#endif


/* Bits in the third argument to `waitpid'.  */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

/* Bits in the fourth argument to `waitid'.  */
#define	WSTOPPED	2	/* Report stopped child (same as WUNTRACED). */
#define	WCONTINUED	4	/* Report continued child.  */
#define	WNOWAIT		8	/* Poll only. Don't delete the proc entry. */
#define WEXITED         16      /* Wait for exited processes. */
#define WTRAPPED        32      /* Wait for a process to hit a trap or
                                   a breakpoint. */
                                   
#define __WCLONE	0x80000000	/* Wait for cloned process.  */
#ifdef __USE_MISC
# define WLINUXCLONE	__WCLONE	/* FreeBSD name for __WCLONE.  */
#endif


/* The following values are used by the `waitid' function.  */
#if defined __USE_MISC || defined __USE_XOPEN || defined __USE_XOPEN2K8
# ifndef __ENUM_IDTYPE_T
# define __ENUM_IDTYPE_T 1

typedef enum
{
        /*
         * These names were mostly lifted from Solaris source code and
         * still use Solaris style naming to avoid breaking any
         * OpenSolaris code which has been ported to FreeBSD.  There
         * is no clear FreeBSD counterpart for all of the names, but
         * some have a clear correspondence to FreeBSD entities.
         *
         * The numerical values are kept synchronized with the Solaris
         * values.
         */
        P_PID,                  /* A process identifier. */
        P_PPID,                 /* A parent process identifier. */
        P_PGID,                 /* A process group identifier. */
        P_SID,                  /* A session identifier. */
        P_CID,                  /* A scheduling class identifier. */
        P_UID,                  /* A user identifier. */
        P_GID,                  /* A group identifier. */
        P_ALL,                  /* All processes. */
        P_LWPID,                /* An LWP identifier. */
        P_TASKID,               /* A task identifier. */
        P_PROJID,               /* A project identifier. */
        P_POOLID,               /* A pool identifier. */
        P_JAILID,               /* A zone identifier. */
        P_CTID,                 /* A (process) contract identifier. */
        P_CPUID,                /* CPU identifier. */
        P_PSETID                /* Processor set identifier. */
} idtype_t;                     /* The type of id_t we are using. */

#  if defined __USE_MISC
#  define P_ZONEID        P_JAILID
#  endif
# endif
#endif
