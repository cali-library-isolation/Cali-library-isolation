/* Copyright (C) 2002, 2006, 2010 Free Software Foundation, Inc.
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

/* This structure corresponds to the standard FreeBSD 'struct stat'
   (i.e. _STAT_VER_stat), and is used by the stat() system call family. */

struct stat16
  {
    __dev_t st_dev;		/* Device containing the file.  */
    __ino_t st_ino;		/* File serial number.  */

    __uint16_t st_mode;		/* File mode.  */
    __uint16_t st_nlink;	/* Link count.  */

    __uid_t st_uid;		/* User ID of the file's owner.  */
    __gid_t st_gid;		/* Group ID of the file's group.  */

    __dev_t st_rdev;		/* Device number, if device.  */

    struct  timespec st_atimespec;  /* time of last access */
    struct  timespec st_mtimespec;  /* time of last data modification */
    struct  timespec st_ctimespec;  /* time of last file status change */

    __off_t st_size;		/* Size of file, in bytes.  */

    __blkcnt_t st_blocks;	/* Number of 512-byte blocks allocated.  */

    __blksize_t st_blksize;	/* Optimal block size for I/O.  */

    __uint32_t st_flags;	/* User defined flags.  */

    __uint32_t st_gen;		/* Generation number.  */

    __uint32_t __unused1;

    __time_t st_birthtime;	/* Time of file creation.  */
    long int st_birthtimensec;	/* Nanoseconds of file creation.  */

#define _BIRTH_PADSIZE     (16 - sizeof(__time_t) - sizeof (long int))
    char __birth_padding[_BIRTH_PADSIZE];
  };
