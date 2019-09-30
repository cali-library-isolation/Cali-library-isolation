/* Copyright (C) 1995,1996,1997,2000,2001,2003 Free Software Foundation, Inc.
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

#ifndef _SYS_PARAM_H
# error "Never use <bits/param.h> directly; include <sys/param.h> instead."
#endif

#include <limits.h>
#include <sys/syslimits.h>

/*
 * __FreeBSD_version numbers are documented in the Porter's Handbook.
 * If you bump the version for any reason, you should update the documentation
 * there.
 * Currently this lives here:
 *
 *      doc/en_US.ISO8859-1/books/porters-handbook/book.sgml
 *
 * scheme is:  <major><two digit minor><0 if release branch, otherwise 1>xx
 */
#include <osreldate.h>

/* Some inet code expects that this file defines the 'u_int32_t' type.  */
#include <stdint.h>

/* FreeBSD code expects that this file implicitly defines SIG* macros.  */
#include <signal.h>

/*
 * Machine-independent constants (some used in following include files).
 * Redefined constants are from POSIX 1003.1 limits file.
 *
 * MAXCOMLEN should be >= sizeof(ac_comm) (see <acct.h>)
 * MAXLOGNAME should be == UT_NAMESIZE+1 (see <utmp.h>)
 */

#define MAXCOMLEN       19              /* max command name remembered */
#define MAXINTERP       32              /* max interpreter file name length */
#define MAXLOGNAME      33              /* max login name length (incl. NUL) */
#define MAXUPRC         CHILD_MAX       /* max simultaneous processes */
#define NGROUPS         (NGROUPS_MAX+1) /* max number groups */
#define MAXHOSTNAMELEN  256             /* max hostname size */
#define SPECNAMELEN     63              /* max length of devicename */
#define TTY_NAME_MAX	SPECNAMELEN

/* BSD names for some <limits.h> values.  */

#define	MAXSYMLINKS	32
#define	CANBSIZ		MAX_CANON

/* Machine type dependent parameters. */
#include <machine/param.h>

#ifndef BLKDEV_IOSIZE
#define BLKDEV_IOSIZE	PAGE_SIZE        /* default block device I/O size */
#endif
#ifndef DFLTPHYS
#define DFLTPHYS	(64 * 1024)     /* default max raw I/O transfer size */
#endif
#ifndef MAXPHYS
#define MAXPHYS		(128 * 1024)    /* max raw I/O transfer size */
#endif
#ifndef MAXDUMPPGS
#define MAXDUMPPGS	(DFLTPHYS/PAGE_SIZE)
#endif

/*
 * Constants related to network buffer management.
 * MCLBYTES must be no larger than PAGE_SIZE.
 */
#ifndef	MSIZE
#define MSIZE		256		/* size of an mbuf */
#endif	/* MSIZE */

#ifndef	MCLSHIFT
#define MCLSHIFT	11		/* convert bytes to mbuf clusters */
#endif	/* MCLSHIFT */

#define MCLBYTES	(1 << MCLSHIFT)	/* size of an mbuf cluster */

/*
 * Some macros for units conversion
 */

/* clicks to bytes */
#ifndef ctob
#define ctob(x)	((x)<<PAGE_SHIFT)
#endif

/* bytes to clicks */
#ifndef btoc
#define btoc(x)	(((vm_offset_t)(x)+PAGE_MASK)>>PAGE_SHIFT)
#endif

/*
 * btodb() is messy and perhaps slow because `bytes' may be an off_t.  We
 * want to shift an unsigned type to avoid sign extension and we don't
 * want to widen `bytes' unnecessarily.  Assume that the result fits in
 * a daddr_t.
 */
#ifndef btodb
#define btodb(bytes)	 		/* calculates (bytes / DEV_BSIZE) */ \
	(sizeof (bytes) > sizeof(long) \
	 ? (daddr_t)((unsigned long long)(bytes) >> DEV_BSHIFT) \
	 : (daddr_t)((unsigned long)(bytes) >> DEV_BSHIFT))
#endif

#ifndef dbtob
#define dbtob(db)			/* calculates (db * DEV_BSIZE) */ \
	((off_t)(db) << DEV_BSHIFT)
#endif

#define	PRIMASK	0x0ff
#define	PCATCH	0x100		/* OR'd with pri for tsleep to check signals */
#define	PDROP	0x200	/* OR'd with pri to stop re-entry of interlock mutex */

#define	NBPW	sizeof(int)	/* number of bytes per word (integer) */

#define	CMASK	022		/* default file mask: S_IWGRP|S_IWOTH */

#define	NODEV	(dev_t)(-1)	/* non-existent device */

#define	CBLOCK	128		/* Clist block size, must be a power of 2. */
#define CBQSIZE	(CBLOCK/NBBY)	/* Quote bytes/cblock - can do better. */
				/* Data chars/clist. */
#define	CBSIZE	(CBLOCK - sizeof(struct cblock *) - CBQSIZE)
#define	CROUND	(CBLOCK - 1)	/* Clist rounding. */

/*
 * File system parameters and macros.
 *
 * MAXBSIZE -	Filesystems are made out of blocks of at most MAXBSIZE bytes
 *		per block.  MAXBSIZE may be made larger without effecting
 *		any existing filesystems as long as it does not exceed MAXPHYS,
 *		and may be made smaller at the risk of not being able to use
 *		filesystems which require a block size exceeding MAXBSIZE.
 *
 * BKVASIZE -	Nominal buffer space per buffer, in bytes.  BKVASIZE is the
 *		minimum KVM memory reservation the kernel is willing to make.
 *		Filesystems can of course request smaller chunks.  Actual
 *		backing memory uses a chunk size of a page (PAGE_SIZE).
 *
 *		If you make BKVASIZE too small you risk seriously fragmenting
 *		the buffer KVM map which may slow things down a bit.  If you
 *		make it too big the kernel will not be able to optimally use
 *		the KVM memory reserved for the buffer cache and will wind
 *		up with too-few buffers.
 *
 *		The default is 16384, roughly 2x the block size used by a
 *		normal UFS filesystem.
 */
#define MAXBSIZE	65536	/* must be power of 2 */
#define BKVASIZE	16384	/* must be power of 2 */
#define BKVAMASK	(BKVASIZE-1)

/*
 * Scale factor for scaled integers used to count %cpu time and load avgs.
 *
 * The number of CPU `tick's that map to a unique `%age' can be expressed
 * by the formula (1 / (2 ^ (FSHIFT - 11))).  The maximum load average that
 * can be calculated (assuming 32 bits) can be closely approximated using
 * the formula (2 ^ (2 * (16 - FSHIFT))) for (FSHIFT < 15).
 *
 * For the scheduler to maintain a 1:1 mapping of CPU `tick' to `%age',
 * FSHIFT must be at least 11; this gives us a maximum load avg of ~1024.
 */
#define	FSHIFT	11		/* bits to right of fixed binary point */
#define FSCALE	(1<<FSHIFT)

#define dbtoc(db)			/* calculates devblks to pages */ \
	((db + (ctodb(1) - 1)) >> (PAGE_SHIFT - DEV_BSHIFT))

#define ctodb(db)			/* calculates pages to devblks */ \
	((db) << (PAGE_SHIFT - DEV_BSHIFT))
