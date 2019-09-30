/* pre-.hidden libgcc compatibility
   Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Guido Guenther <agx@sigxcpu.org>

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


#include <stdint.h>
#include <shlib-compat.h>

#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_2_6)

extern int32_t __cmpdi2 (int64_t, int64_t);
int32_t __cmpdi2_internal (int64_t u, int64_t v)
{
  return __cmpdi2 (u, v);
}
symbol_version (__cmpdi2_internal, __cmpdi2, GLIBC_2.0);


extern int32_t __ucmpdi2 (int64_t, int64_t);
int32_t __ucmpdi2_internal (int64_t u, int64_t v)
{
  return __ucmpdi2 (u, v);
}
symbol_version (__ucmpdi2_internal, __ucmpdi2, GLIBC_2.0);


extern int64_t __fixunsdfdi (double);
int64_t __fixunsdfdi_internal (double d)
{
	  return __fixunsdfdi (d);
}
symbol_version (__fixunsdfdi_internal, __fixunsdfdi, GLIBC_2.0);

extern int64_t __fixunssfdi (float);
int64_t __fixunssfdi_internal (float d)
{
  return __fixunssfdi (d);
}

symbol_version (__fixunsfdi_internal, __fixunsfdi, GLIBC_2.0);

typedef float XFtype __attribute__ ((mode (XF)));

extern int64_t __fixunsxfdi (XFtype);
int64_t __fixunsxfdi_internal (XFtype d)
{
  return __fixunsxfdi (d);
}

symbol_version (__fixunsxfdi_internal, __fixunsxfdi, GLIBC_2.0);

extern double __floatdidf (int64_t);
double __floatdidf_internal (int64_t u)
{
	  return __floatdidf (u);
}
symbol_version (__floatdidf_internal, __floatdidf, GLIBC_2.0);

#endif
