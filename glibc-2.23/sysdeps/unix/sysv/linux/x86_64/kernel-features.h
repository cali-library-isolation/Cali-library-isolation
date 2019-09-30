/* Set flags signalling availability of kernel features based on given
   kernel version number.  x86-64 version.
   Copyright (C) 1999-2016 Free Software Foundation, Inc.
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

/* The accept4 syscall was added for x86-64 in 2.6.28.  */
#define __ASSUME_ACCEPT4_SYSCALL	1

/* The recvmmsg syscall was added for x86-64 in 2.6.33.  */
#if __LINUX_KERNEL_VERSION >= 0x020621
# define __ASSUME_RECVMMSG_SYSCALL	1
#endif

/* The sendmmsg syscall was added for x86-64 in 3.0.  */
#if __LINUX_KERNEL_VERSION >= 0x030000
# define __ASSUME_SENDMMSG_SYSCALL	1
#endif

/* getcpu is a syscall for x86-64 since 3.1.  */
#if __LINUX_KERNEL_VERSION >= 0x030100
# define __ASSUME_GETCPU_SYSCALL	1
#endif

#include_next <kernel-features.h>
