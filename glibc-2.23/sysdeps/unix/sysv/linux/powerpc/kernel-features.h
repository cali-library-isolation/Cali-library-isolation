/* Set flags signalling availability of kernel features based on given
   kernel version number.  PowerPC version.
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

/* PowerPC uses socketcall.  */
#define __ASSUME_SOCKETCALL		1

/* The accept4 syscall was added for PowerPC in 2.6.37.  */
#if __LINUX_KERNEL_VERSION >= 0x020625
# define __ASSUME_ACCEPT4_SYSCALL	1
#endif

/* The recvmmsg syscall was added for PowerPC in 2.6.37.  */
#if __LINUX_KERNEL_VERSION >= 0x020625
# define __ASSUME_RECVMMSG_SYSCALL	1
#endif

/* New syscalls added for PowerPC in 2.6.37.  */
#if __LINUX_KERNEL_VERSION >= 0x020625
# define __ASSUME_SOCKET_SYSCALL	1
# define __ASSUME_BIND_SYSCALL		1
# define __ASSUME_CONNECT_SYSCALL	1
# define __ASSUME_LISTEN_SYSCALL	1
# define __ASSUME_ACCEPT_SYSCALL	1
# define __ASSUME_GETSOCKNAME_SYSCALL	1
# define __ASSUME_GETPEERNAME_SYSCALL	1
# define __ASSUME_SOCKETPAIR_SYSCALL	1
# define __ASSUME_SEND_SYSCALL		1
# define __ASSUME_SENDTO_SYSCALL	1
# define __ASSUME_RECV_SYSCALL		1
# define __ASSUME_RECVFROM_SYSCALL	1
# define __ASSUME_SHUTDOWN_SYSCALL	1
# define __ASSUME_GETSOCKOPT_SYSCALL	1
# define __ASSUME_SETSOCKOPT_SYSCALL	1
# define __ASSUME_SENDMSG_SYSCALL	1
# define __ASSUME_RECVMSG_SYSCALL	1
#endif

/* The sendmmsg syscall was added for PowerPC in 3.0.  */
#if __LINUX_KERNEL_VERSION >= 0x030000
# define __ASSUME_SENDMMSG_SYSCALL	1
#endif
#define __ASSUME_SENDMMSG_SYSCALL_WITH_SOCKETCALL	1

#include_next <kernel-features.h>
