/* @(#)s_ldexp.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: s_ldexp.c,v 1.6 1995/05/10 20:47:40 jtc Exp $";
#endif

#include <math.h>
#include <math_private.h>
#include <errno.h>

double __ldexp(double value, int exp)
{
	if(!isfinite(value)||value==0.0) return value;
	value = __scalbn(value,exp);
	if(!isfinite(value)||value==0.0) __set_errno (ERANGE);
	return value;
}
weak_alias (__ldexp, ldexp)
weak_alias (__ldexp, scalbn)
#ifdef NO_LONG_DOUBLE
strong_alias (__ldexp, __ldexpl)
weak_alias (__ldexp, ldexpl)
weak_alias (__ldexp, scalbnl)
#endif
