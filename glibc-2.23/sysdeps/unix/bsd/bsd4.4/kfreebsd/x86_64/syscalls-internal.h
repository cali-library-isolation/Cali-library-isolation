/* generally used "internal syscalls"
   Copyright (C) 2009 Free Software Foundation, Inc.
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

#ifndef KFREEBSD_INTERNAL_SYSCALLS_H
#define KFREEBSD_INTERNAL_SYSCALLS_H

#include <sys/syscall.h>

/* 
   for now, we do not care whether syscall succeeded,
   we do not have defined 
   INTERNAL_SYSCALL_ERROR_P and INTERNAL_SYSCALL_ERRNO
   we do not store errno at all
   to be sure, we return void
*/

#undef INTERNAL_SYSCALL_DECL
#undef INTERNAL_SYSCALL_NCS
#undef INTERNAL_SYSCALL
#undef INTERNAL_SYSCALL_ERROR_P
#undef INTERNAL_SYSCALL_ERRNO

#define INTERNAL_SYSCALL_DECL(err)		\
  do { } while (0)

#define INTERNAL_SYSCALL(name, err, nr, args...) \
  INTERNAL_SYSCALL_##name(name, err, nr, ##args)


#define INTERNAL_SYSCALL_clock_gettime(name, err, nr, clkid, ts) \
(void)({                                        \
  register long int _a1 = (long int) (clkid);	\
  register long int _a2 = (long int) (ts);	\
  register long int result;			\
  asm volatile (				\
	"syscall"				\
        : "=a" (result)				\
        : "0" ((long int) SYS_##name),		\
	  "D" (_a1),				\
	  "S" (_a2)				\
	: "memory", "cc", "cx", "dx", "r8", "r9", "r10", "r11"); \
  result;                                       \
})

#define INTERNAL_SYSCALL_close(name, err, nr, fd) \
(void)({                                        \
  register long int _a1 = (long int) (fd);	\
  register long int result;			\
  asm volatile (				\
	"syscall"				\
        : "=a" (result)				\
        : "0" ((long int) SYS_##name),		\
	  "D" (_a1)				\
	: "memory", "cc", "cx", "dx", "r8", "r9", "r10", "r11"); \
  result;                                       \
})

#define INTERNAL_SYSCALL_kill(name, err, nr, pid, sig) \
(void)({                                        \
  register long int _a1 = (long int) (pid);	\
  register long int _a2 = (long int) (sig);	\
  register long int result;			\
  asm volatile (				\
	"syscall"				\
        : "=a" (result)				\
        : "0" ((long int) SYS_##name),		\
	  "D" (_a1),				\
	  "S" (_a2)				\
	: "memory", "cc", "cx", "dx", "r8", "r9", "r10", "r11"); \
  result;                                       \
})

#define INTERNAL_SYSCALL_write(name, err, nr, fd, buf, cnt) \
(void)({                                        \
  register long int _a1 = (long int) (fd);	\
  register long int _a2 = (long int) (buf);	\
  register long int _a3 = (long int) (cnt);	\
  register long int result;			\
  register long int _trash;			\
  asm volatile (				\
	"syscall"				\
        : "=a" (result),			\
          "=d" (_trash)				\
        : "0" ((long int) SYS_##name),		\
	  "D" (_a1),				\
	  "S" (_a2),				\
	  "d" (_a3)				\
          /*  beware rdx is not preserved after syscall */ \
	: "memory", "cc", "cx", "r8", "r9", "r10", "r11"); \
  result;                                       \
})

#define INTERNAL_SYSCALL_writev(name, err, nr, fd, iov, cnt) \
(void)({                                        \
  register long int _a1 = (long int) (fd);	\
  register long int _a2 = (long int) (iov);	\
  register long int _a3 = (long int) (cnt);	\
  register long int result;			\
  register long int _trash;			\
  asm volatile (				\
	"syscall"				\
        : "=a" (result),			\
          "=d" (_trash)				\
        : "0" ((long int) SYS_##name),		\
	  "D" (_a1),				\
	  "S" (_a2),				\
	  "d" (_a3)				\
          /*  beware rdx is not preserved after syscall */ \
	: "memory", "cc", "cx", "r8", "r9", "r10", "r11"); \
  result;                                       \
})

#endif
