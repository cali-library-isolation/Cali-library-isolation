/* Copyright (C) 1999, 2002 Free Software Foundation, Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <ifreq.h>

/* The FreeBSD ioctl SIOCGIFCONF returns the list if interfaces as a
   concatenation of records of different size, each having at least
   sizeof (struct ifreq) bytes.  */


void
__ifreq (struct ifreq **ifreqs, int *num_ifs, int sockfd)
{
  int fd = sockfd;
  struct ifconf ifc;
  int rq_len;
  int nifs;
  char *ifstart;
  char *ifend;
  struct ifreq *ifr;
  /* FreeBSD has many interfaces, many of them are usually down.  */
# define RQ_IFS	16
  /* We have to assume all records are of limited size, so that we know
     when we can stop enlarging the buffer.  */
# define RQ_MAXSIZE 256

  if (fd < 0)
    fd = __opensock ();
  if (fd < 0)
    {
      *num_ifs = 0;
      *ifreqs = NULL;
      return;
    }

  ifc.ifc_buf = NULL;
  rq_len = RQ_IFS * sizeof (struct ifreq) + RQ_MAXSIZE;
  for (;;)
    {
      ifc.ifc_len = rq_len;
      ifc.ifc_buf = realloc (ifc.ifc_buf, ifc.ifc_len);
      if (ifc.ifc_buf == NULL || __ioctl (fd, SIOCGIFCONF, &ifc) < 0)
	{
	  if (ifc.ifc_buf)
	    free (ifc.ifc_buf);

	  if (fd != sockfd)
	    __close (fd);
	  *num_ifs = 0;
	  *ifreqs = NULL;
	  return;
	}
      if (ifc.ifc_len + RQ_MAXSIZE <= rq_len)
	break;
      rq_len *= 2;
    }

  nifs = 0;
  ifstart = (char *) ifc.ifc_buf;
  ifend = ifstart + ifc.ifc_len;
  for (ifr = (struct ifreq *) ifstart;
       (char *) ifr < ifend;
       ifr = __if_nextreq (ifr))
    nifs++;

  if (fd != sockfd)
    __close (fd);

  *num_ifs = nifs;
  *ifreqs = realloc (ifc.ifc_buf, ifc.ifc_len);
}
