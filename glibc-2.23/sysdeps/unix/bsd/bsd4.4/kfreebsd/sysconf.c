/* Get file-specific information about a file.
   Copyright (C) 2006 Free Software Foundation, Inc.
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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sysdep.h>
#include <time.h>
#include <unistd.h>
#include <hp-timing.h>
#include <sys/sysctl.h>

static long int posix_sysconf (int name);

/* Get the value of the system variable NAME.  */
long int
__sysconf (int name)
{
  int request[2];
  int value;
  size_t len = sizeof(value);

  switch(name)
    {
      case _SC_CPUTIME:
      case _SC_THREAD_CPUTIME:
#if HP_TIMING_AVAIL
	// XXX We can add  here test for machines which cannot support a
	// XXX usable TSC.
	return 200809L;
#else
	return -1;
#endif
      case _SC_NGROUPS_MAX:
	request[0] = CTL_KERN;
	request[1] = KERN_NGROUPS;
	if (__sysctl(request, 2, &value, &len, NULL, 0) == -1)
	    return NGROUPS_MAX;
	return (long)value;
      case _SC_ARG_MAX:
	request[0] = CTL_KERN;
	request[1] = KERN_ARGMAX;
	if (__sysctl(request, 2, &value, &len, NULL, 0) == -1)
	    return ARG_MAX;
	return (long)value;
    }
  return posix_sysconf (name);
}

/* Now the POSIX version.  */
#undef __sysconf
#define __sysconf static posix_sysconf
#include <sysdeps/posix/sysconf.c>
