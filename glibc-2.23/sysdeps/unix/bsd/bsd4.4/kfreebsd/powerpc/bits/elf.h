/* This file defines standard ELF types, structures, and macros.
   Copyright (C) 2011 Free Software Foundation, Inc.
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

#ifndef _BITS_ELF_H
#define _BITS_ELF_H

__BEGIN_DECLS

#define	AT_DCACHEBSIZE	10	/* Data cache block size for the processor. */
#define	AT_ICACHEBSIZE	11	/* Instruction cache block size for the uP. */
#define	AT_UCACHEBSIZE	12	/* Cache block size, or `0' if cache not unified. */
#define	AT_EXECPATH	13	/* Path to the executable. */
#define	AT_CANARY	14	/* Canary for SSP */
#define	AT_CANARYLEN	15	/* Length of the canary. */
#define	AT_OSRELDATE	16	/* OSRELDATE. */
#define	AT_NCPUS	17	/* Number of CPUs. */
#define	AT_PAGESIZES	18	/* Pagesizes. */
#define	AT_PAGESIZESLEN	19	/* Number of pagesizes. */
#define	AT_STACKPROT	21	/* Initial stack protection. */

__END_DECLS

#endif	/* elf.h */
