# Copyright (C) 2002-2013 Free Software Foundation, Inc.
# This file is part of the GNU C Library.
# Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <http://www.gnu.org/licenses/>.

ifeq ($(subdir),fbtl)
sysdep_routines += register-atfork unregister-atfork libc_pthread_init \
		   libc_multiple_threads

libpthread-sysdep_routines += pt-fork pthread_mutex_cond_lock

gen-as-const-headers += lowlevelrwlock.sym \
			unwindbuf.sym \
 			structsem.sym
 			
#gen-as-const-headers += lowlevelcond.sym lowlevelrwlock.sym \
#			lowlevelbarrier.sym unwindbuf.sym \
#			lowlevelrobustlock.sym pthread-pi-defines.sym \
#			structsem.sym
endif

ifeq ($(subdir),fbtl)
routines += sys_thr_kill sys_thr_self
routines += sys_sigwaitinfo sys_sigwait sys_sigtimedwait
libpthread-routines += ptw-sys_sigwaitinfo ptw-sys_sigwait ptw-sys_sigtimedwait
libpthread-routines += ptw-sys_sigprocmask ptw-sys_umtx
libpthread-routines += ptw-sys_thr_kill
libpthread-routines += ptw-sys_thr_kill2
libpthread-routines += ptw-sys_thr_new
libpthread-routines += ptw-sys_thr_self
libpthread-routines += ptw-sys_thr_set_name
libpthread-routines += ptw-sys_thr_exit
libpthread-routines += ptw-sigprocmask
libpthread-routines += ptw-ioctl
libpthread-routines += ptw-sys_clock_getcpuclockid2
endif

ifeq ($(subdir),rt)
sysdep_routines += sys_clock_getcpuclockid2
endif

ifeq ($(subdir),posix)
CFLAGS-fork.c = $(libio-mtsafe)
CFLAGS-getpid.o = -fomit-frame-pointer
CFLAGS-getpid.os = -fomit-frame-pointer
endif

# Needed in both the signal and nptl subdir.
CFLAGS-sigaction.c = -DWRAPPER_INCLUDE='<fbtl/sigaction.c>'
