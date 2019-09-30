/* Copyright (C) 1995-2013 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Legal values for a_type (entry type).  */

#define	AT_NULL		0	/* End of vector */
#define	AT_IGNORE	1	/* Entry should be ignored */
#define	AT_EXECFD	2	/* File descriptor of program */
#define	AT_PHDR		3	/* Program headers for program */
#define	AT_PHENT	4	/* Size of program header entry */
#define	AT_PHNUM	5	/* Number of program headers */
#define	AT_PAGESZ	6	/* System page size */
#define	AT_BASE		7	/* Base address of interpreter */
#define	AT_FLAGS	8	/* Flags */
#define	AT_ENTRY	9	/* Entry point of program */
#define	AT_NOTELF	10	/* Program is not ELF */
#define	AT_UID		11	/* Real uid */
#define	AT_EUID		12	/* Effective uid */
#define	AT_GID		13	/* Real gid */
#define	AT_EGID		14	/* Effective gid */
#define	AT_EXECPATH	15	/* Path to the executable. */
#define	AT_CANARY	16	/* Canary for SSP. */
#define	AT_CANARYLEN	17	/* Length of the canary. */
#define	AT_OSRELDATE	18	/* OSRELDATE. */
#define	AT_NCPUS	19	/* Number of CPUs. */
#define	AT_PAGESIZES	20	/* Pagesizes. */
#define	AT_PAGESIZESLEN	21	/* Number of pagesizes. */
#define	AT_TIMEKEEP	22	/* Pointer to timehands. */
#define	AT_STACKPROT	23	/* Initial stack protection. */

