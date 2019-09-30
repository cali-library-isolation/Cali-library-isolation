/* Machine-dependent signal context structure for FreeBSD.  i386 version.
   Copyright (C) 1991-1992,1994,1997,2001-2002 Free Software Foundation, Inc.
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

#if !defined _SIGNAL_H && !defined _SYS_UCONTEXT_H
# error "Never use <bits/sigcontext.h> directly; include <signal.h> instead."
#endif

#ifndef _BITS_SIGCONTEXT_H
#define _BITS_SIGCONTEXT_H  1

#ifdef __i386__

/* State of this thread when the signal was taken.
   The unions below are for compatibility with Linux (whose sigcontext
   components don't have sc_ prefix) */
__extension__ struct sigcontext
  {
    __sigset_t 	sc_mask;		/* Blocked signals to restore.  */
    int 	sc_onstack;		/* Nonzero if running on sigstack.  */

    /* Segment registers.  */
    union { int sc_gs; int gs; };
    union { int sc_fs; int fs; };
    union { int sc_es; int es; };
    union { int sc_ds; int ds; };

    /* "General" registers.  These members are in the order that the i386
       `pusha' and `popa' instructions use (`popa' ignores %esp).  */
    union { int sc_edi; int edi; };
    union { int sc_esi; int esi; };
    union { int sc_ebp; int ebp; };
    union { int sc_isp; int isp; };		/* Not used; sc_esp is used instead.  */
    union { int sc_ebx; int ebx; };
    union { int sc_edx; int edx; };
    union { int sc_ecx; int ecx; };
    union { int sc_eax; int eax; };

    union { int sc_trapno; int trapno; };
    union { int sc_err; int err; };

    union { int sc_eip; int eip; };		/* Instruction pointer.  */
    union { int sc_cs; int cs; };		/* Code segment register.  */

    union { int sc_efl; int eflags; }; 		/* Processor flags.  */

    union { int sc_esp; int esp; };		/* This stack pointer is used.  */
    union { int sc_ss; int ss; };		/* Stack segment register.  */

    int     sc_len;                 /* sizeof(mcontext_t) */
    /*
     * XXX - See <machine/ucontext.h> and <machine/npx.h> for
     *       the following fields.
     */
    int     sc_fpformat;
    int     sc_ownedfp;
    int     sc_spare1[1];
    int     sc_fpstate[128] __attribute__((aligned(16)));
    int     sc_spare2[8];
  };

/* Traditional BSD names for some members.  */
#define sc_sp		sc_esp		/* Stack pointer.  */
#define sc_fp		sc_ebp		/* Frame pointer.  */
#define sc_pc		sc_eip		/* Process counter.  */
#define sc_ps		sc_efl
#define sc_eflags	sc_efl

#if 1 /* FIXME: These need verification.  */

/* Codes for SIGILL.  */
#define ILL_PRIVIN_FAULT	1
#define ILL_ALIGN_FAULT		14
#define ILL_FPOP_FAULT		24

/* Codes for SIGBUS.  */
#define BUS_PAGE_FAULT		12
#define BUS_SEGNP_FAULT		26
#define BUS_STK_FAULT		27
#define BUS_SEGM_FAULT		29

#endif

#else

__extension__ struct sigcontext
  {
    __sigset_t 	sc_mask;		/* Blocked signals to restore.  */
    long 	sc_onstack;		/* Nonzero if running on sigstack.  */
    union { long sc_rdi; long rdi;};
    union { long sc_rsi; long rsi;};
    union { long sc_rdx; long rdx;};
    union { long sc_rcx; long rcx;};
    union { long sc_r8; long r8;};
    union { long sc_r9; long r9;};
    union { long sc_rax; long rax;};
    union { long sc_rbx; long rbx;};
    union { long sc_rbp; long rbp;};
    union { long sc_r10; long r10;};
    union { long sc_r11; long r11;};
    union { long sc_r12; long r12;};
    union { long sc_r13; long r13;};
    union { long sc_r14; long r14;};
    union { long sc_r15; long r15;};
    union { long sc_trapno; long trapno;};
    union { long sc_addr; long addr;};
    union { long sc_flags; long flags;};
    union { long sc_err; long err;};
    union { long sc_rip; long rip;};
    union { long sc_cs; long cs;};
    union { long sc_rflags; long rflags;};
    union { long sc_rsp; long rsp;};
    union { long sc_ss; long ss;};
    long	sc_len;        /* sizeof(mcontext_t) */
    /*
     * XXX - See <machine/ucontext.h> and <machine/fpu.h> for
     *       the following fields.
     */
    long	sc_fpformat;
    long	sc_ownedfp;
    long	sc_fpstate[64] __attribute__((aligned(16)));
    long	sc_spare[8];
  };

/* Traditional BSD names for some members.  */
#define sc_sp           sc_rsp          /* Stack pointer.  */
#define sc_fp           sc_rbp          /* Frame pointer.  */
#define sc_pc           sc_rip          /* Process counter.  */

#endif

#endif /* _BITS_SIGCONTEXT_H */
