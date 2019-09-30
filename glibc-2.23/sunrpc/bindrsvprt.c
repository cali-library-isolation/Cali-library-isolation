/*
 * Copyright (c) 2010, Oracle America, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the "Oracle America, Inc." nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libc-lock.h>

#define STARTPORT 600
#define LOWPORT 512
#define ENDPORT (IPPORT_RESERVED - 1)
#define NPORTS	(ENDPORT - STARTPORT + 1)

/*
 * Read the file /etc/rpc.blacklisted, so that we don't bind
 * to this ports.
 */

static int blacklist_read;
static int *list;
static int list_size = 0;

static void
load_blacklist (void)
{
  FILE *fp;
  char *buf = NULL;
  size_t buflen = 0;
  int size = 0, ptr = 0;

  blacklist_read = 1;

  fp = fopen ("/etc/bindresvport.blacklist", "r");
  if (NULL == fp)
    return;

  while (!feof_unlocked (fp))
    {
      unsigned long port;
      char *tmp, *cp;
      ssize_t n = __getline (&buf, &buflen, fp);
      if (n < 1)
        break;

      cp = buf;
      tmp = strchr (cp, '#');  /* remove comments */
      if (tmp)
        *tmp = '\0';
      while (isspace ((int)*cp))    /* remove spaces and tabs */
        ++cp;
      if (*cp == '\0')        /* ignore empty lines */
        continue;
      if (cp[strlen (cp) - 1] == '\n')
        cp[strlen (cp) - 1] = '\0';

      port = strtoul (cp, &tmp, 0);
      while (isspace(*tmp))
        ++tmp;
      if (*tmp != '\0' || (port == ULONG_MAX && errno == ERANGE))
	continue;

      /* Don't bother with out-of-range ports */
      if (port < LOWPORT || port > ENDPORT)
        continue;

      if (ptr >= size)
	{
	  size += 10;
	  list = realloc (list, size * sizeof (int));
	  if (list == NULL)
	    {
	      free (buf);
	      return;
	    }
	}

      list[ptr++] = port;
    }

  fclose (fp);

  if (buf)
    free (buf);

  list_size = ptr;
}

/*
 * Locks the static variables in this file.
 */
__libc_lock_define_initialized (static, lock);

/*
 * Bind a socket to a privileged IP port
 */
int
bindresvport (int sd, struct sockaddr_in *sin)
{
  static short startport = STARTPORT;
  static short port;
  struct sockaddr_in myaddr;
  int i;

  if (!blacklist_read)
    load_blacklist ();

  if (sin == (struct sockaddr_in *) 0)
    {
      sin = &myaddr;
      __bzero (sin, sizeof (*sin));
      sin->sin_family = AF_INET;
    }
  else if (sin->sin_family != AF_INET)
    {
      __set_errno (EAFNOSUPPORT);
      return -1;
    }

  if (port == 0)
    {
      port = (__getpid () % NPORTS) + STARTPORT;
    }

  __set_errno (EADDRINUSE);
  /* Initialize to make gcc happy.  */
  int res = -1;

  int nports = ENDPORT - startport + 1;
  int endport = ENDPORT;

  __libc_lock_lock (lock);

 again:
  for (i = 0; i < nports; ++i)
    {
      int j;

      sin->sin_port = htons (port);

      /* Check, if this port is not blacklisted.  */
      for (j = 0; j < list_size; j++)
	if (port == list[j])
	  goto try_next_port;

      res = __bind (sd, sin, sizeof (struct sockaddr_in));
      if (res >= 0 || errno != EADDRINUSE)
	break;

try_next_port:
      if (++port > endport)
	port = startport;
    }

  if (i == nports && startport != LOWPORT)
    {
      startport = LOWPORT;
      endport = STARTPORT - 1;
      nports = STARTPORT - LOWPORT;
      port = LOWPORT + port % (STARTPORT - LOWPORT);
      goto again;
    }

  __libc_lock_unlock (lock);

  return res;
}
libc_hidden_def (bindresvport)
