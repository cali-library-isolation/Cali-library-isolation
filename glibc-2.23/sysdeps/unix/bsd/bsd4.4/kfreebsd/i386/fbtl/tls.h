/* Definition for thread-local data handling.  fbtl/i386 version.
   Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
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

#ifndef _FREEBSD_TLS_H
#define _FREEBSD_TLS_H

#include <fbtl/sysdeps/i386/tls.h>

# ifndef __ASSEMBLER__

#undef TLS_INIT_TP
#undef TLS_SETUP_GS_SEGMENT
#undef TLS_GET_GS
#undef TLS_SET_GS       
#undef __NR_set_thread_area

#include <sysarch.h>
#include <sys/syscall.h>


/* Code to initially initialize the thread pointer.  This might need
   special attention since 'errno' is not yet available and if the
   operation can cause a failure 'errno' must not be touched. */

#  define TLS_DO_SET_GSBASE(descr)		\
({                                      	\
  long base = (long) descr;             	\
  int result;                           	\
  asm volatile (                        	\
                "pushl %3\n\t"          	\
                "pushl %2\n\t"          	\
                "pushl %1\n\t"       		\
                "int $0x80\n\t"         	\
                "popl %3\n\t"        		\
                "popl %3\n\t"        		\
                "popl %3\n\t"        		\
                : "=a" (result)         	\
                : "0" (SYS_sysarch),     	\
                  "i" (I386_SET_GSBASE),       	\
                  "d" (&base)			\
                : "memory", "cc" );    		\
  result;                                       \
})

#   define TLS_SETUP_GS_SEGMENT(descr, secondcall)                            \
  (TLS_DO_SET_GSBASE(descr)                                                   \
   ? "set_thread_area failed when setting up thread-local storage\n" : NULL)

/*   The value of this macro is null if successful, or an error string.  */

#  define TLS_INIT_TP(descr)						      \
  ({									      \
    void *_descr = (descr);						      \
    tcbhead_t *head = _descr;						      \
									      \
    head->tcb = _descr;							      \
    /* For now the thread descriptor is at the same address.  */	      \
    head->self = _descr;						      \
									      \
    INIT_SYSINFO;							      \
    TLS_SETUP_GS_SEGMENT (_descr, secondcall);				      \
  })

#if 0
/* in Linux one; */
/* Magic for libthread_db to know how to do THREAD_SELF.  */
# define DB_THREAD_SELF \
  REGISTER_THREAD_AREA (32, offsetof (struct user_regs_struct, xgs), 3) \
  REGISTER_THREAD_AREA (64, 26 * 8, 3) /* x86-64's user_regs_struct->gs */
#else
/* # warning proper variant needed */
# undef DB_THREAD_SELF_INCLUDE
# undef DB_THREAD_SELF
# define DB_THREAD_SELF \
  REGISTER_THREAD_AREA (32, 10 * 4, 3) /* offsetof (struct user_regs_struct, xgs) */ \
  REGISTER_THREAD_AREA (64, 26 * 8, 3) /* x86-64's user_regs_struct->gs */
#endif

/* in fact this is OS-specific, but we do not have better header for it */
#define NEED_STACK_SIZE_FOR_PTH_CREATE 1
 
#endif /* __ASSEMBLER__ */

#endif	/* tls.h */
