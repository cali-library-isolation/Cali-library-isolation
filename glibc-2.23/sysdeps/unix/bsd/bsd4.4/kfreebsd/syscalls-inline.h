/* prototypes of generally used "inline syscalls"
   Copyright (C) 2006 Free Software Foundation, Inc.
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

#ifndef KFREEBSD_INLINE_SYSCALLS_H
#define KFREEBSD_INLINE_SYSCALLS_H

#include <sys/types.h>
#include <signal.h>
#include <syscalls-internal.h>

struct iovec;
struct rusage;
struct timespec;

int __syscall_open(const char *path, int flags, ...);
int __syscall_close(int fd);
libc_hidden_proto (__syscall_open)
libc_hidden_proto (__syscall_close)

ssize_t __syscall_read(int fd, void *buf, size_t nbyte);
ssize_t __syscall_write(int fd, const void *buf, size_t nbyte);
ssize_t __syscall_writev(int fd, const struct iovec *iovp, int iovcnt);
libc_hidden_proto (__syscall_read)
libc_hidden_proto (__syscall_write)
libc_hidden_proto (__syscall_writev)

ssize_t __syscall_readlink(const char *path, char *buf, size_t bufsiz);
libc_hidden_proto (__syscall_readlink)

int __syscall_fcntl(int fd, int cmd, ...);
int __syscall_fork(void);
int __syscall_wait4(int pid, int *status, int options, struct rusage *rusage);
int __syscall_sigsuspend (const sigset_t *set);
int __syscall_sigprocmask (int how, const sigset_t *set, sigset_t *oldset);
int __syscall_nanosleep (const struct timespec *requested_time, struct timespec *remaining);
libc_hidden_proto (__syscall_fcntl)
libc_hidden_proto (__syscall_fork)
libc_hidden_proto (__syscall_wait4)
libc_hidden_proto (__syscall_sigsuspend)
libc_hidden_proto (__syscall_sigprocmask)
libc_hidden_proto (__syscall_nanosleep)

int __syscall_sigwait     (const sigset_t *set, int *sig);
int __syscall_sigwaitinfo (const sigset_t *set, siginfo_t *info);
int __syscall_sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);
libc_hidden_proto (__syscall_sigwait)
libc_hidden_proto (__syscall_sigwaitinfo)
libc_hidden_proto (__syscall_sigtimedwait)

int __syscall_clock_getcpuclockid2(int64_t id, int which, clockid_t *clock_id);
libc_hidden_proto (__syscall_clock_getcpuclockid2)
                           
int __syscall_thr_exit(long *p);
int __syscall_thr_kill(long id, int sig);
int __syscall_thr_kill2(int pid, long id, int sig);
int __syscall_thr_new(void *arg, int size);
int __syscall_thr_self(long *id);
int __syscall_thr_set_name(long id, const char *name);
libc_hidden_proto (__syscall_thr_exit)
libc_hidden_proto (__syscall_thr_kill)
libc_hidden_proto (__syscall_thr_kill2)
libc_hidden_proto (__syscall_thr_new)
libc_hidden_proto (__syscall_thr_self)
libc_hidden_proto (__syscall_thr_set_name)

#endif
