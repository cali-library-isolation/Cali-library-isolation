/* pre-.hidden libgcc compatibility
   Copyright (C) 2002 Free Software Foundation, Inc.
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


#include <stdint.h>
#include <shlib-compat.h>

#if SHLIB_COMPAT(libc, GLIBC_2_2, GLIBC_2_2_6)

typedef int int128_t __attribute__((__mode__(TI)));

extern long double __divtf3 (long double, long double) attribute_hidden;
long double __divtf3_internal (long double x, long double y)
{
  return __divtf3 (x, y);
}
symbol_version (__divtf3_internal, __divtf3, GLIBC_2.2);

extern double __divdf3 (double, double) attribute_hidden;
double __divdf3_internal (double x, double y)
{
  return __divdf3 (x, y);
}
symbol_version (__divdf3_internal, __divdf3, GLIBC_2.2);

extern float __divsf3 (float, float) attribute_hidden;
float __divsf3_internal (float x, float y)
{
  return __divsf3 (x, y);
}
symbol_version (__divsf3_internal, __divsf3, GLIBC_2.2);

extern int64_t __divdi3 (int64_t, int64_t) attribute_hidden;
int64_t __divdi3_internal (int64_t x, int64_t y)
{
  return __divdi3 (x, y);
}
symbol_version (__divdi3_internal, __divdi3, GLIBC_2.2);

extern int64_t __moddi3 (int64_t, int64_t) attribute_hidden;
int64_t __moddi3_internal (int64_t x, int64_t y)
{
  return __moddi3 (x, y);
}
symbol_version (__moddi3_internal, __moddi3, GLIBC_2.2);

extern uint64_t __udivdi3 (uint64_t, uint64_t) attribute_hidden;
uint64_t __udivdi3_internal (uint64_t x, uint64_t y)
{
  return __udivdi3 (x, y);
}
symbol_version (__udivdi3_internal, __udivdi3, GLIBC_2.2);

extern uint64_t __umoddi3 (uint64_t, uint64_t) attribute_hidden;
uint64_t __umoddi3_internal (uint64_t x, uint64_t y)
{
  return __umoddi3 (x, y);
}
symbol_version (__umoddi3_internal, __umoddi3, GLIBC_2.2);

extern int128_t __multi3 (int128_t, int128_t) attribute_hidden;
int128_t __multi3_internal (int128_t x, int128_t y)
{
  return __multi3 (x, y);
}
symbol_version (__multi3_internal, __multi3, GLIBC_2.2);

extern int64_t __divsi3 (int64_t, int64_t) attribute_hidden;
int64_t __divsi3_internal (int64_t x, int64_t y)
{
  return __divsi3 (x, y);
}
symbol_version (__divsi3_internal, __divsi3, GLIBC_2.2);

extern int64_t __modsi3 (int64_t, int64_t) attribute_hidden;
int64_t __modsi3_internal (int64_t x, int64_t y)
{
  return __modsi3 (x, y);
}
symbol_version (__modsi3_internal, __modsi3, GLIBC_2.2);

extern uint64_t __udivsi3 (uint64_t, uint64_t) attribute_hidden;
uint64_t __udivsi3_internal (uint64_t x, uint64_t y)
{
  return __udivsi3 (x, y);
}
symbol_version (__udivsi3_internal, __udivsi3, GLIBC_2.2);

extern uint64_t __umodsi3 (uint64_t, uint64_t) attribute_hidden;
uint64_t __umodsi3_internal (uint64_t x, uint64_t y)
{
  return __umodsi3 (x, y);
}
symbol_version (__umodsi3_internal, __umodsi3, GLIBC_2.2);

#endif
