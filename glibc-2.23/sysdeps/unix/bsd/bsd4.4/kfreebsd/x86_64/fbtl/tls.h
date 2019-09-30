/* Definitions for thread-local data handling.  fbtl/x86-64 version.
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Modification for FreeBSD by Petr Salinger, 2006.

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

#ifndef _FREEBSD_TLS_H
#define _FREEBSD_TLS_H

#include <fbtl/sysdeps/x86_64/tls.h>

#ifndef __ASSEMBLER__

#include <sysarch.h>
#include <sys/syscall.h>

#if 0
/* in Linux one; */
/* Magic for libthread_db to know how to do THREAD_SELF.  */
# define DB_THREAD_SELF_INCLUDE  <sys/reg.h> /* For the FS constant.  */
# define DB_THREAD_SELF CONST_THREAD_AREA (64, FS)
#else
/* # warning proper FS constant needed */
# undef DB_THREAD_SELF_INCLUDE
# undef DB_THREAD_SELF
# define DB_THREAD_SELF CONST_THREAD_AREA (64, 25)
#endif

/* Code to initially initialize the thread pointer.  This might need
   special attention since 'errno' is not yet available and if the
   operation can cause a failure 'errno' must not be touched.  */

# undef TLS_INIT_TP
# define TLS_INIT_TP(descr)						      \
  ({									      \
    void *_descr = (descr);						      \
    tcbhead_t *head = _descr;						      \
    long int _result;							      \
									      \
    head->tcb = _descr;							      \
    /* For now the thread descriptor is at the same address.  */	      \
    head->self = _descr;						      \
									      \
    asm volatile ("syscall"						      \
		  : "=a" (_result)					      \
		  : "0" ((unsigned long int) SYS_sysarch),		      \
		    "D" ((unsigned long int) AMD64_SET_FSBASE),		      \
		    "S" (&_descr)					      \
		  : "memory", "cc", "cx", "dx", "r8", "r9", "r10", "r11");    \
									      \
    _result ? "cannot set %fs base address for thread-local storage" : 0;     \
  })

/* in fact this is OS-specific, but we do not have better header for it */
#define NEED_STACK_SIZE_FOR_PTH_CREATE 1

#endif /* __ASSEMBLER__ */

#endif	/* tls.h */
