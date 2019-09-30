/* pre-.hidden libgcc compatibility
   Copyright (C) 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by GOTO Masanori <gotom@debian.org>

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

extern int64_t __ashldi3 (int64_t, int32_t);
int64_t __ashldi3_internal (int64_t u, int32_t b)
{
  return __ashldi3 (u, b);
}
symbol_version (__ashldi3_internal, __ashldi3, GLIBC_2.0);

extern int64_t __ashrdi3 (int64_t, int32_t);
int64_t __ashrdi3_internal (int64_t u, int32_t b)
{
  return __ashrdi3 (u, b);
}
symbol_version (__ashrdi3_internal, __ashrdi3, GLIBC_2.0);

extern int64_t __fixdfdi (double);
int64_t __fixdfdi_internal (double d)
{
  return __fixdfdi (d);
}
symbol_version (__fixdfdi_internal, __fixdfdi, GLIBC_2.0);

extern int64_t __fixsfdi (float);
int64_t __fixsfdi_internal (float d)
{
  return __fixsfdi (d);
}
symbol_version (__fixsfdi_internal, __fixsfdi, GLIBC_2.0);

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
symbol_version (__fixunssfdi_internal, __fixunssfdi, GLIBC_2.0);

extern double __floatdidf (int64_t);
double __floatdidf_internal (int64_t u)
{
  return __floatdidf (u);
}
symbol_version (__floatdidf_internal, __floatdidf, GLIBC_2.0);

extern float __floatdisf (int64_t);
float __floatdisf_internal (int64_t u)
{
  return __floatdisf (u);
}
symbol_version (__floatdisf_internal, __floatdisf, GLIBC_2.0);

#endif
