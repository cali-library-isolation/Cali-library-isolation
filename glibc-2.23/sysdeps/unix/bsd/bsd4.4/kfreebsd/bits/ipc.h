/* Copyright (C) 1995-1997, 1999, 2002 Free Software Foundation, Inc.
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

#ifndef _SYS_IPC_H
# error "Never use <bits/ipc.h> directly; include <sys/ipc.h> instead."
#endif

#include <bits/types.h>

/* Mode bits for `msgget', `semget', and `shmget'.  */
#define IPC_CREAT	01000		/* create key if key does not exist */
#define IPC_EXCL	02000		/* fail if key exists */
#define IPC_NOWAIT	04000		/* return error on wait */

/* Control commands for `msgctl', `semctl', and `shmctl'.  */
#define IPC_RMID	0		/* remove identifier */
#define IPC_SET		1		/* set `ipc_perm' options */
#define IPC_STAT	2		/* get `ipc_perm' options */

/* Special key values.  */
#define IPC_PRIVATE	((__key_t) 0)	/* private key */

#ifdef __USE_MISC
/* Common mode bits.  */
# define IPC_R		0400		/* read permission, same as S_IRUSR */
# define IPC_W		0200		/* write permission, same as S_IWUSR */
# define IPC_M		0x1000		/* control permission */
#endif


/* Data structure used to pass permission information to IPC operations.  */
struct ipc_perm
{
    __uid_t		cuid;	/* creator user id */
    __gid_t		cgid;	/* creator group id */
    __uid_t		uid;	/* user id */
    __gid_t		gid;	/* group id */
    __mode_t		mode;	/* r/w permission */
    __uint16_t		__seq;	/* sequence # (to generate unique ipcid) */
    __key_t		__key;	/* user specified msg/sem/shm key */
};
