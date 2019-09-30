/* Copyright (C) 1995, 1997, 2000, 2002 Free Software Foundation, Inc.
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

#ifndef _SYS_MSG_H
#error "Never use <bits/msq.h> directly; include <sys/msg.h> instead."
#endif

#include <bits/types.h>

/* Define options for message queue functions.  */
#define MSG_NOERROR	010000	/* no error if message is too big */

/* Types used in the structure definition.  */
typedef unsigned long int msgqnum_t;
typedef unsigned long int msglen_t;


/* Structure of record for one message inside the kernel.
   The type `struct __msg' is opaque.  */
struct msqid_ds
{
	struct	ipc_perm msg_perm;	/* msg queue permission bits */
	void *__msg_first;		/* first message in the queue */
	void *__msg_last;		/* last message in the queue */
	msglen_t __msg_cbytes;	/* number of bytes in use on the queue */
	msgqnum_t msg_qnum;	/* number of msgs in the queue */
	msglen_t msg_qbytes;	/* max # of bytes on the queue */
	__pid_t	msg_lspid;	/* pid of last msgsnd() */
	__pid_t	msg_lrpid;	/* pid of last msgrcv() */
	__time_t msg_stime;	/* time of last msgsnd() */
	__time_t msg_rtime;	/* time of last msgrcv() */
	__time_t msg_ctime;	/* time of last msgctl() */
};

#ifdef __USE_MISC

# define msg_cbytes	__msg_cbytes

/* buffer for msgctl calls IPC_INFO, MSG_INFO */
struct msginfo
  {
    int msgmax;
    int msgmni;
    int msgmnb;
    int msgtql;
    int msgssz;
    int msgseg;
  };

#endif /* __USE_MISC */
