/* Copyright (C) 1998, 1999, 2002 Free Software Foundation, Inc.
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

#include <sys/socket.h>

#include <netatalk/at.h>
#include <netinet/in.h>
#include <netipx/ipx.h>
#include <sys/un.h>

extern int __libc_sa_len (sa_family_t __af);
libc_hidden_proto (__libc_sa_len)

int
__libc_sa_len (sa_family_t af)
{
  switch (af)
    {
    case AF_APPLETALK:
      return sizeof (struct sockaddr_at);
    case AF_INET:
      return sizeof (struct sockaddr_in);
    case AF_INET6:
      return sizeof (struct sockaddr_in6);
    case AF_IPX:
      return sizeof (struct sockaddr_ipx);
    case AF_LOCAL:
      return __SOCKADDR_COMMON_SIZE + sizeof(((struct sockaddr_un *) 0)->sun_path);
    }
  return 0;
}
libc_hidden_def (__libc_sa_len)
