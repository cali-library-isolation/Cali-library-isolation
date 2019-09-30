/* Copyright (C) 2013 Free Software Foundation, Inc.
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

/* From <sys/cpuset.h> */


/*
 * Valid cpulevel_t values.
 */
#define	CPU_LEVEL_ROOT		1	/* All system cpus. */
#define	CPU_LEVEL_CPUSET	2	/* Available cpus for which. */
#define	CPU_LEVEL_WHICH		3	/* Actual mask/id for which. */

/*
 * Valid cpuwhich_t values.
 */
#define	CPU_WHICH_TID		1	/* Specifies a thread id. */
#define	CPU_WHICH_PID		2	/* Specifies a process id. */
#define	CPU_WHICH_CPUSET	3	/* Specifies a set id. */
#define	CPU_WHICH_IRQ		4	/* Specifies an irq #. */
#define	CPU_WHICH_JAIL		5	/* Specifies a jail id. */

/*
 * Reserved cpuset identifiers.
 */
#define	CPUSET_INVALID	-1
#define	CPUSET_DEFAULT	0

extern int __syscall_cpuset_getaffinity(int level, int which, int64_t id,
					size_t setsize, cpu_set_t *mask);
libc_hidden_proto(__syscall_cpuset_getaffinity)


extern int __syscall_cpuset_setaffinity(int level, int which, int64_t id,
					size_t setsize, const cpu_set_t *mask);
libc_hidden_proto(__syscall_cpuset_setaffinity)

