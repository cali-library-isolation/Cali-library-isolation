/* Copyright (C) 2002, 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Bruno Haible <bruno@clisp.org>.

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

#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <errno.h>
#include <string.h>

#define SYSNAME                "GNU/kFreeBSD"
#define SYSNAME_LEN    13

/* Check for bounds in pre-processor */
#if SYSNAME_LEN > _UTSNAME_SYSNAME_LENGTH
#error
#endif

/* Put information about the system in NAME.  */
int
__uname (struct utsname *name)
{
  int request[2] = { CTL_KERN };
  size_t len;

  /* Fill sysname: "uname -s". */
  strcpy (name->sysname, SYSNAME);

  /* Fill nodename: "uname -n".  Fetch sysctl "kern.hostname".  */
  request[1] = KERN_HOSTNAME;
  len = sizeof (name->nodename);
  if (__sysctl (request, 2, name->nodename, &len, NULL, 0) >= 0)
    {
      if (len < sizeof (name->nodename))
	name->nodename[len] = '\0';
    }

  /* Fill release: "uname -r".  Fetch sysctl "kern.osrelease".  */
  request[1] = KERN_OSRELEASE;
  len = sizeof (name->release);
  if (__sysctl (request, 2, name->release, &len, NULL, 0) >= 0)
    {
      if (len < sizeof (name->release))
        name->release[len] = '\0';
    }

  /* Fill version: "uname -v".  Fetch sysctl "kern.version".  */
  request[1] = KERN_VERSION;
  len = sizeof (name->version);
  if (__sysctl (request, 2, name->version, &len, NULL, 0) >= 0)
    {
      if (len < sizeof (name->version))
        name->version[len] = '\0';
    }

  /* Remove trailing whitespace.  Turn non-trailing whitespace to
     spaces.  */
  {
    char *p0 = name->version;
    char *p = p0 + __strnlen (p0, sizeof (name->version));

    while (p > p0 && (p[-1] == '\t' || p[-1] == '\n' || p[-1] == ' '))
      *--p = '\0';

    while (p > p0)
      {
        --p;
        if (*p == '\t' || *p == '\n')
          *p = ' ';
      }
  }

#ifdef __x86_64__
  /* Check for bounds in pre-processor */
# if 7 > _UTSNAME_MACHINE_LENGTH
#  error
# endif
  /* Pristine FreeBSD kernel would return "amd64".  Avoid that.  */
  strcpy (name->machine, "x86_64");
#else
  /* Fill machine: "uname -m".  Fetch sysctl "hw.machine".  */
  request[0] = CTL_HW;
  request[1] = HW_MACHINE;
  len = sizeof (name->machine);
  if (__sysctl (request, 2, name->machine, &len, NULL, 0) >= 0)
    {
      if (len < sizeof (name->machine))
        name->machine[len] = '\0';
    }
#endif

  return 0;
}
libc_hidden_def (__uname)
weak_alias (__uname, uname)
libc_hidden_def (uname)
