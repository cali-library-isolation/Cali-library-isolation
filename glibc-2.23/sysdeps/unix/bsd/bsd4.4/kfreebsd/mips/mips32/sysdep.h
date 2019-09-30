/* Copyright (C) 2010 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Robert Millan.

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

#ifndef _KFREEBSD_MIPS_SYSDEP_H
#define _KFREEBSD_MIPS_SYSDEP_H 1

/* This drags in __syscall_error, keep in sync with Makefile in
   this directory.  */
#include <sysdeps/unix/sysv/linux/mips/mips32/sysdep.h>

/* Default INLINE_SYSCALL works but is buggy for 64-bit arguments
   (breaks mmap, lseek...).  */
#undef INLINE_SYSCALL
#define INLINE_SYSCALL(name, nr, args...) __syscall_##name(args)

/* Revert Linux kludge.  */
#undef SYS_ify
#ifdef __STDC__
# define SYS_ify(syscall_name)  SYS_##syscall_name
#else
# define SYS_ify(syscall_name)  SYS_/**/syscall_name
#endif

/* Workaround for http://sources.redhat.com/bugzilla/show_bug.cgi?id=12301 */
#ifdef __ASSEMBLER__
/* We don't want the label for the error handler to be visible in the symbol
   table when we define it here.  */
#ifdef __PIC__
# define SYSCALL_ERROR_LABEL 99b
#endif
#endif   /* ! __ASSEMBLER__ */

#endif /* _KFREEBSD_MIPS_SYSDEP_H */
