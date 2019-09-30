/* Machine-dependent processor state structure for FreeBSD.
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.  i386 version.

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

#if !defined _SYS_UCONTEXT_H
# error "Never use <bits/mcontext.h> directly; include <sys/ucontext.h> instead."
#endif

/*-
 * Copyright (c) 1999 Marcel Moolenaar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * based on $FreeBSD: src/sys/i386/include/ucontext.h,v 1.10 2002/12/02 19:58:55 deischen Exp $
 */

/* Whole processor state.  */
typedef struct
  {
    /*
     * The first 20 fields must match the definition of
     * sigcontext. So that we can support sigcontext
     * and ucontext_t at the same time.
     */

    int mc_onstack;		/* Nonzero if running on sigstack.  */

    /* Segment registers.  */
    int mc_gs;
    int mc_fs;
    int mc_es;
    int mc_ds;

    /* "General" registers.  These members are in the order that the i386
       `pusha' and `popa' instructions use (`popa' ignores %esp).  */
    int mc_edi;
    int mc_esi;
    int mc_ebp;
    int mc_isp;			/* Not used; sc_esp is used instead.  */
    int mc_ebx;
    int mc_edx;
    int mc_ecx;
    int mc_eax;

    int mc_trapno;
    int mc_err;

    int mc_eip;			/* Instruction pointer.  */
    int mc_cs;			/* Code segment register.  */

    int mc_efl;			/* Processor flags.  */

    int mc_esp;			/* This stack pointer is used.  */
    int mc_ss;			/* Stack segment register.  */

    int mc_len;			/* sizeof(mcontext_t) */
#define	_MC_FPFMT_NODEV		0x10000	/* device not present or configured */
#define	_MC_FPFMT_387		0x10001
#define	_MC_FPFMT_XMM		0x10002
    int mc_fpformat;
#define	_MC_FPOWNED_NONE	0x20000	/* FP state not used */
#define	_MC_FPOWNED_FPU		0x20001	/* FP state came from FPU */
#define	_MC_FPOWNED_PCB		0x20002	/* FP state came from PCB */
    int mc_ownedfp;
    int mc_spare1[1];		/* align next field to 16 bytes */
    /*
     * See <machine/npx.h> for the internals of mc_fpstate[].
     */
    int mc_fpstate[128] __attribute__((aligned(16)));
    int mc_spare2[8];
  } mcontext_t;

/* Traditional BSD names for some members.  */
#define mc_eflags	mc_efl
