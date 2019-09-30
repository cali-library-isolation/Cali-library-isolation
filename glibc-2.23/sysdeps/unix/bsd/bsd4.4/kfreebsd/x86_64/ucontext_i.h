/* Offsets  needed in the makecontext() function implementation.
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

/* Offsets of the fields in the ucontext_t structure. */

#define oRDI		24
#define oRSI		32
#define oRDX		40
#define oRCX		48
#define oR8		56
#define oR9		64

#define oRAX		72
#define oRBX		80
#define oRBP		88

#define oRIP		176
#define oRSP		200

#define oLINK		816
#define oSS_SP		824
#define oSS_SIZE	832
