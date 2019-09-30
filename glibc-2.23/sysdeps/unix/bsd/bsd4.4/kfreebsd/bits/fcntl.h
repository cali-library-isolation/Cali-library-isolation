/* O_*, F_*, FD_* bit values for FreeBSD.
   Copyright (C) 1991-1992, 1997-2013 Free Software Foundation, Inc.
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

#ifndef	_FCNTL_H
# error "Never use <bits/fcntl.h> directly; include <fcntl.h> instead."
#endif

#include <sys/types.h>
#include <bits/wordsize.h>
#ifdef __USE_GNU
# include <bits/uio.h>
#endif

/*
 * File status flags: these are used by open(2), fcntl(2).
 * They are also used (indirectly) in the kernel file structure f_flags,
 * which is a superset of the open/fcntl flags.  Open flags and f_flags
 * are inter-convertible using OFLAGS(fflags) and FFLAGS(oflags).
 * Open/fcntl flags begin with O_; kernel-internal flags begin with F.
 */
/* open-only flags */
#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */

/*
 * Kernel encoding of open mode; separate read and write bits that are
 * independently testable: 1 greater than the above.
 */
 
#define	O_NONBLOCK	0x0004		/* no delay */
#define	O_NDELAY	O_NONBLOCK	/* compat */
#define	O_APPEND	0x0008		/* set append mode */

#if defined __USE_MISC
#define	O_SHLOCK	0x0010		/* open with shared file lock */
#define	O_EXLOCK	0x0020		/* open with exclusive file lock */
#endif

#define	O_ASYNC		0x0040		/* signal pgrp when data ready */
#define	O_FSYNC		0x0080		/* synchronous writes */
#define	O_SYNC		O_FSYNC		/* POSIX synonym for O_FSYNC */

#if defined (__USE_XOPEN2K8) || defined (__USE_MISC)
#define	O_NOFOLLOW	0x0100		/* don't follow symlinks */
#endif

#define	O_CREAT		0x0200		/* create if nonexistent */
#define	O_TRUNC		0x0400		/* truncate to zero length */
#define	O_EXCL		0x0800		/* error if already exists */
/* Defined by POSIX 1003.1; BSD default, but must be distinct from O_RDONLY. */
#define	O_NOCTTY	0x8000		/* don't assign controlling terminal */

#if defined __USE_MISC
/* Attempt to bypass buffer cache */
#define	O_DIRECT	0x00010000
enum { O_NOATIME = 0};          /* Do not set atime.  */
#endif

/* Defined by POSIX Extended API Set Part 2 */
#if defined (__USE_XOPEN2K8) || defined (__USE_MISC)
#define	O_DIRECTORY	0x00020000	/* Fail if not directory */
#define	O_EXEC		0x00040000	/* Open for execute only */
#endif

#if defined (__USE_XOPEN2K8) || defined (__USE_MISC)
/* Defined by POSIX 1003.1-2008; BSD default, but reserve for future use. */
#define	O_TTY_INIT	0x00080000	/* Restore default termios attributes */
#define	O_CLOEXEC	0x00100000
#endif

/* For now FreeBSD has synchronisity options for data and read operations.
   We define the symbols here but let them do the same as O_SYNC since
   this is a superset.	*/
#if defined __USE_POSIX199309 || defined __USE_UNIX98
# define O_DSYNC	O_SYNC	/* Synchronize data.  */
# define O_RSYNC	O_SYNC	/* Synchronize read operations.	 */
#endif

/* Since 'off_t' is 64-bit, O_LARGEFILE is a no-op.  */
#define O_LARGEFILE	0

#ifdef __USE_MISC
/* Bits in the file status flags returned by F_GETFL.
   These are all the O_* flags, plus FREAD and FWRITE, which are
   independent bits set by which of O_RDONLY, O_WRONLY, and O_RDWR, was
   given to `open'.  */
#define FREAD		1
#define	FWRITE		2
#endif

#ifdef __USE_ATFILE
# define AT_FDCWD		-100	/* Special value used to indicate
					   the *at functions should use the
					   current working directory. */
# define AT_EACCESS		0x100	/* Test access permitted for
					   effective IDs, not real IDs.  */
# define AT_SYMLINK_NOFOLLOW	0x200	/* Do not follow symbolic links.  */
# define AT_SYMLINK_FOLLOW	0x400	/* Follow symbolic links.  */
# define AT_REMOVEDIR		0x800	/* Remove directory instead of
					   unlinking file.  */
#endif

/*
 * We are out of bits in f_flag (which is a short).  However,
 * the flag bits not set in FMASK are only meaningful in the
 * initial open syscall.  Those bits can thus be given a
 * different meaning for fcntl(2).
 */
#ifdef __USE_MISC
/*
 * Set by shm_open(3) to get automatic MAP_ASYNC behavior
 * for POSIX shared memory objects (which are otherwise
 * implemented as plain files).
 */
#define FRDAHEAD        O_CREAT
#define FPOSIXSHM	O_NOFOLLOW
#endif

/* Values for the second argument to `fcntl'.  */
#define F_DUPFD		0	/* Duplicate file descriptor.  */
#define F_GETFD		1	/* Get file descriptor flags.  */
#define F_SETFD		2	/* Set file descriptor flags.  */
#define F_GETFL		3	/* Get file status flags.  */
#define F_SETFL		4	/* Set file status flags.  */
#define F_GETLK		11	/* Get record locking info.  */
#define F_SETLK		12	/* Set record locking info (non-blocking).  */
#define F_SETLKW	13	/* Set record locking info (blocking).	*/
/* Not necessary, we always have 64-bit offsets.  */
#define F_GETLK64	11	/* Get record locking info.  */
#define F_SETLK64	12	/* Set record locking info (non-blocking).  */
#define F_SETLKW64	13	/* Set record locking info (blocking).	*/

#ifdef __USE_XOPEN2K8
#define	F_DUPFD_CLOEXEC	17	/* Like F_DUPFD, but FD_CLOEXEC is set */
#endif
#if defined __USE_MISC
#define F_DUP2FD_CLOEXEC 18	/* Like F_DUP2FD, but FD_CLOEXEC is set */
#endif

#if defined __USE_UNIX98 || defined __USE_XOPEN2K8
# define F_GETOWN	5	/* Get owner of socket (receiver of SIGIO).  */
# define F_SETOWN	6	/* Set owner of socket (receiver of SIGIO).  */
#endif

/* For F_[GET|SET]FD.  */
#define FD_CLOEXEC	1	/* actually anything with low bit set goes */

/* record locking flags (F_GETLK, F_SETLK, F_SETLKW) */
#define	F_RDLCK		1		/* shared or read lock */
#define	F_UNLCK		2		/* unlock */
#define	F_WRLCK		3		/* exclusive or write lock */
#if defined __USE_MISC
#define	F_UNLCKSYS	4		/* purge locks for a given system ID */ 
#define	F_CANCEL	5		/* cancel an async lock request */
#endif

#ifdef __USE_MISC
/* Operations for bsd flock(), also used by the kernel implementation.	*/
# define LOCK_SH	1	/* shared lock */
# define LOCK_EX	2	/* exclusive lock */
# define LOCK_NB	4	/* or'd with one of the above to prevent
				   blocking */
# define LOCK_UN	8	/* remove lock */
#endif

struct flock
  {
    __off_t l_start;	/* Offset where the lock begins.  */
    __off_t l_len;	/* Size of the locked area; zero means until EOF.  */
    __pid_t l_pid;	/* Process holding the lock.  */
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
    int	__l_sysid;	/* remote system id or zero for local */
  };

#ifdef __USE_LARGEFILE64
struct flock64
  {
    __off64_t l_start;	/* Offset where the lock begins.  */
    __off64_t l_len;	/* Size of the locked area; zero means until EOF.  */
    __pid_t l_pid;	/* Process holding the lock.  */
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
    int	__l_sysid;	/* remote system id or zero for local */
  };
#endif

/* Define some more compatibility macros to be backward compatible with
   BSD systems which did not managed to hide these kernel macros.  */
#ifdef	__USE_MISC
# define FAPPEND	O_APPEND
# define FFSYNC		O_FSYNC
# define FASYNC		O_ASYNC
# define FNONBLOCK	O_NONBLOCK
# define FNDELAY	O_NDELAY

#define FCREAT		O_CREAT
#define FEXCL		O_EXCL
#define FTRUNC		O_TRUNC
#define FNOCTTY		O_NOCTTY
#define FSYNC		O_SYNC
#endif /* Use BSD.  */


#ifdef __USE_XOPEN2K
/*
 * Advice to posix_fadvise
 */
#define	POSIX_FADV_NORMAL	0	/* no special treatment */
#define	POSIX_FADV_RANDOM	1	/* expect random page references */
#define	POSIX_FADV_SEQUENTIAL	2	/* expect sequential page references */
#define	POSIX_FADV_WILLNEED	3	/* will need these pages */
#define	POSIX_FADV_DONTNEED	4	/* dont need these pages */
#define	POSIX_FADV_NOREUSE	5	/* access data only once */
#endif
