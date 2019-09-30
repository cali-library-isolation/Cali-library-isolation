/* Copyright (C) 2013 Free Software Foundation, Inc.
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

#ifndef _LOWLEVELUMTX_H
#define _LOWLEVELUMTX_H	1

#include <kernel-features.h>
#include <sysdep.h>
#include <atomic.h>

/* from <sys/umtx.h> */
/* op code for _umtx_op */
#define	UMTX_OP_WAIT		2
#define	UMTX_OP_WAKE		3
#define	UMTX_OP_WAIT_UINT	11
#define	UMTX_OP_WAIT_UINT_PRIVATE	15
#define	UMTX_OP_WAKE_PRIVATE	16

int __syscall__umtx_op(void *, int, long, void*, void*);
libc_hidden_proto (__syscall__umtx_op)

#define UMTX_SYSCALL __syscall__umtx_op

/*
 * 
   we provide set of functions
   lll_umtx_{int,long}_{wait,wake}_{private,shared}
 *  
 */

/*******************************************************/

#define lll_umtx_int_wait_private(addr, val, timeout)	\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAIT_UINT_PRIVATE, 			\
        (long)((unsigned int)(val)),			\
        NULL,						\
        timeout);					\
   })

#define lll_umtx_int_wait_shared(addr, val, timeout)	\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAIT_UINT,	 			\
        (long)((unsigned int)(val)),			\
        NULL,						\
        timeout);					\
   })

#define lll_umtx_long_wait_private(addr, val, timeout)	\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAIT,  /* only autoshare available */	\
        val,						\
        NULL,						\
        timeout);					\
   })

#define lll_umtx_long_wait_shared(addr, val, timeout)	\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAIT,  /* only autoshare available */	\
        val,						\
        NULL,						\
        timeout);					\
   })

/*******************************************************/

#define lll_umtx_int_wake_private(addr, num)  		\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAKE_PRIVATE,	 			\
        num,						\
        NULL,						\
        NULL);						\
   })

#define lll_umtx_int_wake_shared(addr, num)  		\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAKE,		 			\
        num,						\
        NULL,						\
        NULL);						\
   })

#define lll_umtx_long_wake_private(addr, num) 		\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAKE_PRIVATE,	 			\
        num,						\
        NULL,						\
        NULL);						\
   })

#define lll_umtx_long_wake_shared(addr, num)  		\
  ({							\
    UMTX_SYSCALL(					\
        addr,						\
        UMTX_OP_WAKE,		 			\
        num,						\
        NULL,						\
        NULL);						\
   })

/*******************************************************/

#endif	/* _LOWLEVELUMTX_H */
