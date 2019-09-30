/* Copyright (C) 2004, 2010 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Robert Millan

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

/*-
 * Copyright (c) 2002 Doug Rabson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sysdep.h>
#include <sys/sem.h>
#include <stdarg.h> /* va_list */
#include <stdlib.h> /* NULL */
#include <unistd.h>
#include <ipc_priv.h>

#include <sysdep.h>
#include <string.h>
#include <sys/syscall.h>
#include <bits/wordsize.h>
#include <shlib-compat.h>


/* union semun from FreeBSD <sys/sem.h> */
/*
 * semctl's arg parameter structure
 */
union semun
{
  int val;			/* value for SETVAL */
  struct semid_ds *buf;		/* buffer for IPC_STAT & IPC_SET */
  struct semid_ds_old *oldbuf;	/* buffer for IPC_STAT & IPC_SET */
  unsigned short *array;	/* array for GETALL & SETALL */
};

extern int __syscall_semctl (int semid, int semnum,
                             int cmd, union semun *arg);
libc_hidden_proto (__syscall_semctl)

int
__new_semctl (int semid, int semnum, int cmd, ...)
{
    va_list ap;
    union semun semun;
    union semun *semun_ptr;

    va_start (ap, cmd);
    switch (cmd) 
    {
        case SEM_STAT:
        case IPC_SET: 
        case IPC_STAT: 
        case GETALL:
        case SETVAL:  
        case SETALL:
            semun = va_arg (ap, union semun);
            semun_ptr = &semun;
        break;
        default:                                                  
            semun_ptr = NULL;
    }
    va_end (ap);
    return INLINE_SYSCALL (semctl, 4, semid, semnum, cmd, semun_ptr);
}
versioned_symbol (libc, __new_semctl, semctl, GLIBC_2_18);

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_18)

struct semid_ds_old
{
  struct ipc_perm_old sem_perm;		/* operation permission struct */
  void *__sem_base;
  unsigned short int sem_nsems;		/* number of semaphores in set */
  __time_t sem_otime;			/* last semop() time */
  long __unused1;
  __time_t sem_ctime;			/* last time changed by semctl() */
  long __unused2;
  long __unused3[4];
};

int
attribute_compat_text_section
__old_semctl (int semid, int semnum, int cmd, ...)
{
    struct semid_ds newbuf;
    struct semid_ds_old *buf;
    int rv;

    va_list ap;
    union semun semun;
    union semun *semun_ptr;

    va_start (ap, cmd);
    switch (cmd) 
    {
        case SEM_STAT:
        case IPC_SET: 
        case IPC_STAT:
            semun = va_arg (ap, union semun);
            buf = semun.oldbuf;
            semun.buf = &newbuf;
	    semun_ptr = &semun;
	break;
        case GETALL:
        case SETVAL:  
        case SETALL:
            semun = va_arg (ap, union semun);
	    semun_ptr = &semun;
        break;
        default:                                                  
            semun_ptr = NULL;
    }
    va_end (ap);
  
    if (cmd == IPC_SET)
    {
        ipc_perm_old2new(&(buf->sem_perm), &(newbuf.sem_perm));
        newbuf.__sem_base = buf->__sem_base;
        newbuf.sem_nsems  = buf->sem_nsems;
        newbuf.sem_otime  = buf->sem_otime;
        newbuf.sem_ctime  = buf->sem_ctime;
    }
    
    rv = INLINE_SYSCALL (semctl, 4, semid, semnum, cmd, semun_ptr);
    
    if ((rv != -1) && ((cmd == IPC_STAT) || (cmd == SEM_STAT)))
    {
        ipc_perm_new2old(&(newbuf.sem_perm), &(buf->sem_perm));
        buf->__sem_base = newbuf.__sem_base;
        buf->sem_nsems  = newbuf.sem_nsems;
        buf->sem_otime  = newbuf.sem_otime;
        buf->sem_ctime  = newbuf.sem_ctime;
    }
        
    return rv;
}
compat_symbol (libc, __old_semctl, semctl, GLIBC_2_0);
#endif
