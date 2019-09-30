/* Run-time dynamic linker data structures for loaded ELF shared objects.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
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

#ifndef	_LDSODEFS_H

/* Get the real definitions.  */
#include_next <ldsodefs.h>

/* Now define our stuff.  */

/* FreeBSD puts some extra information into an auxiliary vector when it
   execs ELF executables.  Note that it uses AT_* values of 10 and 11
   to denote something different than AT_NOTELF and AT_UID, but this is
   not a problem since elf/dl-support.c ignores these AT_* values.  */
#define HAVE_AUX_VECTOR

/* Used by static binaries to check the auxiliary vector.  */
extern void _dl_aux_init (ElfW(auxv_t) *av) internal_function;

/* Initialization which is normally done by the dynamic linker.  */
extern void _dl_non_dynamic_init (void) internal_function;

/* Contrary to most kernels which use ELFOSABI_SYSV aka ELFOSABI_NONE,
   FreeBSD uses ELFOSABI_FREEBSD for the OSABI field. */
#undef VALID_ELF_HEADER
#define VALID_ELF_HEADER(hdr,exp,size)	(memcmp (hdr, expected2, size) == 0)
#undef VALID_ELF_OSABI
#define VALID_ELF_OSABI(osabi)		(osabi == ELFOSABI_FREEBSD)
#undef MORE_ELF_HEADER_DATA
#define MORE_ELF_HEADER_DATA \
  static const unsigned char expected2[EI_PAD] =	\
  {							\
    [EI_MAG0] = ELFMAG0,				\
    [EI_MAG1] = ELFMAG1,				\
    [EI_MAG2] = ELFMAG2,				\
    [EI_MAG3] = ELFMAG3,				\
    [EI_CLASS] = ELFW(CLASS),				\
    [EI_DATA] = byteorder,				\
    [EI_VERSION] = EV_CURRENT,				\
    [EI_OSABI] = ELFOSABI_FREEBSD,			\
    [EI_ABIVERSION] = 0					\
  }

#endif /* ldsodefs.h */
