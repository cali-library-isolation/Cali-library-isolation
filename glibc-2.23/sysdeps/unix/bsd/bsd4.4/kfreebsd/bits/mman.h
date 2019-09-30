/* Definitions for POSIX memory map interface.  FreeBSD version.
   Copyright (C) 1994-1998, 2000-2002 Free Software Foundation, Inc.
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

#ifndef _SYS_MMAN_H
# error "Never use <bits/mman.h> directly; include <sys/mman.h> instead."
#endif

#include <features.h>
#include <stddef.h>

/* Protections are chosen from these bits, OR'd together.  The
   implementation does not necessarily support PROT_EXEC or PROT_WRITE
   without PROT_READ.  The only guarantees are that no writing will be
   allowed without PROT_WRITE and no access will be allowed for PROT_NONE. */

#define	PROT_NONE	 0x00	/* No access.  */
#define	PROT_READ	 0x01	/* Pages can be read.  */
#define	PROT_WRITE	 0x02	/* Pages can be written.  */
#define	PROT_EXEC	 0x04	/* Pages can be executed.  */

/* Flags contain mapping type, sharing type and options.  */

/* Mapping type (must choose one and only one of these).  */
#ifdef __USE_MISC
# define MAP_FILE	 0x0000	/* Mapped from a file or device.  */
# define MAP_ANON	 0x1000	/* Allocated from anonymous virtual memory.  */
# define MAP_TYPE	 0x1000	/* Mask for type field.  */
# ifdef __USE_MISC
#  define MAP_ANONYMOUS  MAP_ANON /* Linux name. */
# endif
#endif

/* Sharing types (must choose one and only one of these).  */
#define	MAP_SHARED	 0x0001	/* Share changes.  */
#define	MAP_PRIVATE	 0x0002	/* Changes private; copy pages on write.  */
#ifdef __USE_MISC
# define MAP_COPY MAP_PRIVATE	/* Virtual copy of region at mapping time.  */
#endif

/* Other flags.  */
#define	MAP_FIXED	 0x0010	/* Map address must be exactly as requested. */
#ifdef __USE_MISC
#define MAP_RENAME	 0x0020 /* Sun: rename private pages to file */
#define MAP_NORESERVE	 0x0040 /* Sun: don't reserve needed swap area */
#define MAP_RESERVED0080 0x0080	/* previously misimplemented MAP_INHERIT */
#define MAP_RESERVED0100 0x0100	/* previously unimplemented MAP_NOEXTEND */
# define MAP_HASSEMPHORE 0x0200	/* Region may contain semaphores.  */
# define MAP_STACK	 0x0400	/* Region grows down, like a stack.  */
# define MAP_NOSYNC	 0x0800	/* Try to avoid flushing to the disk.  */
# define MAP_NOCORE	0x20000	/* Don't include these pages in a core dump.  */
#endif

/* Advice to `madvise'.  */
#ifdef __USE_MISC
# define MADV_NORMAL	 0	/* No further special treatment.  */
# define MADV_RANDOM	 1	/* Expect random page references.  */
# define MADV_SEQUENTIAL 2	/* Expect sequential page references.  */
# define MADV_WILLNEED	 3	/* Will need these pages.  */
# define MADV_DONTNEED	 4	/* Don't need these pages.  */
# define MADV_FREE	 5	/* Don't need these pages, they contain junk. */
# define MADV_NOSYNC	 6	/* Try to avoid flushing to the disk.  */
# define MADV_AUTOSYNC	 7	/* Use the default flushing strategy.  */
# define MADV_NOCORE	 8	/* Don't include these pages in a core dump.  */
# define MADV_CORE	 9	/* Include pages in a core dump (default).  */
# define MADV_PROTECT	10	/* protect process from pageout kill */
#endif

/* The POSIX people had to invent similar names for the same things.  */
#ifdef __USE_XOPEN2K
# define POSIX_MADV_NORMAL	0 /* No further special treatment.  */
# define POSIX_MADV_RANDOM	1 /* Expect random page references.  */
# define POSIX_MADV_SEQUENTIAL	2 /* Expect sequential page references.  */
# define POSIX_MADV_WILLNEED	3 /* Will need these pages.  */
# define POSIX_MADV_DONTNEED	4 /* Don't need these pages.  */
#endif

/* Flags to `msync'.  */
#define MS_ASYNC	1		/* Sync memory asynchronously.  */
#define MS_SYNC		0		/* Synchronous memory sync.  */
#define MS_INVALIDATE	2		/* Invalidate the caches.  */

/* Flags for `mlockall' (can be OR'd together).  */
#define MCL_CURRENT	1		/* Lock all currently mapped pages.  */
#define MCL_FUTURE	2		/* Lock all additions to address
					   space.  */

/* Flags for 'minherit'.  */
#ifdef __USE_MISC
# define INHERIT_SHARE	0
# define INHERIT_COPY	1
# define INHERIT_NONE	2
#endif


/*
 * Return bits from mincore
 */
#ifdef __USE_MISC
#define	MINCORE_INCORE	 	 0x1 /* Page is incore */
#define	MINCORE_REFERENCED	 0x2 /* Page has been referenced by us */
#define	MINCORE_MODIFIED	 0x4 /* Page has been modified by us */
#define	MINCORE_REFERENCED_OTHER 0x8 /* Page has been referenced */
#define	MINCORE_MODIFIED_OTHER	0x10 /* Page has been modified */
#endif /* Use MISC */

#ifdef __USE_MISC

__BEGIN_DECLS

extern int minherit (void *__addr, size_t __len, int __inherit);

__END_DECLS

#endif /* Use BSD */
