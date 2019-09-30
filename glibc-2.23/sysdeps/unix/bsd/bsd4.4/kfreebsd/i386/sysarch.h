/* Parameters for the architecture specific system call.  i386 version.
   Copyright (C) 2002 Free Software Foundation, Inc.
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

#ifndef _SYSARCH_H
#define _SYSARCH_H	1

#include <features.h>

/* Function that can be used as first argument to 'sysarch'.  */
enum
  {
    I386_GET_LDT = 0,
#define I386_GET_LDT I386_GET_LDT
    I386_SET_LDT = 1,
#define I386_SET_LDT I386_SET_LDT
    I386_GET_IOPERM = 3,
#define I386_GET_IOPERM I386_GET_IOPERM
    I386_SET_IOPERM = 4,
#define I386_SET_IOPERM I386_SET_IOPERM
    I386_VM86 = 6,
#define I386_VM86 I386_VM86
    I386_GET_FSBASE = 7,
#define I386_GET_FSBASE I386_GET_FSBASE
    I386_SET_FSBASE = 8,
#define I386_SET_FSBASE I386_SET_FSBASE
    I386_GET_GSBASE = 9,
#define I386_GET_GSBASE I386_GET_GSBASE
    I386_SET_GSBASE = 10
#define I386_SET_GSBASE I386_SET_GSBASE
  };

/* Argument struct for I386_GET_LDT and I386_SET_LDT.  */
struct i386_ldt_args
  {
    int start;
    union descriptor *descs;
    int num;
  };

/* Argument struct for I386_GET_IOPERM and I386_SET_IOPERM.  */
struct i386_ioperm_args
  {
    unsigned int start;
    unsigned int length;
    int enable;
  };

/* Argument struct for I386_VM86.  */
struct i386_vm86_args
  {
    int sub_op;
    void *sub_args;
  };

__BEGIN_DECLS

extern int sysarch (int __cmd, void *__arg);

#ifdef _LIBC
extern int __sysarch (int __cmd, void *__arg);
#endif

__END_DECLS

#endif /* _SYSARCH_H */
