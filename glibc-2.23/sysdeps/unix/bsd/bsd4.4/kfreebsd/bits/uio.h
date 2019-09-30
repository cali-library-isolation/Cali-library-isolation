/* Copyright (C) 1996-1997, 2002 Free Software Foundation, Inc.
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

#if !defined _SYS_UIO_H && !defined _FCNTL_H
# error "Never include <bits/uio.h> directly; use <sys/uio.h> instead."
#endif

#ifndef _BITS_UIO_H
#define _BITS_UIO_H     1

#include <sys/types.h>


/* `struct iovec' -- Structure describing a section of memory.  */

struct iovec
{
  /* Starting address.  */
  __ptr_t iov_base;
  /* Length in bytes.  */
  size_t iov_len;
};

/* Maximum number of 'struct iovec's that can be passed to a readv() or
   writev() system call.  For larger arrays of 'struct iovec', the libc
   uses a single read() or write() call to guarantee atomicity.  */
#define UIO_MAXIOV	1024

#ifdef __USE_MISC
enum    uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
        UIO_USERSPACE,          /* from user data space */
        UIO_SYSSPACE,           /* from system space */
        UIO_NOCOPY              /* don't copy, already in object */
};
#endif

#endif
