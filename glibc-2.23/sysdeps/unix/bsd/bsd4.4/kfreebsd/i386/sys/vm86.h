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

#ifndef _SYS_VM86_H
#define _SYS_VM86_H	1

#include <features.h>

/* Possible values for the 'cmd' argument to i386_vm86.  */
enum
  {
    VM86_INIT = 1,
#define VM86_INIT VM86_INIT
    VM86_GET_VME = 3
#define VM86_GET_VME VM86_GET_VME
  };

/* Argument structure for VM86_INIT.  */
struct vm86_init_args
  {
    int debug;
    int cpu_type;
    unsigned char int_map[32];
  };

/* Argument structure for VM86_GET_VME.  */
struct vm86_vme_args
  {
    int state;
  };

__BEGIN_DECLS

extern int i386_vm86 (int __cmd, void *__arg);

__END_DECLS

#endif	/* _SYS_VM86_H */
