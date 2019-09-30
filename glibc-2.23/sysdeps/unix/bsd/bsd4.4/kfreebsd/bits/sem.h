/* Copyright (C) 1995-1998, 2002 Free Software Foundation, Inc.
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

#ifndef _SYS_SEM_H
# error "Never include <bits/sem.h> directly; use <sys/sem.h> instead."
#endif

#include <sys/types.h>

/* Flags for `semop'.  */
#define SEM_UNDO	0x1000		/* undo the operation on exit */

/* Commands for `semctl'.  */
#define GETPID		4		/* get sempid */
#define GETVAL		5		/* get semval */
#define GETALL		6		/* get all semval's */
#define GETNCNT		3		/* get semncnt */
#define GETZCNT		7		/* get semzcnt */
#define SETVAL		8		/* set semval */
#define SETALL		9		/* set all semval's */

#ifdef __USE_MISC
# define SEM_R		IPC_R		/* read permission for user */
# define SEM_A		IPC_W		/* alter permission for user */
#endif


/* Data structure describing a set of semaphores.  */
struct semid_ds 
{
    struct ipc_perm	sem_perm;	/* operation permission struct */
    void		*__sem_base;	/* pointer to first semaphore in set */
    unsigned short	sem_nsems;	/* number of sems in set */
    __time_t		sem_otime;	/* last operation time */
    __time_t		sem_ctime;	/* last change time */
    					/* Times measured in secs since */
    					/* 00:00:00 GMT, Jan. 1, 1970 */
};
/* The user should define a union like the following to use it for arguments
   for `semctl'.

   union semun
   {
     int val;				<= value for SETVAL
     struct semid_ds *buf;		<= buffer for IPC_STAT & IPC_SET
     unsigned short int *array;		<= array for GETALL & SETALL
     struct seminfo *__buf;		<= buffer for IPC_INFO
   };

   Previous versions of this file used to define this union but this is
   incorrect.  One can test the macro _SEM_SEMUN_UNDEFINED to see whether
   one must define the union or not.  */
#define _SEM_SEMUN_UNDEFINED	1

#ifdef __USE_MISC

/* ipcs ctl cmds */
# define SEM_STAT 10
# define SEM_INFO 11

/*
 * semaphore info struct
 */
struct seminfo {
        int     semmap,         /* # of entries in semaphore map */
                semmni,         /* # of semaphore identifiers */
                semmns,         /* # of semaphores in system */
                semmnu,         /* # of undo structures in system */
                semmsl,         /* max # of semaphores per id */
                semopm,         /* max # of operations per semop call */
                semume,         /* max # of undo entries per process */
                semusz,         /* size in bytes of undo structure */
                semvmx,         /* semaphore maximum value */
                semaem;         /* adjust on exit max value */
};

#endif

