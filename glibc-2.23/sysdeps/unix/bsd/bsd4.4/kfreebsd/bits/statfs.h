/* Definition of `struct statfs', information about a filesystem.
   Copyright (C) 1996-1997, 2002 Free Software Foundation, Inc.
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

#ifndef _BITS_STATFS_H
#define _BITS_STATFS_H	1

#if !defined _SYS_STATFS_H && !defined _SYS_MOUNT_H
# error "Never include <bits/statfs.h> directly; use <sys/statfs.h> instead."
#endif

#include <bits/types.h>

struct statfs
  {
    unsigned long f_version;
    unsigned long f_bsize;
    unsigned long f_iosize;
#ifndef __USE_FILE_OFFSET64
    __fsblkcnt_t f_blocks;
    __fsblkcnt_t f_bfree;
    __fsblkcnt_t f_bavail;
    __fsfilcnt_t f_files;
    __fsfilcnt_t f_ffree;
#else
    __fsblkcnt64_t f_blocks;
    __fsblkcnt64_t f_bfree;
    __fsblkcnt64_t f_bavail;
    __fsfilcnt64_t f_files;
    __fsfilcnt64_t f_ffree;
#endif
    __fsid_t f_fsid;
    __uid_t f_owner;
    int f_type;
    int f_flags;
    unsigned long int f_syncwrites;
    unsigned long int f_asyncwrites;
    char f_fstypename[16];
    char f_mntonname[80];
    unsigned long int f_syncreads;
    unsigned long int f_asyncreads;
    unsigned short f_namemax;
    char f_mntfromname[80];
    short __unused3;
    long __unused4[2];
  };

#ifdef __USE_LARGEFILE64
struct statfs64
  {
    unsigned long f_version;
    unsigned long f_bsize;
    unsigned long f_iosize;
    __fsblkcnt64_t f_blocks;
    __fsblkcnt64_t f_bfree;
    __fsblkcnt64_t f_bavail;
    __fsfilcnt64_t f_files;
    __fsfilcnt64_t f_ffree;
    __fsid_t f_fsid;
    __uid_t f_owner;
    int f_type;
    int f_flags;
    unsigned long int f_syncwrites;
    unsigned long int f_asyncwrites;
    char f_fstypename[16];
    char f_mntonname[80];
    unsigned long int f_syncreads;
    unsigned long int f_asyncreads;
    unsigned short f_namemax;
    char f_mntfromname[80];
    short __unused3;
    long __unused4[2];
  };
#endif

#endif /* _BITS_STATFS_H */
