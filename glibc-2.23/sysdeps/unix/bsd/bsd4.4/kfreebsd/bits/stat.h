/* Copyright (C) 1992, 1996-1997, 2000, 2002 Free Software Foundation, Inc.
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

#if !defined _SYS_STAT_H && !defined _FCNTL_H
# error "Never include <bits/stat.h> directly; use <sys/stat.h> instead."
#endif

#ifndef _BITS_STAT_H
#define _BITS_STAT_H   1

/* This structure needs to be defined in accordance with the
   implementation of __stat, __fstat, and __lstat.  */

#include <bits/types.h>

/* Versions of the 'struct stat' data structure.  */
#define _STAT_VER_ostat	0	/* 'struct ostat' in /usr/src/sys/sys/stat.h */
#define _STAT_VER_stat	1	/* 'struct stat' in /usr/src/sys/sys/stat.h */
#define _STAT_VER_nstat	2	/* 'struct nstat' in /usr/src/sys/sys/stat.h */
#define _STAT_VER_gstat 3	/* glibc's 'struct stat' without LFS */
/* By default we use _STAT_VER_gstat, but we support also _STAT_VER_stat */
#define _STAT_VER	_STAT_VER_gstat

/* Structure describing file characteristics.  */
struct stat
  {
    __dev_t st_dev;		/* Device containing the file.  */
#ifndef __USE_FILE_OFFSET64
    __ino_t st_ino;		/* File serial number.  */
#else
    __ino64_t st_ino;		/* File serial number.	*/
#endif

    __mode_t st_mode;		/* File mode.  */
    __mode_t __pad_mode;	/* __mode_t is 16 bit, fill to 32 bit to retain previous ABI */
    __nlink_t st_nlink;		/* Link count.  */
    __nlink_t __pad_nlink;	/* __nlink_t is 16 bit, fill to 32 bit to retain previous ABI */

    __uid_t st_uid;		/* User ID of the file's owner.  */
    __gid_t st_gid;		/* Group ID of the file's group.  */

    __dev_t st_rdev;		/* Device number, if device.  */

#if defined __USE_MISC || defined __USE_XOPEN2K8
    /* Nanosecond resolution timestamps are stored in a format
       equivalent to 'struct timespec'.  This is the type used
       whenever possible but the Unix namespace rules do not allow the
       identifier 'timespec' to appear in the <sys/stat.h> header.
       Therefore we have to handle the use of this header in strictly
       standard-compliant sources special.  */
    struct timespec st_atim;		/* Time of last access.  */
    struct timespec st_mtim;		/* Time of last modification.  */
    struct timespec st_ctim;		/* Time of last status change.  */
# define st_atime st_atim.tv_sec	/* Backward compatibility.  */
# define st_mtime st_mtim.tv_sec
# define st_ctime st_ctim.tv_sec
#else
    __time_t st_atime;		/* Time of last access.  */
    long int st_atimensec;	/* Nanoseconds of last access.  */
    __time_t st_mtime;		/* Time of last modification.  */
    long int st_mtimensec;	/* Nanoseconds of last modification.  */
    __time_t st_ctime;		/* Time of last status change.  */
    long int st_ctimensec;	/* Nanoseconds of last status change.  */
#endif

    __off_t st_size;		/* Size of file, in bytes.  */

    __blkcnt_t st_blocks;	/* Number of 512-byte blocks allocated.  */

    __blksize_t st_blksize;	/* Optimal block size for I/O.  */
#define _STATBUF_ST_BLKSIZE	/* Tell code we have this member.  */

    __uint32_t st_flags;	/* User defined flags.  */

    __uint32_t st_gen;		/* Generation number.  */

    __quad_t __unused1[2];
  };

#ifdef __USE_LARGEFILE64
struct stat64
  {
    __dev_t st_dev;		/* Device containing the file.  */
    __ino64_t st_ino;		/* File serial number.	*/

    __mode_t st_mode;		/* File mode.  */
    __mode_t __pad_mode;	/* __mode_t is 16 bit, fill to 32 bit to retain previous ABI */
    __nlink_t st_nlink;		/* Link count.  */
    __nlink_t __pad_nlink;	/* __nlink_t is 16 bit, fill to 32 bit to retain previous ABI */

    __uid_t st_uid;		/* User ID of the file's owner.  */
    __gid_t st_gid;		/* Group ID of the file's group.  */

    __dev_t st_rdev;		/* Device number, if device.  */

#if defined __USE_MISC || defined __USE_XOPEN2K8
    /* Nanosecond resolution timestamps are stored in a format
       equivalent to 'struct timespec'.  This is the type used
       whenever possible but the Unix namespace rules do not allow the
       identifier 'timespec' to appear in the <sys/stat.h> header.
       Therefore we have to handle the use of this header in strictly
       standard-compliant sources special.  */
    struct timespec st_atim;		/* Time of last access.  */
    struct timespec st_mtim;		/* Time of last modification.  */
    struct timespec st_ctim;		/* Time of last status change.  */
# define st_atime st_atim.tv_sec	/* Backward compatibility.  */
# define st_mtime st_mtim.tv_sec
# define st_ctime st_ctim.tv_sec
#else
    __time_t st_atime;		/* Time of last access.  */
    long int st_atimensec;	/* Nanoseconds of last access.  */
    __time_t st_mtime;		/* Time of last modification.  */
    long int st_mtimensec;	/* Nanoseconds of last modification.  */
    __time_t st_ctime;		/* Time of last status change.  */
    long int st_ctimensec;	/* Nanoseconds of last status change.  */
#endif

    __off_t st_size;		/* Size of file, in bytes.  */

    __blkcnt_t st_blocks;	/* Number of 512-byte blocks allocated.  */

    __blksize_t st_blksize;	/* Optimal block size for I/O.  */

    __uint32_t st_flags;	/* User defined flags.  */

    __uint32_t st_gen;		/* Generation number.  */

    __quad_t __unused1[2];
  };
#endif

/* Encoding of the file mode.  These are the standard Unix values,
   but POSIX.1 does not specify what values should be used.  */

#define __S_IFMT	0170000	/* These bits determine file type.  */

/* File types.  */
#define __S_IFDIR	0040000	/* Directory.  */
#define __S_IFCHR	0020000	/* Character device.  */
#define __S_IFBLK	0060000	/* Block device.  */
#define __S_IFREG	0100000	/* Regular file.  */
#define __S_IFLNK	0120000	/* Symbolic link.  */
#define __S_IFSOCK	0140000	/* Socket.  */
#define __S_IFWHT	0160000	/* Whiteout.  */
#define __S_IFIFO	0010000	/* FIFO.  */

/* POSIX.1b objects.  */
#define __S_TYPEISMQ(buf) 0
#define __S_TYPEISSEM(buf) 0
#define __S_TYPEISSHM(buf) 0

/* Protection bits.  */

#define __S_ISUID	04000	/* Set user ID on execution.  */
#define __S_ISGID	02000	/* Set group ID on execution.  */
#define __S_ISVTX	01000	/* Save swapped text after use (sticky).  */
#define __S_IREAD	0400	/* Read by owner.  */
#define __S_IWRITE	0200	/* Write by owner.  */
#define __S_IEXEC	0100	/* Execute by owner.  */

#ifdef __USE_MISC

/* Definitions of flags stored in file flags word.  */

/* Super-user and owner changeable flags.  */
# define UF_SETTABLE	0x0000ffff	/* mask of owner changeable flags */
# define UF_NODUMP	0x00000001	/* do not dump file */
# define UF_IMMUTABLE	0x00000002	/* file may not be changed */
# define UF_APPEND	0x00000004	/* writes to file may only append */
# define UF_OPAQUE	0x00000008	/* directory is opaque wrt. union */
# define UF_NOUNLINK	0x00000010	/* file may not be removed or renamed */

/* Super-user changeable flags.  */
# define SF_SETTABLE	0xffff0000	/* mask of superuser changeable flags */
# define SF_ARCHIVED	0x00010000	/* file is archived */
# define SF_IMMUTABLE	0x00020000	/* file may not be changed */
# define SF_APPEND	0x00040000	/* writes to file may only append */
# define SF_NOUNLINK	0x00100000	/* file may not be removed or renamed */
# define SF_SNAPSHOT	0x00200000	/* snapshot inode */

__BEGIN_DECLS

/* Set file flags for FILE to FLAGS.  */
extern int chflags (__const char *__file, unsigned long int __flags) __THROW;

/* Set file flags of the file referred to by FD to FLAGS.  */
extern int fchflags (int __fd, unsigned long int __flags) __THROW;

/* Set file flags for FILE to FLAGS without following symlinks.  */
extern int lchflags(__const char *__file, int __flags);

/* Get device name in /dev with a device number of dev and a file type
   matching the one encoded in type.  */
extern char *devname(__dev_t dev, __mode_t type) __THROW;

/* Store at most BUFLEN characters of the device name in /dev with a 
   device number of dev and a file type matching the one encoded in type.  */
extern char *devname_r(__dev_t dev, __mode_t type, char *buf, int buflen) __THROW;

__END_DECLS

#endif /* __USE_MISC */

#endif /* bits/stat.h */
