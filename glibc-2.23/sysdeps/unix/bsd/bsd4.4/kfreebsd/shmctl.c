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

#include <errno.h>
#include <sys/shm.h>
#include <ipc_priv.h>

#include <sysdep.h>
#include <string.h>
#include <sys/syscall.h>
#include <bits/wordsize.h>
#include <shlib-compat.h>

#include <kernel-features.h>

/* Provide operations to control over shared memory segments.  */
extern int __syscall_shmctl (int shmid, int cmd, struct shmid_ds *buf);
libc_hidden_proto (__syscall_shmctl)


int
__new_shmctl (int shmid, int cmd, struct shmid_ds *buf)
{
  return INLINE_SYSCALL (shmctl, 3, shmid, cmd, buf);
}
versioned_symbol (libc, __new_shmctl, shmctl, GLIBC_2_18);

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_18)
struct shmid_ds_old {
        struct ipc_perm_old shm_perm;   /* operation permission structure */
        int             shm_segsz;      /* size of segment in bytes */
        pid_t           shm_lpid;   /* process ID of last shared memory op */
        pid_t           shm_cpid;       /* process ID of creator */
        unsigned short  shm_nattch;     /* number of current attaches */
        time_t          shm_atime;      /* time of last shmat() */
        time_t          shm_dtime;      /* time of last shmdt() */
        time_t          shm_ctime;      /* time of last change by shmctl() */
        void           *shm_internal;   /* sysv stupidity */
};

int
attribute_compat_text_section
__old_shmctl (int shmid, int cmd, struct shmid_ds_old *buf)
{
    struct shmid_ds newbuf;
    int rv;
    
    if (cmd == IPC_SET)
    {
        ipc_perm_old2new(&(buf->shm_perm), &(newbuf.shm_perm));
        newbuf.shm_segsz = buf->shm_segsz;
        newbuf.shm_lpid  = buf->shm_lpid;
        newbuf.shm_cpid  = buf->shm_cpid;
        newbuf.shm_nattch= buf->shm_nattch;
        newbuf.shm_atime = buf->shm_atime;
        newbuf.shm_dtime = buf->shm_dtime;
        newbuf.shm_ctime = buf->shm_ctime;
    }
    
    rv = __new_shmctl (shmid, cmd, &newbuf);
    
    if ((rv != -1) && (cmd == IPC_STAT))
    {
        ipc_perm_new2old(&(newbuf.shm_perm), &(buf->shm_perm));
        buf->shm_segsz = newbuf.shm_segsz;
        buf->shm_lpid  = newbuf.shm_lpid;
        buf->shm_cpid  = newbuf.shm_cpid;
        buf->shm_nattch= newbuf.shm_nattch;
        buf->shm_atime = newbuf.shm_atime;
        buf->shm_dtime = newbuf.shm_dtime;
        buf->shm_ctime = newbuf.shm_ctime;
    }
        
    return rv;
}
compat_symbol (libc, __old_shmctl, shmctl, GLIBC_2_0);
#endif
