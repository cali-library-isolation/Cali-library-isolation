/* __sig_atomic_t, __sigset_t, and related definitions.  FreeBSD version.
   Copyright (C) 1994-1996, 2002 Free Software Foundation, Inc.
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

#ifndef	_SIGSET_H_types
# define _SIGSET_H_types	1

typedef int __sig_atomic_t;

/* A `sigset_t' has a bit for each signal.  */
__extension__ typedef struct
  {
    __extension__ union
      {
        unsigned int __sigbits[4];
        unsigned int __bits[4];
      };
  } __sigset_t;

#endif


/* We only want to define these functions if <signal.h> was actually
   included; otherwise we were included just to define the types.  Since we
   are namespace-clean, it wouldn't hurt to define extra macros.  But
   trouble can be caused by functions being defined (e.g., any global
   register vars declared later will cause compilation errors).  */

#if !defined _SIGSET_H_fns && defined _SIGNAL_H
# define _SIGSET_H_fns 1

# ifndef _EXTERN_INLINE
#  define _EXTERN_INLINE __extern_inline
# endif

/* Return a mask that includes the bit for SIG only.  */
# define __sigmask(sig)	((unsigned int) 1 << ((sig) - 1) % 32)

/* Return the word index for SIG.  */
# define __sigword(sig)	(((sig) - 1) >> 5)

# if defined __GNUC__ && __GNUC__ >= 2
#  define __sigemptyset(set) \
  (__extension__ ({ sigset_t *__set = (set);				      \
		    __set->__sigbits[0] = 0; __set->__sigbits[1] = 0;	      \
		    __set->__sigbits[2] = 0; __set->__sigbits[3] = 0;	      \
		    0; }))
#  define __sigfillset(set) \
  (__extension__ ({ sigset_t *__set = (set);				      \
		    __set->__sigbits[0] = ~0; __set->__sigbits[1] = ~0;	      \
		    __set->__sigbits[2] = ~0; __set->__sigbits[3] = ~0;	      \
		    0; }))

#  ifdef __USE_GNU
/* The POSIX does not specify for handling the whole signal set in one
   command.  This is often wanted and so we define three more functions
   here.  */
#   define __sigisemptyset(set) \
  (__extension__ ({ const sigset_t *__set = (set);			      \
		    __set->__sigbits[0] == 0				      \
		    && __set->__sigbits[1] == 0				      \
		    && __set->__sigbits[2] == 0				      \
		    && __set->__sigbits[3] == 0; }))
#   define __sigandset(dest, left, right) \
  (__extension__ ({ sigset_t *__dest = (dest);				      \
		    const sigset_t *__left = (left);			      \
		    const sigset_t *__right = (right);			      \
		    __dest->__sigbits[0] =				      \
		      __left->__sigbits[0] & __right->__sigbits[0];	      \
		    __dest->__sigbits[1] =				      \
		      __left->__sigbits[1] & __right->__sigbits[1];	      \
		    __dest->__sigbits[2] =				      \
		      __left->__sigbits[2] & __right->__sigbits[2];	      \
		    __dest->__sigbits[3] =				      \
		      __left->__sigbits[3] & __right->__sigbits[3];	      \
		    0; }))
#   define __sigorset(dest, left, right) \
  (__extension__ ({ sigset_t *__dest = (dest);				      \
		    const sigset_t *__left = (left);			      \
		    const sigset_t *__right = (right);			      \
		    __dest->__sigbits[0] =				      \
		      __left->__sigbits[0] | __right->__sigbits[0];	      \
		    __dest->__sigbits[1] =				      \
		      __left->__sigbits[1] | __right->__sigbits[1];	      \
		    __dest->__sigbits[2] =				      \
		      __left->__sigbits[2] | __right->__sigbits[2];	      \
		    __dest->__sigbits[3] =				      \
		      __left->__sigbits[3] | __right->__sigbits[3];	      \
		    0; }))
#  endif
# endif

/* These functions needn't check for a bogus signal number -- error
   checking is done in the non __ versions.  */

extern int __sigismember (__const __sigset_t *, int);
extern int __sigaddset (__sigset_t *, int);
extern int __sigdelset (__sigset_t *, int);

# ifdef __USE_EXTERN_INLINES

_EXTERN_INLINE int
__sigismember (__const __sigset_t *__set, int __sig)
{
  return (__set->__sigbits[__sigword (__sig)] & __sigmask (__sig) ? 1 : 0);
}

_EXTERN_INLINE int
__sigaddset (__sigset_t *__set, int __sig)
{
  __set->__sigbits[__sigword (__sig)] |= __sigmask (__sig);
  return 0;
}

_EXTERN_INLINE int
__sigdelset (__sigset_t *__set, int __sig)
{
  __set->__sigbits[__sigword (__sig)] &= ~__sigmask (__sig);
  return 0;
}

# endif


#endif /* ! _SIGSET_H_fns.  */
