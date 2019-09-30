/* Convert between different 'struct stat' formats.
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>, 2002.

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

#include <string.h>
#include <sys/mount.h>
#include <bits/stat16.h>

extern int __syscall_fhstat (const fhandle_t *fhp, struct stat16 *buf);
extern int __syscall_fstat (int, struct stat16 *);
extern int __syscall_lstat (const char *, struct stat16 *);
extern int __syscall_stat (const char *, struct stat16 *);
libc_hidden_proto (__syscall_fhstat)
libc_hidden_proto (__syscall_fstat)
libc_hidden_proto (__syscall_lstat)
libc_hidden_proto (__syscall_stat)

/* Convert a 'struct stat16' to 'struct stat'.  */
static inline void
stat16_to_stat (const struct stat16 *p16, struct stat *q)
{
  q->st_dev = p16->st_dev;
  q->st_ino = p16->st_ino;
  q->st_mode = p16->st_mode;
  q->__pad_mode = 0;
  q->st_nlink = p16->st_nlink;
  q->__pad_nlink = 0;
  q->st_uid = p16->st_uid;
  q->st_gid = p16->st_gid;
  q->st_rdev = p16->st_rdev;
  q->st_atim = p16->st_atimespec;
  q->st_mtim = p16->st_mtimespec;
  q->st_ctim = p16->st_ctimespec;
  q->st_size = p16->st_size;
  q->st_blocks = p16->st_blocks;
  q->st_blksize = p16->st_blksize;
  q->st_flags = p16->st_flags;
  q->st_gen = p16->st_gen;
#if 0
  memcpy (q->__unused1, p16->__unused2, sizeof (p16->__unused2));
#endif
}

/* Convert a 'struct stat16' to 'struct stat64'.  */
static inline void
stat16_to_stat64 (const struct stat16 *p16, struct stat64 *q)
{
  q->st_dev = p16->st_dev;
  q->st_ino = p16->st_ino;
  q->st_mode = p16->st_mode;
  q->__pad_mode = 0;
  q->st_nlink = p16->st_nlink;
  q->__pad_nlink = 0;
  q->st_uid = p16->st_uid;
  q->st_gid = p16->st_gid;
  q->st_rdev = p16->st_rdev;
  q->st_atim = p16->st_atimespec;
  q->st_mtim = p16->st_mtimespec;
  q->st_ctim = p16->st_ctimespec;
  q->st_size = p16->st_size;
  q->st_blocks = p16->st_blocks;
  q->st_blksize = p16->st_blksize;
  q->st_flags = p16->st_flags;
  q->st_gen = p16->st_gen;
#if 0
  memcpy (q->__unused1, p16->__unused2, sizeof (p16->__unused2));
#endif
}
