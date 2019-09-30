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

#include <sysdeps/generic/ldconfig.h>

#define SYSDEP_KNOWN_INTERPRETER_NAMES \
  { "/lib/ld.so.1", FLAG_ELF_LIBC6 },				\
  { "/lib/ld-kfreebsd-x86-64.so.1", FLAG_ELF_LIBC6 },		\
  { "/lib64/ld-kfreebsd-x86-64.so.1", FLAG_ELF_LIBC6 },		\
  { "/libexec/ld-elf.so.1", FLAG_ELF_LIBC5 },

#define SYSDEP_KNOWN_LIBRARY_NAMES \
  { "libc.so.0.1", FLAG_ELF_LIBC6 },				\
  { "libm.so.1", FLAG_ELF_LIBC6 },				\
  { "libc.so.4", FLAG_ELF_LIBC5 },				\
  { "libm.so.4", FLAG_ELF_LIBC5 },				\
  { "libc.so.5", FLAG_ELF_LIBC5 },				\
  { "libm.so.5", FLAG_ELF_LIBC5 },				\
  { "libc.so.6", FLAG_ELF_LIBC5 },				\
  { "libm.so.6", FLAG_ELF_LIBC5 },				\
  { "libc.so.7", FLAG_ELF_LIBC5 },				\
  { "libm.so.7", FLAG_ELF_LIBC5 },				\
  { "libc.so.8", FLAG_ELF_LIBC5 },				\
  { "libm.so.8", FLAG_ELF_LIBC5 },
