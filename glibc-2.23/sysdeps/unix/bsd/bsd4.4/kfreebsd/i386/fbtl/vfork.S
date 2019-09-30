/* Copyright (C) 1999-2013 Free Software Foundation, Inc.
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


#include <tcb-offsets.h>

/* Save the PID value.  */
#define SAVE_PID \
	movl	%gs:PID, %edx; 						      \
	negl	%edx;							      \
	jne	1f;							      \
	movl	$0x80000000, %edx;					      \
1:	movl	%edx, %gs:PID


/* we do not have spare register during syscall */
/* Restore the old PID value in the parent.  */
#define RESTORE_PID_IN_PARENT \
	movl	%gs:PID, %edx; 						      \
	cmpl	$0x80000000, %edx;					      \
	jne	1f;							      \
	xorl	%edx, %edx;						      \
1:	negl	%edx;							      \
	movl	%edx, %gs:PID

#include "../vfork.S"
