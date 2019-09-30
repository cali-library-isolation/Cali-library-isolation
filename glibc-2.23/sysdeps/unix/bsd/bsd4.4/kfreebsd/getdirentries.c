/* Read directory entries, 4 argument function.  FreeBSD version.
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

#ifndef GETDIRENTRIES
# define GETDIRENTRIES getdirentries
# define OFF_T off_t
#endif

#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sysdep.h>

#if 1

/* Use the 4-argument system call.  */

extern int __syscall_getdirentries (int fd, char *buf, unsigned int nbytes,
				    long *basep);

/* Read directory entries from FD into BUF, reading at most NBYTES.
   Reading starts at offset *BASEP, and *BASEP is updated with the new
   position after reading.  Returns the number of bytes read; zero when at
   end of directory; or -1 for errors.  */
/* FIXME: This is not what this function does.  It starts reading at the
   current position of FD, not at *BASEP.  */
ssize_t
GETDIRENTRIES (int fd, char *buf, size_t nbytes, OFF_T *basep)
{
  /* On 32-bit platforms, the system call differs from this function because
     it takes a 'long *', not an 'OFF_T *'.  On 64-bit platforms, the system
     call differs from this function because it takes an 'unsigned int', not
     a 'size_t'.  */
  unsigned int nbytes32;

  nbytes32 = nbytes;
  if (nbytes32 == nbytes)
    {
      long base;
      int result = __syscall_getdirentries (fd, buf, nbytes32, &base);

      if (result >= 0 && basep != NULL)
	*basep = base;
      return result;
    }
  else
    {
      /* NBYTES is too large.  */
      __set_errno (EINVAL);
      return -1;
    }
}

#else

/* Use the 3-argument system call.  */

extern int __syscall_getdents (int fd, char *buf, size_t nbytes);

/* Read directory entries from FD into BUF, reading at most NBYTES.
   Reading starts at offset *BASEP, and *BASEP is updated with the new
   position after reading.  Returns the number of bytes read; zero when at
   end of directory; or -1 for errors.  */
/* FIXME: This is not what this function does.  It starts reading at the
   current position of FD, not at *BASEP.  */
ssize_t
GETDIRENTRIES (int fd, char *buf, size_t nbytes, OFF_T *basep)
{
  OFF_T base = 0;
  ssize_t result;

  if (basep)
    base = __lseek (fd, (off_t) 0, SEEK_CUR);

  result = __syscall_getdents (fd, buf, nbytes);

  if (basep && result >= 0)
    *basep = base;
  return result;
}

#endif
