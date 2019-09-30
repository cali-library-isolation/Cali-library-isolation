/* Set flags signalling availability of kernel features based on given
   kernel version number.
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

#ifndef __KFREEBSD_KERNEL_VERSION
/* We assume the worst; all kernels should be supported.  */
# define __KFREEBSD_KERNEL_VERSION	0
#endif

/* The encoding for __KFREEBSD_KERNEL_VERSION is defined the following
   way: the major, minor, and subminor all get a byte with the major
   number being in the highest byte.  This means we can do numeric
   comparisons.

   In the following we will define certain symbols depending on
   whether the describes kernel feature is available in the kernel
   version given by __KFREEBSD_KERNEL_VERSION.  We are not always exactly
   recording the correct versions in which the features were
   introduced.  If somebody cares these values can afterwards be
   corrected.  */

/* 
   The used encoding corresponds to the following in elf/dl-load.c:

            osversion = (abi_note[5] & 0xff) * 65536
                        + (abi_note[6] & 0xff) * 256
                        + (abi_note[7] & 0xff);
            if (abi_note[4] != __ABI_TAG_OS
                || (GLRO(dl_osversion) && GLRO(dl_osversion) < osversion))
            
   Therefore, the __KFREEBSD_KERNEL_VERSION have different value compared to
   __FreeBSD_version/__FreeBSD_kernel__version. 
   The transformation is not just prepend 0x to __FreeBSD_kernel_version.

   For changes see i.e.
   http://www.freebsd.org/doc/en/books/porters-handbook/freebsd-versions.html
*/

/* Real-time signals introduced in FreeBSD 7.x.  */
# define __ASSUME_REALTIME_SIGNALS	1

/* Use signals #32, #33, #34 for internal linuxthreads communication */
#define PTHREAD_SIGBASE 32

/* The `ftruncate' syscall was introduced in kFreeBSD 7.0. */
# define __ASSUME_TRUNCATE_SYSCALL	1

/* The `lseek' syscall was introduced in kFreeBSD 7.0. */
# define __ASSUME_LSEEK_SYSCALL		1

/* The `mmap' syscall was introduced in kFreeBSD 7.0. */
# define __ASSUME_MMAP_SYSCALL		1

/* The `pread' and `pwrite' syscalls were introduced in kFreeBSD 7.0. */
# define __ASSUME_PREAD_PWRITE_SYSCALLS	1

/* The `shm_*' syscalls were introduced in kFreeBSD 8.0 */
# define __ASSUME_SHMFCTS		1

/* The `*at' syscalls were introduced in kFreeBSD 8.0. */
# define __ASSUME_ATFCTS		1

/* The pselect syscall was introduced in kFreeBSD 8.1. */
# define __ASSUME_PSELECT		1

/* The posix_fallocate syscall was introduced in kFreeBSD 8.3. */
#if __KFREEBSD_KERNEL_VERSION >= 0x80300
# define __ASSUME_FALLOCATE		1
#endif

/* The wait6 syscall was introduced in kFreeBSD 9.2. */
#if __KFREEBSD_KERNEL_VERSION >= 0x90200
# define __ASSUME_WAIT6			1
#endif

/* Support for private "futexes" was added before we start with fbtl. */
# define __ASSUME_PRIVATE_FUTEX		1
