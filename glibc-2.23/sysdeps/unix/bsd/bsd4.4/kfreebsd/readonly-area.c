/* Copyright (C) 2004, 2005, 2009 Free Software Foundation, Inc.
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
#include <stdint.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include "libio/libioP.h"

/* Return 1 if the whole area PTR .. PTR+SIZE is not writable.
   Return -1 if it is writable.  */

int
__readonly_area (const char *ptr, size_t size)
{
  const void *ptr_end = ptr + size;

  int mib[4];
  size_t kve_len = 0;
  char *kve_buf, *kve_bufp;

  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_VMMAP;
  mib[3] = __getpid ();

  if (__sysctl (mib, 4, NULL, &kve_len, NULL, 0) != 0)
    {
      __set_errno (ENOSYS);
      return 1;
    }

  kve_buf = alloca (kve_len);
  if (__sysctl (mib, 4, kve_buf, &kve_len, NULL, 0) != 0)
    {
      __set_errno (ENOSYS);
      return 1;
    }

  kve_bufp = kve_buf;
  while (kve_bufp < kve_buf + kve_len)
    {
      struct kinfo_vmentry *kve = (struct kinfo_vmentry *) (uintptr_t) kve_bufp;
      kve_bufp += kve->kve_structsize;

      uintptr_t from = kve->kve_start;
      uintptr_t to = kve->kve_end;

      if (from < (uintptr_t) ptr_end && to > (uintptr_t) ptr)
	{
	  /* Found an entry that at least partially covers the area.  */
	  if (!(kve->kve_protection & KVME_PROT_READ)
	      || (kve->kve_protection & KVME_PROT_WRITE))
	    break;

	  if (from <= (uintptr_t) ptr && to >= (uintptr_t) ptr_end)
	    {
	      size = 0;
	      break;
	    }
	  else if (from <= (uintptr_t) ptr)
	    size -= to - (uintptr_t) ptr;
	  else if (to >= (uintptr_t) ptr_end)
	    size -= (uintptr_t) ptr_end - from;
	  else
	    size -= to - from;

	  if (!size)
	    break;
	}
    }

  /* If the whole area between ptr and ptr_end is covered by read-only
     VMAs, return 1.  Otherwise return -1.  */
  return size == 0 ? 1 : -1;
}
