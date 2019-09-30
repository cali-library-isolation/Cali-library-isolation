/*-
 * Copyright (c) 1984, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ptrace.h	8.2 (Berkeley) 1/4/94
 * $FreeBSD: src/sys/sys/ptrace.h,v 1.28.10.1.2.1 2009/10/25 01:10:29 kensmith Exp $
 */
 
#ifndef _SYS_PTRACE_H
#define _SYS_PTRACE_H	1

#include <features.h>

#include <signal.h>
#include <sys/kern/param.h>
#include <machine/reg.h>

__BEGIN_DECLS

#define	PT_TRACE_ME	0	/* child declares it's being traced */
#define	PT_READ_I	1	/* read word in child's I space */
#define	PT_READ_D	2	/* read word in child's D space */
/* was	PT_READ_U	3	 * read word in child's user structure */
#define	PT_WRITE_I	4	/* write word in child's I space */
#define	PT_WRITE_D	5	/* write word in child's D space */
/* was	PT_WRITE_U	6	 * write word in child's user structure */
#define	PT_CONTINUE	7	/* continue the child */
#define	PT_KILL		8	/* kill the child process */
#define	PT_STEP		9	/* single step the child */

#define	PT_ATTACH	10	/* trace some running process */
#define	PT_DETACH	11	/* stop tracing a process */
#define PT_IO		12	/* do I/O to/from stopped process. */

#define	PT_LWPINFO	13	/* Info about the LWP that stopped. */
#define PT_GETNUMLWPS	14	/* get total number of threads */
#define PT_GETLWPLIST	15	/* get thread list */

#define PT_CLEARSTEP	16	/* turn off single step */
#define PT_SETSTEP	17	/* turn on single step */
#define PT_SUSPEND	18	/* suspend a thread */
#define PT_RESUME	19	/* resume a thread */

#define	PT_TO_SCE	20
#define	PT_TO_SCX	21
#define	PT_SYSCALL	22

#define PT_GETREGS      33	/* get general-purpose registers */
#define PT_SETREGS      34	/* set general-purpose registers */
#define PT_GETFPREGS    35	/* get floating-point registers */
#define PT_SETFPREGS    36	/* set floating-point registers */
#define PT_GETDBREGS    37	/* get debugging registers */
#define PT_SETDBREGS    38	/* set debugging registers */
#define PT_FIRSTMACH    64	/* for machine-specific requests */

struct ptrace_io_desc {
	int	piod_op;	/* I/O operation */
	void	*piod_offs;	/* child offset */
	void	*piod_addr;	/* parent offset */
	size_t	piod_len;	/* request length */
};

/*
 * Operations in piod_op.
 */
#define PIOD_READ_D	1	/* Read from D space */
#define PIOD_WRITE_D	2	/* Write to D space */
#define PIOD_READ_I	3	/* Read from I space */
#define PIOD_WRITE_I	4	/* Write to I space */

/* Argument structure for PT_LWPINFO. */
struct ptrace_lwpinfo {
	__lwpid_t	pl_lwpid;	/* LWP described. */
	int	pl_event;	/* Event that stopped the LWP. */
#define	PL_EVENT_NONE	0
#define	PL_EVENT_SIGNAL	1
	int	pl_flags;	/* LWP flags. */
#define	PL_FLAG_SA	0x01	/* M:N thread */
#define	PL_FLAG_BOUND	0x02	/* M:N bound thread */
#define	PL_FLAG_SCE	0x04	/* syscall enter point */
#define	PL_FLAG_SCX	0x08	/* syscall leave point */
#define	PL_FLAG_EXEC	0x10	/* exec(2) succeeded */
#define	PL_FLAG_SI	0x20	/* siginfo is valid */
#define	PL_FLAG_FORKED	0x40	/* new child */
#define	PL_FLAG_CHILD	0x80	/* I am from child */
	sigset_t	pl_sigmask;	/* LWP signal mask */
	sigset_t	pl_siglist;	/* LWP pending signal */
	struct siginfo	pl_siginfo;	/* siginfo for signal */
	char		pl_tdname[MAXCOMLEN + 1]; /* LWP name */
	int		pl_child_pid;	/* New child pid */
};

extern int ptrace(int _request, pid_t _pid, caddr_t _addr, int _data) __THROW;

__END_DECLS

#endif /* _SYS_PTRACE_H */
