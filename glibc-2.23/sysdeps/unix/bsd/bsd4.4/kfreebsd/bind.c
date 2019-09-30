/* Copyright (C) 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Aurelien Jarno <aurelien@aurel32.net>, 2005.

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

#include <sys/socket.h>
#include <sysdep.h>

/* According to POSIX.1-2004 the len argument specifies the length of
   the sockaddr structure pointed to by the addrarg argument. However
   the FreeBSD kernel waits the actual length of the address stored
   there. The code below emulate this behaviour. */

extern int __libc_sa_len (sa_family_t __af);
libc_hidden_proto (__libc_sa_len)

extern int __syscall_bind (int fd, __CONST_SOCKADDR_ARG addr,
			   socklen_t addrlen) __THROW;
libc_hidden_proto (__syscall_bind)

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.  */

int
__bind (int fd, __CONST_SOCKADDR_ARG addr, socklen_t addrlen)
{
  socklen_t new_addrlen;

  new_addrlen = __libc_sa_len ((addr.__sockaddr__)->sa_family);

  /* Only allow a smaller size, otherwise it could lead to
    stack corruption */
  if ((new_addrlen != 0) && (new_addrlen < addrlen))
    addrlen = new_addrlen;

  /* We pass 3 arguments.  */
  return INLINE_SYSCALL (bind, 3, fd, addr.__sockaddr__, addrlen);
}

weak_alias (__bind, bind)
