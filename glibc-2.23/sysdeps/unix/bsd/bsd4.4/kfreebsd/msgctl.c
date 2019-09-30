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
#include <sys/msg.h>
#include <ipc_priv.h>

#include <sysdep.h>
#include <string.h>
#include <sys/syscall.h>
#include <bits/wordsize.h>
#include <shlib-compat.h>

#include <kernel-features.h>

/* Provide operations to control over shared memory segments.  */

extern int __syscall_msgctl(int msqid, int cmd, struct msqid_ds *buf);
libc_hidden_proto (__syscall_msgctl)

int
__new_msgctl(int msqid, int cmd, struct msqid_ds *buf)
{
  return INLINE_SYSCALL (msgctl, 3, msqid, cmd, buf);
}
versioned_symbol (libc, __new_msgctl, msgctl, GLIBC_2_18);

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_18)
struct msqid_ds_old
{
  struct ipc_perm_old msg_perm;	/* structure describing operation permission */
  void *__msg_first;
  void *__msg_last;
  msglen_t __msg_cbytes;	/* current number of bytes on queue */
  msgqnum_t msg_qnum;		/* number of messages currently on queue */
  msglen_t msg_qbytes;		/* max number of bytes allowed on queue */
  __pid_t msg_lspid;		/* pid of last msgsnd() */
  __pid_t msg_lrpid;		/* pid of last msgrcv() */
  __time_t msg_stime;		/* time of last msgsnd command */
  long __unused1;
  __time_t msg_rtime;		/* time of last msgrcv command */
  long __unused2;
  __time_t msg_ctime;		/* time of last change */
  long __unused3;
  long __unused4[4];
};

int
attribute_compat_text_section
__old_msgctl(int msqid, int cmd, struct msqid_ds_old *buf)
{
    struct msqid_ds newbuf;
    int rv;
    
    if (cmd == IPC_SET)
    {
        ipc_perm_old2new(&(buf->msg_perm), &(newbuf.msg_perm));
        newbuf.__msg_first   = buf->__msg_first;
        newbuf.__msg_last    = buf->__msg_first;
        newbuf.__msg_cbytes  = buf->__msg_cbytes;
        newbuf.msg_qnum      = buf->msg_qnum;
        newbuf.msg_qbytes    = buf->msg_qbytes;
        newbuf.msg_lspid     = buf->msg_lspid;
        newbuf.msg_lrpid     = buf->msg_lrpid;
        newbuf.msg_stime     = buf->msg_stime;
        newbuf.msg_rtime     = buf->msg_rtime;
        newbuf.msg_ctime     = buf->msg_ctime;
    }
    
    rv = __new_msgctl (msqid, cmd, &newbuf);
    
    if ((rv != -1) && (cmd == IPC_STAT))
    {
        ipc_perm_new2old(&(newbuf.msg_perm), &(buf->msg_perm));
        buf->__msg_first   = newbuf.__msg_first;
        buf->__msg_last    = newbuf.__msg_first;
        buf->__msg_cbytes  = newbuf.__msg_cbytes;
        buf->msg_qnum      = newbuf.msg_qnum;
        buf->msg_qbytes    = newbuf.msg_qbytes;
        buf->msg_lspid     = newbuf.msg_lspid;
        buf->msg_lrpid     = newbuf.msg_lrpid;
        buf->msg_stime     = newbuf.msg_stime;
        buf->msg_rtime     = newbuf.msg_rtime;
        buf->msg_ctime     = newbuf.msg_ctime;   
    }
        
    return rv;
}
compat_symbol (libc, __old_msgctl, msgctl, GLIBC_2_0);
#endif
