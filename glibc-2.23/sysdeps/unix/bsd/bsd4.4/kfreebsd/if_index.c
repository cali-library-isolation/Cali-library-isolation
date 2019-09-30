/* Copyright (C) 2002 Free Software Foundation, Inc.
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

#include <net/if.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <alloca.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


typedef int (*if_fn) (void *private, unsigned int index, const char *name);

/* Iterate through all present interfaces.
   Call FN once for every interface, returning immediately if FN returns
   a nonzero value.  */
static void
if_iterate (if_fn fn, void *private)
{
  int request[6] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_IFLIST, 0 };
  char *buf;
  size_t bufsize = 512;
  char *bufend;
  char *p;

  /* Call the kernel function sysctl_iflist() in /usr/src/sys/net/rtsock.c.  */
  for (;;)
    {
      buf = alloca (bufsize);
      if (__sysctl (request, 6, buf, &bufsize, NULL, 0) >= 0)
	break;
      if (errno != ENOMEM)
	return;
      bufsize *= 2;
    }

  bufend = buf + bufsize;
  for (p = buf; p < bufend; )
    {
      struct if_msghdr *msg = (struct if_msghdr *) p;

      if (msg->ifm_version != RTM_VERSION)
	abort ();

      switch (msg->ifm_type)
	{
	case RTM_IFINFO:
	  if (msg->ifm_addrs & RTA_IFP)
	    {
	      unsigned int index;
	      struct sockaddr_dl *sdl;
	      char namebuf[IFNAMSIZ + 1];
	      size_t namelen;

	      index = msg->ifm_index;
	      if (index == 0)
		abort ();

	      /* FIXME: 'struct if_msghdr' contains a 'struct if_data' which in turns
	         contains 'unsigned long' values. Their size therefore depends on
		 the running kernel (32 or 64 bits). This should be fixed in the
		 compat layer of the kernel. Meanwhile just workaround the bug here/ */
#if 0
	      sdl = (struct sockaddr_dl *) (msg + 1);
#else
	      sdl = (struct sockaddr_dl *) (p + msg->ifm_msglen - sizeof(struct sockaddr_dl) - 2);
#endif
	      namelen = sdl->sdl_nlen;
	      /* Avoid overflowing namebuf[].  */
	      if (namelen > IFNAMSIZ)
		namelen = IFNAMSIZ;
	      memcpy (namebuf, sdl->sdl_data, namelen);
	      namebuf[namelen] = '\0';

	      /* Call FN now.  */
	      if (fn (private, index, namebuf))
		return;
	    }
	  break;

	case RTM_NEWADDR:
	  break;

	default:
	  abort ();
	}

      p += msg->ifm_msglen;
    }
}

/* ------------------------------------------------------------------------- */

struct nametoindex_locals
  {
    const char *name;
    unsigned int index;
  };

static int
nametoindex_aux (void *private, unsigned int index, const char *name)
{
  struct nametoindex_locals *l = (struct nametoindex_locals *) private;
  if (strcmp (name, l->name) == 0)
    {
      l->index = index;
      return 1;
    }
  return 0;
}

/* Return the index of an interface given by name.  */
unsigned int
__if_nametoindex (const char *ifname)
{
  struct nametoindex_locals l;

  l.name = ifname;
  l.index = 0;
  if_iterate (nametoindex_aux, &l);

  return l.index;
}
libc_hidden_def (__if_nametoindex)
weak_alias (__if_nametoindex, if_nametoindex)
libc_hidden_weak (if_nametoindex)

/* ------------------------------------------------------------------------- */

struct indextoname_locals
  {
    unsigned int index;
    char *name;
    char *retval;
  };

static int
indextoname_aux (void *private, unsigned int index, const char *name)
{
  struct indextoname_locals *l = (struct indextoname_locals *) private;
  if (index == l->index)
    {
      strncpy (l->name, name, IF_NAMESIZE);
      l->retval = l->name;
      return 1;
    }
  __set_errno (ENXIO);
  return 0;
}

/* Return the name of an interface given by name.  */
char *
__if_indextoname (unsigned int ifindex, char *ifname)
{
  struct indextoname_locals l;

  l.index = ifindex;
  l.name = ifname;
  l.retval = NULL;
  if_iterate (indextoname_aux, &l);
  return l.retval;
}
weak_alias (__if_indextoname, if_indextoname)
libc_hidden_weak (if_indextoname)

/* ------------------------------------------------------------------------- */

struct nameindex_locals
  {
    /* Resizable array of 'struct if_nameindex'.  */
    struct if_nameindex *s_array;
    size_t s_len;
    size_t s_allocated;
    /* Resizable array of char.  */
    char *c_array;
    size_t c_len;
    size_t c_allocated;
    /* Out-of-memory indicator.  */
    int oom;
  };

static void
add_s (struct nameindex_locals *l, unsigned int index, char *name)
{
  if (l->s_len == l->s_allocated)
    {
      size_t new_allocated = 2 * l->s_allocated + 1;
      struct if_nameindex *new_array =
	(struct if_nameindex *)
	realloc (l->s_array, new_allocated * sizeof (struct if_nameindex));
      if (new_array == NULL)
	{
	  l->oom = 1;
	  return;
	}
      l->s_array = new_array;
      l->s_allocated = new_allocated;
    }
  /* Now l->s_len < l->s_allocated.  */
  l->s_array[l->s_len].if_index = index;
  l->s_array[l->s_len].if_name = name;
  l->s_len++;
}

static __inline size_t
add_c (struct nameindex_locals *l, const char *name)
{
  size_t n = strlen (name) + 1;
  size_t result_offset;
  if (l->c_len + n > l->c_allocated)
    {
      size_t new_allocated =
	(l->c_len + n < 2 * l->c_allocated + 1
	 ? 2 * l->c_allocated + 1
	 : l->c_len + n);
      char *new_array = (char *) realloc (l->c_array, new_allocated);
      if (new_array == NULL)
	{
	  l->oom = 1;
	  return 0;
	}
      l->c_array = new_array;
      l->c_allocated = new_allocated;
    }
  /* Now l->c_len + n <= l->c_allocated.  */
  result_offset = l->c_len;
  memcpy (l->c_array + l->c_len, name, n);
  l->c_len += n;
  return result_offset;
}

static int
nameindex_aux (void *private, unsigned int index, const char *name)
{
  struct nameindex_locals *l = (struct nameindex_locals *) private;

  size_t name_offset = add_c (l, name);
  if (!l->oom)
    {
      add_s (l, index, (char *) NULL + name_offset);
      if (!l->oom)
	return 0;
    }
  return 1;
}

/* Return an array of 'struct if_nameindex', one for each present
   interface.  */
struct if_nameindex *
__if_nameindex (void)
{
  struct nameindex_locals l;

  l.s_array = NULL; l.s_len = 0; l.s_allocated = 0;
  l.c_array = NULL; l.c_len = 0; l.c_allocated = 0;
  l.oom = 0;
  if_iterate (nameindex_aux, &l);
  if (!l.oom)
    {
      /* Convert all offsets to real pointers.  */
      struct if_nameindex *p;
      struct if_nameindex *p_end;

      for (p = l.s_array, p_end = p + l.s_len; p < p_end; p++)
	p->if_name = l.c_array + (p->if_name - (char *) NULL);

      /* Add a terminating entry.  */
      add_s (&l, 0, NULL);
    }
  if (l.oom)
    {
      free (l.s_array);
      free (l.c_array);
      __set_errno (ENOMEM);
      return NULL;
    }
  return l.s_array;
}
weak_alias (__if_nameindex, if_nameindex)
libc_hidden_weak (if_nameindex)

/* ------------------------------------------------------------------------- */

/* Free an array returned by if_nameindex().  */
void
__if_freenameindex (struct if_nameindex *ifn)
{
  if (ifn != NULL)
    {
      /* Free c_array.  */
      free (ifn[0].if_name);
      /* Free s_array.  */
      free (ifn);
    }
}
libc_hidden_def (__if_freenameindex)
weak_alias (__if_freenameindex, if_freenameindex)
libc_hidden_weak (if_freenameindex)
