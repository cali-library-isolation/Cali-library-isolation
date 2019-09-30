/* Convert between different 'struct statfs' and 'struct statvfs' formats.
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.
   Contributed by Petr Salinger, 2006.

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


#include <stdint.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/syslimits.h>
#include <sys/statvfs.h>
#include <sys/statfs.h>

/*
 * filesystem statistics
 */

#define MFSNAMELEN	16		/* length of type name including null */
#define MNAMELEN	88		/* size of on/from name bufs */

struct statfs_fbsd5 {
	uint32_t f_version;		/* structure version number */
	uint32_t f_type;		/* type of filesystem */
	uint64_t f_flags;		/* copy of mount exported flags */
	uint64_t f_bsize;		/* filesystem fragment size */
	uint64_t f_iosize;		/* optimal transfer block size */
	uint64_t f_blocks;		/* total data blocks in filesystem */
	uint64_t f_bfree;		/* free blocks in filesystem */
	int64_t	 f_bavail;		/* free blocks avail to non-superuser */
	uint64_t f_files;		/* total file nodes in filesystem */
	int64_t	 f_ffree;		/* free nodes avail to non-superuser */
	uint64_t f_syncwrites;		/* count of sync writes since mount */
	uint64_t f_asyncwrites;		/* count of async writes since mount */
	uint64_t f_syncreads;		/* count of sync reads since mount */
	uint64_t f_asyncreads;		/* count of async reads since mount */
	uint64_t f_spare[10];		/* unused spare */
	uint32_t f_namemax;		/* maximum filename length */
	__uid_t	 f_owner;		/* user that mounted the filesystem */
	__fsid_t f_fsid;		/* filesystem id */
	char	 f_charspare[80];	   /* spare string space */
	char	 f_fstypename[MFSNAMELEN]; /* filesystem type name */
	char	 f_mntfromname[MNAMELEN];  /* mounted filesystem */
	char	 f_mntonname[MNAMELEN];	   /* directory on which mounted */
};

extern int __syscall_getfsstat(struct statfs_fbsd5 *buf, long bufsize, int flags);
extern int __syscall_statfs(const char *path, struct statfs_fbsd5 *buf);
extern int __syscall_fstatfs(int fd, struct statfs_fbsd5 *buf);
extern int __syscall_fhstatfs(const struct fhandle *u_fhp, struct statfs_fbsd5 *buf);


/*
 * Convert a FreeBSD 5.x format statfs structure to an glibc format statfs structure.
 */

static inline void
statfs5_to_statfs(const struct statfs_fbsd5 *pk, struct statfs *p32)
{
  memset(p32, 0, sizeof(*p32));

  p32->f_version	= pk->f_version;
  p32->f_bsize		= MIN(pk->f_bsize,  ULONG_MAX);
  p32->f_iosize		= MIN(pk->f_iosize, ULONG_MAX);

  p32->f_blocks		= MIN(pk->f_blocks, ULONG_MAX);
  p32->f_bfree		= MIN(pk->f_bfree,  ULONG_MAX);
  p32->f_bavail		= MIN(pk->f_bavail, ULONG_MAX);
  p32->f_files		= MIN(pk->f_files,  ULONG_MAX);
  p32->f_ffree		= MIN(pk->f_ffree,  ULONG_MAX);

  p32->f_fsid		= pk->f_fsid;
  p32->f_owner		= pk->f_owner;
  p32->f_type		= pk->f_type;
  p32->f_flags		= pk->f_flags;

  p32->f_syncwrites	= MIN(pk->f_syncwrites,	 ULONG_MAX);
  p32->f_asyncwrites	= MIN(pk->f_asyncwrites, ULONG_MAX);
  p32->f_syncreads	= MIN(pk->f_syncreads,	 ULONG_MAX);
  p32->f_asyncreads	= MIN(pk->f_asyncreads,	 ULONG_MAX);

  p32->f_namemax	= MIN(pk->f_namemax, USHRT_MAX);

  memcpy(p32->f_fstypename, pk->f_fstypename, sizeof (p32->f_fstypename));
  memcpy(p32->f_mntonname,  pk->f_mntonname,  sizeof (p32->f_mntonname));
  memcpy(p32->f_mntfromname,pk->f_mntfromname,sizeof (p32->f_mntfromname));
}


/*
 * Convert a FreeBSD 5.x format statfs structure to an glibc format statfs64 structure.
 */

static inline void
statfs5_to_statfs64(const struct statfs_fbsd5 *pk, struct statfs64 *p64)
{
  memset(p64, 0, sizeof(*p64));

  p64->f_version	= pk->f_version;
  p64->f_bsize		= MIN(pk->f_bsize,  ULONG_MAX);
  p64->f_iosize		= MIN(pk->f_iosize, ULONG_MAX);

  p64->f_blocks		= pk->f_blocks;
  p64->f_bfree		= pk->f_bfree;
  p64->f_bavail		= pk->f_bavail;
  p64->f_files		= pk->f_files;
  p64->f_ffree		= pk->f_ffree;

  p64->f_fsid		= pk->f_fsid;
  p64->f_owner		= pk->f_owner;
  p64->f_type		= pk->f_type;
  p64->f_flags		= pk->f_flags;

  p64->f_syncwrites	= MIN(pk->f_syncwrites,	 ULONG_MAX);
  p64->f_asyncwrites	= MIN(pk->f_asyncwrites, ULONG_MAX);
  p64->f_syncreads	= MIN(pk->f_syncreads,	 ULONG_MAX);
  p64->f_asyncreads	= MIN(pk->f_asyncreads,	 ULONG_MAX);

  p64->f_namemax	= MIN(pk->f_namemax, USHRT_MAX);

  memcpy(p64->f_fstypename, pk->f_fstypename, sizeof (p64->f_fstypename));
  memcpy(p64->f_mntonname,  pk->f_mntonname,  sizeof (p64->f_mntonname));
  memcpy(p64->f_mntfromname,pk->f_mntfromname,sizeof (p64->f_mntfromname));
}


/*
 * Convert a FreeBSD 5.x format statfs structure to an glibc format statvfs structure.
 */

static inline void
statfs5_to_statvfs (const struct statfs_fbsd5 *pk, struct statvfs *p32)
{
  /* FIXME: What is the difference between f_bsize and f_frsize ? */
  p32->f_bsize		= MIN(pk->f_bsize,  ULONG_MAX);
  p32->f_frsize		= MIN(pk->f_bsize,  ULONG_MAX);

  p32->f_blocks		= MIN(pk->f_blocks, ULONG_MAX);
  p32->f_bfree		= MIN(pk->f_bfree,  ULONG_MAX);
  p32->f_bavail		= MIN(pk->f_bavail, ULONG_MAX);
  p32->f_files		= MIN(pk->f_files,  ULONG_MAX);
  p32->f_ffree		= MIN(pk->f_ffree,  ULONG_MAX);
  p32->f_favail		= MIN(pk->f_ffree,  ULONG_MAX); /* Hmm.	 May be filesystem dependent.  */

  memcpy(&(p32->f_fsid), &(pk->f_fsid), sizeof(__fsid_t));

  p32->f_flag		=
      (pk->f_flags & MNT_RDONLY ? ST_RDONLY : 0)
    | (pk->f_flags & MNT_NOSUID ? ST_NOSUID : 0)
    | (pk->f_flags & MNT_NOEXEC ? ST_NOEXEC : 0)
    | (pk->f_flags & MNT_SYNCHRONOUS ? ST_SYNCHRONOUS : 0)
    | (pk->f_flags & MNT_NOATIME ? ST_NOATIME : 0);
  p32->f_namemax	= pk->f_namemax;

  memset (p32->f_spare, '\0', sizeof (p32->f_spare));
}


/*
 * Convert a FreeBSD 5.x format statfs structure to an glibc format statvfs64 structure.
 */

static inline void
statfs5_to_statvfs64 (const struct statfs_fbsd5 *pk, struct statvfs64 *p64)
{
  /* FIXME: What is the difference between f_bsize and f_frsize ? */
  p64->f_bsize		= MIN(pk->f_bsize,  ULONG_MAX);
  p64->f_frsize		= MIN(pk->f_bsize,  ULONG_MAX);

  p64->f_blocks		= pk->f_blocks;
  p64->f_bfree		= pk->f_bfree;
  p64->f_bavail		= pk->f_bavail;
  p64->f_files		= pk->f_files;
  p64->f_ffree		= pk->f_ffree;
  p64->f_favail		= pk->f_ffree; /* Hmm.	May be filesystem dependent.  */

  memcpy(&(p64->f_fsid), &(pk->f_fsid), sizeof(__fsid_t));

  p64->f_flag		=
      (pk->f_flags & MNT_RDONLY ? ST_RDONLY : 0)
    | (pk->f_flags & MNT_NOSUID ? ST_NOSUID : 0)
    | (pk->f_flags & MNT_NOEXEC ? ST_NOEXEC : 0)
    | (pk->f_flags & MNT_SYNCHRONOUS ? ST_SYNCHRONOUS : 0)
    | (pk->f_flags & MNT_NOATIME ? ST_NOATIME : 0);
  p64->f_namemax	= pk->f_namemax;

  memset (p64->f_spare, '\0', sizeof (p64->f_spare));
}
