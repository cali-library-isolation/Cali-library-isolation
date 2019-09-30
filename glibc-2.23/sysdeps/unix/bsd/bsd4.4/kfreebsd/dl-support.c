/* Dynamic linker system dependencies for GNU/kFreeBSD.
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

#include <ldsodefs.h>
#ifdef HAVE_AUX_VECTOR
#include <bits/auxv.h>
int _dl_clktck;

void
internal_function
_dl_aux_init (ElfW(auxv_t) *av)
{
  for (; av->a_type != AT_NULL; ++av)
    switch (av->a_type)
      {
      case AT_PAGESZ:
	GLRO(dl_pagesize) = av->a_un.a_val;
	break;
      case AT_PHDR:
	GL(dl_phdr) = (void *) av->a_un.a_val;
	break;
      case AT_PHNUM:
	GL(dl_phnum) = av->a_un.a_val;
	break;
      }
}
#undef HAVE_AUX_VECTOR
#endif

#include <elf/dl-support.c>
