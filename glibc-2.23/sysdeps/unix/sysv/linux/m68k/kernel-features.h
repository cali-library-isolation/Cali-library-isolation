/* Set flags signalling availability of kernel features based on given
   kernel version number.
   Copyright (C) 2008-2016 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* m68k uses socketcall.  */
#define __ASSUME_SOCKETCALL	1

/* Direct socketcalls available with kernel 4.3.  */
#if __LINUX_KERNEL_VERSION >= 0x040300
# define __ASSUME_RECVMMSG_SYSCALL           1
# define __ASSUME_SENDMMSG_SYSCALL           1
# define __ASSUME_SOCKET_SYSCALL             1
# define __ASSUME_SOCKETPAIR_SYSCALL         1
# define __ASSUME_BIND_SYSCALL               1
# define __ASSUME_CONNECT_SYSCALL            1
# define __ASSUME_LISTEN_SYSCALL             1
# define __ASSUME_ACCEPT4_SYSCALL            1
# define __ASSUME_ACCEPT4_FOR_ACCEPT_SYSCALL 1
# define __ASSUME_GETSOCKOPT_SYSCALL         1
# define __ASSUME_SETSOCKOPT_SYSCALL         1
# define __ASSUME_GETSOCKNAME_SYSCALL        1
# define __ASSUME_GETPEERNAME_SYSCALL        1
# define __ASSUME_SENDTO_SYSCALL             1
# define __ASSUME_SENDTO_FOR_SEND_SYSCALL    1
# define __ASSUME_SENDMSG_SYSCALL            1
# define __ASSUME_RECVFROM_SYSCALL           1
# define __ASSUME_RECVFROM_FOR_RECV_SYSCALL  1
# define __ASSUME_RECVMSG_SYSCALL            1
# define __ASSUME_SHUTDOWN_SYSCALL           1
#endif

#include_next <kernel-features.h>

/* These syscalls were added only in 3.0 for m68k.  */
#if __LINUX_KERNEL_VERSION < 0x030000
# undef __ASSUME_PSELECT
# undef __ASSUME_PPOLL
#endif

/* No support for PI futexes or robust mutexes before 3.10 for m68k.  */
#if __LINUX_KERNEL_VERSION < 0x030a00
# undef __ASSUME_FUTEX_LOCK_PI
# undef __ASSUME_REQUEUE_PI
# undef __ASSUME_SET_ROBUST_LIST
#endif
