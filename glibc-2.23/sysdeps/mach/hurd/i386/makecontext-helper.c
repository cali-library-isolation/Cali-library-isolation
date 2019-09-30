/* Helper for makecontext: handle threadvars.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <hurd/threadvar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

#if 0

void
__makecontext_helper (ucontext_t *ucp)
{
  if (__hurd_threadvar_stack_mask == 0)
    {
      /* We are not using threads, so per init-first.c:init, the threadvars
         live in a malloced space, addressed relative to the base of the
         virtual address space.  Just keep using that one.  */
    }
  else
    {
      /* The following is only prepared to work with libpthread, which only
         keeps the threadvars at the bottom of the stack -- contrary to
         libthreads, which also puts additional data there.  */

      void *s = ucp->uc_stack.ss_sp;
      size_t s_size = ucp->uc_stack.ss_size;

      /* Is the new stack suitable?  Check that that the last threadvar
         occupies the last storage unit within the bounds of the new stack.
         Alignment according to (sp & __hurd_threadvar_stack_mask) == sp is not
         actually a requirement (though, in practice it often will be).  */
      if (__hurd_threadvar_location_from_sp (_HURD_THREADVAR_MAX, s)
          != s + s_size)
        {
          /* Instead of having makecontext return an error, we bail out the
             hard way, as we can't expect its caller to be able to properly
             react to this situation.  */
          fprintf (stderr,
                   "*** makecontext: a stack at %p with size %#x is not "
                   "usable with threadvars\n",
                   s, s_size);
          abort ();
        }

      /* Copy the threadvars to the new stack.  */
      void *t_old = __hurd_threadvar_location (0);
      void *t_new = __hurd_threadvar_location_from_sp (0, s);
      size_t t_size = __hurd_threadvar_max * sizeof (unsigned long int);
      memcpy (t_new, t_old, t_size);
      /* Account for the space taken by the threadvars.  */
      ucp->uc_stack.ss_size -= t_size;
    }
}
#endif
