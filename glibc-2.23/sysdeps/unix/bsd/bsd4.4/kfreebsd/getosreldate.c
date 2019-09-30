/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stddef.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <getosreldate.h>

int
__kernel_getosreldate(void)
{
    static int osreldate;

    int mib[2];
    size_t size;

    if (osreldate == 0)
    {
	mib[0] = CTL_KERN;
	mib[1] = KERN_OSRELDATE;
	size = sizeof osreldate;
	if (__sysctl(mib, 2, &osreldate, &size, NULL, 0) == -1)
		return (-1);
    }		
    return (osreldate);
}

int
__getosreldate(void)
{
    static int osreldate;

    char *temp;

    if (osreldate == 0)
    {
	if ((temp = getenv("OSVERSION"))) {
		osreldate = atoi(temp);
		return (osreldate);
	}

	osreldate = __kernel_getosreldate ();
    }		
    return (osreldate);
}
libc_hidden_def (__kernel_getosreldate)
libc_hidden_def (__getosreldate)
weak_alias (__getosreldate, getosreldate)
