/* Copyright (C) 2002 Free Software Foundation, Inc.
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

/*
 * Copyright (c) 1989, 1993
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
 *	@(#)unistd.h	8.2 (Berkeley) 1/7/94
 * $FreeBSD: src/sys/sys/unistd.h,v 1.22.2.1 2000/03/18 23:20:12 jasone Exp $
 */

#ifndef _SYS_RFORK_H
#define _SYS_RFORK_H

#include <features.h>

/*
 * rfork() options.
 *
 * XXX currently, operations without RFPROC set are not supported.
 */
#define RFNAMEG		(1<<0)  /* UNIMPL new plan9 `name space' */
#define RFENVG		(1<<1)  /* UNIMPL copy plan9 `env space' */
#define RFFDG		(1<<2)  /* copy fd table */
#define RFNOTEG		(1<<3)  /* UNIMPL create new plan9 `note group' */
#define RFPROC		(1<<4)  /* change child (else changes curproc) */
#define RFMEM		(1<<5)  /* share `address space' */
#define RFNOWAIT	(1<<6)  /* parent need not wait() on child */
#define RFCNAMEG	(1<<10) /* UNIMPL zero plan9 `name space' */
#define RFCENVG		(1<<11) /* UNIMPL zero plan9 `env space' */
#define RFCFDG		(1<<12) /* zero fd table */
#define RFTHREAD	(1<<13)	/* enable kernel thread support */
#define RFSIGSHARE	(1<<14)	/* share signal handlers */
#define RFLINUXTHPN     (1<<16) /* do linux clone exit parent notification */
#define	RFSTOPPED	(1<<17) /* leave child in a stopped state */
#define	RFHIGHPID	(1<<18) /* use a pid higher than 10 (idleproc) */
#define	RFTSIGZMB	(1<<19) /* select signal for exit parent notification */
#define	RFTSIGSHIFT	20      /* selected signal number is in bits 20-27  */
#define	RFTSIGMASK	0xFF
#define	RFTSIGNUM(flags)	(((flags) >> RFTSIGSHIFT) & RFTSIGMASK)
#define	RFTSIGFLAGS(signum)	((signum) << RFTSIGSHIFT)
#define RFPPWAIT	(1<<31) /* parent sleeps until child exits (vfork) */

#define RFTHPNSHIFT	24	/* reserve bits 24-30 */
#define RFTHPNMASK	0x7F    /* for compatibility with linuxthreads/clone()   */
				/* allow to specify  "clone exit parent notification" signal */
#define RFTHPNSIGNUM(flags)	(((flags) >> RFTHPNSHIFT) & RFTHPNMASK)

__BEGIN_DECLS

extern int rfork (int __flags) __THROW;

#ifdef _LIBC
extern int __rfork (int __flags);
#endif

__END_DECLS

#endif /* _SYS_RFORK_H */
