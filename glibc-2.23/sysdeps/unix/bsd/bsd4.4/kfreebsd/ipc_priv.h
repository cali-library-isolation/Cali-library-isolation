/* 
   Copyright (C) 2013 Free Software Foundation, Inc.
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

struct ipc_perm_old
  {
    __uint16_t /* yuck! */ cuid;	/* creator's user ID */
    __uint16_t /* yuck! */ cgid;	/* creator's group ID */
    __uint16_t /* yuck! */ uid;		/* owner's user ID */
    __uint16_t /* yuck! */ gid;		/* owner's group ID */
    __mode_t mode;			/* read/write permission */
    __uint16_t __seq;
    __key_t __key;
  };

static inline void
ipc_perm_old2new(const struct ipc_perm_old *in, struct ipc_perm *out)
{
  out->cuid  = in->cuid;
  out->cgid  = in->cgid;
  out->uid   = in->uid;
  out->gid   = in->gid;
  out->mode  = in->mode;
  out->__seq = in->__seq;
  out->__key = in->__key;
}

static inline void
ipc_perm_new2old(const struct ipc_perm *in, struct ipc_perm_old *out)
{
  out->cuid  = in->cuid;
  out->cgid  = in->cgid;
  out->uid   = in->uid;
  out->gid   = in->gid;
  out->mode  = in->mode;
  out->__seq = in->__seq;
  out->__key = in->__key;
}
