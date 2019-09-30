/*	$KAME: getifaddrs.c,v 1.9 2001/08/20 02:31:20 itojun Exp $	*/

/*
 * Copyright (c) 1995, 1999
 *	Berkeley Software Design, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY Berkeley Software Design, Inc. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Berkeley Software Design, Inc. BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	BSDI getifaddrs.c,v 2.12 2000/02/23 14:51:59 dab Exp
 */

#include <sys/cdefs.h>

 /*
__FBSDID("$FreeBSD: src/lib/libc/net/getifaddrs.c,v 1.6 2002/07/25 08:08:30 ume Exp $");
 */

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>

#ifdef	NET_RT_IFLIST
#include <sys/param.h>
#include <net/route.h>
#include <sys/sysctl.h>
#include <net/if_dl.h>
#else
#error NET_RT_IFLIST is required
#endif

#include <errno.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <string.h>

#if !defined(AF_LINK)
#define	SA_LEN(sa)	sizeof(struct sockaddr)
#endif

#if !defined(SA_LEN)
#define	SA_LEN(sa)	(sa)->sa_len
#endif

#define	SALIGN	(sizeof(long) - 1)
#define	SA_RLEN(sa)	((sa)->sa_len ? (((sa)->sa_len + SALIGN) & ~SALIGN) : (SALIGN + 1))

#ifndef	ALIGNBYTES
/*
 * On systems with a routing socket, ALIGNBYTES should match the value
 * that the kernel uses when building the messages.
 */
#define	ALIGNBYTES	XXX
#endif
#ifndef	ALIGN
#define	ALIGN(p)	(((u_long)(p) + ALIGNBYTES) &~ ALIGNBYTES)
#endif

#define	HAVE_IFM_DATA
#define MAX_SYSCTL_TRY 5

int
__getifaddrs(struct ifaddrs **pif)
{
	int icnt = 1;
	int dcnt = 0;
	int ncnt = 0;
#ifdef	NET_RT_IFLIST
	int ntry = 0;
	int mib[6];
	size_t needed;
	char *buf;
	char *next;
	struct ifaddrs *cif = 0;
	char *p, *p0;
	struct rt_msghdr *rtm;
	struct if_msghdr *ifm;
	struct ifa_msghdr *ifam;
	struct sockaddr_dl *dl;
	struct sockaddr *sa;
	struct ifaddrs *ifa, *ift;
	u_short idx = 0;
#else	/* NET_RT_IFLIST */
#endif	/* NET_RT_IFLIST */
	int i;
	size_t len, alen;
	char *data;
	char *names;

#ifdef	NET_RT_IFLIST
	mib[0] = CTL_NET;
	mib[1] = PF_ROUTE;
	mib[2] = 0;             /* protocol */
	mib[3] = 0;             /* wildcard address family */
	mib[4] = NET_RT_IFLIST;
	mib[5] = 0;             /* no flags */
	do {
		/*
		 * We'll try to get addresses several times in case that
		 * the number of addresses is unexpectedly increased during
		 * the two sysctl calls.  This should rarely happen, but we'll
		 * try to do our best for applications that assume success of
		 * this library (which should usually be the case).
		 * Portability note: since FreeBSD does not add margin of
		 * memory at the first sysctl, the possibility of failure on
		 * the second sysctl call is a bit higher.
		 */

		if (__sysctl(mib, 6, NULL, &needed, NULL, 0) < 0)
			return (-1);
		if ((buf = malloc(needed)) == NULL)
			return (-1);
		if (__sysctl(mib, 6, buf, &needed, NULL, 0) < 0) {
			if (errno != ENOMEM || ++ntry >= MAX_SYSCTL_TRY) {
				free(buf);
				return (-1);
			}
			free(buf);
			buf = NULL;
		}
	} while (buf == NULL);

	for (next = buf; next < buf + needed; next += rtm->rtm_msglen) {
		rtm = (struct rt_msghdr *)(void *)next;
		if (rtm->rtm_version != RTM_VERSION)
			continue;
		switch (rtm->rtm_type) {
		case RTM_IFINFO:
			ifm = (struct if_msghdr *)(void *)rtm;
			if (ifm->ifm_addrs & RTA_IFP) {
				idx = ifm->ifm_index;
				++icnt;
				dl = (struct sockaddr_dl *)(void *)(ifm + 1);
				/* XXX: smooth over a kfreebsd 9.0->10.1 ABI break */
				if (rtm->rtm_msglen == 152) {
					/* on kfreebsd-i386 9.0, struct rt_msghdr is 96 bytes */
					dl = (struct sockaddr_dl *)((char *)ifm + 96);
				}
				if (rtm->rtm_msglen == 156) {
					/* on kfreebsd-i386 10.1, struct rt_msghdr is 100 bytes */
					dl = (struct sockaddr_dl *)((char *)ifm + 100);
				}
				dcnt += SA_RLEN((struct sockaddr *)(void*)dl) +
				    ALIGNBYTES;
#ifdef	HAVE_IFM_DATA
				dcnt += sizeof(ifm->ifm_data);
#endif	/* HAVE_IFM_DATA */
				ncnt += dl->sdl_nlen + 1;
			} else
				idx = 0;
			break;

		case RTM_NEWADDR:
			ifam = (struct ifa_msghdr *)(void *)rtm;
			if (idx && ifam->ifam_index != idx)
			{
				errno = EINVAL;		/* this cannot happen */
				return -1;
			};
#define	RTA_MASKS	(RTA_NETMASK | RTA_IFA | RTA_BRD)
			if (idx == 0 || (ifam->ifam_addrs & RTA_MASKS) == 0)
				break;
			p = (char *)(void *)(ifam + 1);
			++icnt;
#ifdef	HAVE_IFAM_DATA
			dcnt += sizeof(ifam->ifam_data) + ALIGNBYTES;
#endif	/* HAVE_IFAM_DATA */
			/* Scan to look for length of address */
			alen = 0;
			for (p0 = p, i = 0; i < RTAX_MAX; i++) {
				if ((RTA_MASKS & ifam->ifam_addrs & (1 << i))
				    == 0)
					continue;
				sa = (struct sockaddr *)(void *)p;
				len = SA_RLEN(sa);
				if (i == RTAX_IFA) {
					alen = len;
					break;
				}
				p += len;
			}
			for (p = p0, i = 0; i < RTAX_MAX; i++) {
				if ((RTA_MASKS & ifam->ifam_addrs & (1 << i))
				    == 0)
					continue;
				sa = (struct sockaddr *)(void *)p;
				len = SA_RLEN(sa);
				if (i == RTAX_NETMASK && SA_LEN(sa) == 0)
					dcnt += alen;
				else
					dcnt += len;
				p += len;
			}
			break;
		}
	}
#else	/* NET_RT_IFLIST */
#endif	/* NET_RT_IFLIST */

	if (icnt + dcnt + ncnt == 1) {
		*pif = NULL;
		free(buf);
		return (0);
	}
	data = malloc(sizeof(struct ifaddrs) * icnt + dcnt + ncnt);
	if (data == NULL) {
		free(buf);
		return(-1);
	}

	ifa = (struct ifaddrs *)(void *)data;
	data += sizeof(struct ifaddrs) * icnt;
	names = data + dcnt;

	memset(ifa, 0, sizeof(struct ifaddrs) * icnt);
	ift = ifa;

#ifdef	NET_RT_IFLIST
	idx = 0;
	for (next = buf; next < buf + needed; next += rtm->rtm_msglen) {
		rtm = (struct rt_msghdr *)(void *)next;
		if (rtm->rtm_version != RTM_VERSION)
			continue;
		switch (rtm->rtm_type) {
		case RTM_IFINFO:
			ifm = (struct if_msghdr *)(void *)rtm;
			if (ifm->ifm_addrs & RTA_IFP) {
				idx = ifm->ifm_index;
				dl = (struct sockaddr_dl *)(void *)(ifm + 1);
				/* XXX: smooth over a kfreebsd 9.0->10.1 ABI break */
				if (rtm->rtm_msglen == 152) {
					/* on kfreebsd-i386 9.0, struct rt_msghdr is 96 bytes */
					dl = (struct sockaddr_dl *)((char *)ifm + 96);
				}
				if (rtm->rtm_msglen == 156) {
					/* on kfreebsd-i386 10.1, struct rt_msghdr is 100 bytes */
					dl = (struct sockaddr_dl *)((char *)ifm + 100);
				}

				cif = ift;
				ift->ifa_name = names;
				ift->ifa_flags = (int)ifm->ifm_flags;
				memcpy(names, dl->sdl_data,
				    (size_t)dl->sdl_nlen);
				names[dl->sdl_nlen] = 0;
				names += dl->sdl_nlen + 1;

				ift->ifa_addr = (struct sockaddr *)(void *)data;
				memcpy(data, dl,
				    (size_t)SA_LEN((struct sockaddr *)
				    (void *)dl));
				data += SA_RLEN((struct sockaddr *)(void *)dl);

#ifdef	HAVE_IFM_DATA
				/* ifm_data needs to be aligned */
				ift->ifa_data = data = (void *)ALIGN(data);
				memcpy(data, &ifm->ifm_data, sizeof(ifm->ifm_data));
 				data += sizeof(ifm->ifm_data);
#else	/* HAVE_IFM_DATA */
				ift->ifa_data = NULL;
#endif	/* HAVE_IFM_DATA */

				ift = (ift->ifa_next = ift + 1);
			} else
				idx = 0;
			break;

		case RTM_NEWADDR:
			ifam = (struct ifa_msghdr *)(void *)rtm;
			if (idx && ifam->ifam_index != idx)
			{
				errno = EINVAL;		/* this cannot happen */
				return -1;
			};
			if (idx == 0 || (ifam->ifam_addrs & RTA_MASKS) == 0)
				break;
			ift->ifa_name = cif->ifa_name;
			ift->ifa_flags = cif->ifa_flags;
			ift->ifa_data = NULL;
			p = (char *)(void *)(ifam + 1);
			/* Scan to look for length of address */
			alen = 0;
			for (p0 = p, i = 0; i < RTAX_MAX; i++) {
				if ((RTA_MASKS & ifam->ifam_addrs & (1 << i))
				    == 0)
					continue;
				sa = (struct sockaddr *)(void *)p;
				len = SA_RLEN(sa);
				if (i == RTAX_IFA) {
					alen = len;
					break;
				}
				p += len;
			}
			for (p = p0, i = 0; i < RTAX_MAX; i++) {
				if ((RTA_MASKS & ifam->ifam_addrs & (1 << i))
				    == 0)
					continue;
				sa = (struct sockaddr *)(void *)p;
				len = SA_RLEN(sa);
				switch (i) {
				case RTAX_IFA:
					ift->ifa_addr =
					    (struct sockaddr *)(void *)data;
					memcpy(data, p, len);
					data += len;
					break;

				case RTAX_NETMASK:
					ift->ifa_netmask =
					    (struct sockaddr *)(void *)data;
					if (SA_LEN(sa) == 0) {
						memset(data, 0, alen);
						data += alen;
						break;
					}
					memcpy(data, p, len);
					data += len;
					break;

				case RTAX_BRD:
					ift->ifa_broadaddr =
					    (struct sockaddr *)(void *)data;
					memcpy(data, p, len);
					data += len;
					break;
				}
				p += len;
			}

#ifdef	HAVE_IFAM_DATA
			/* ifam_data needs to be aligned */
			ift->ifa_data = data = (void *)ALIGN(data);
			memcpy(data, &ifam->ifam_data, sizeof(ifam->ifam_data));
			data += sizeof(ifam->ifam_data);
#endif	/* HAVE_IFAM_DATA */

			ift = (ift->ifa_next = ift + 1);
			break;
		}
	}

	free(buf);
#else	/* NET_RT_IFLIST */
#endif	/* NET_RT_IFLIST */
	if (--ift >= ifa) {
		ift->ifa_next = NULL;
		*pif = ifa;
	} else {
		*pif = NULL;
		free(ifa);
	}
	return (0);
}
weak_alias (__getifaddrs, getifaddrs)
libc_hidden_weak (getifaddrs)

void
__freeifaddrs(struct ifaddrs *ifp)
{

	free(ifp);
}
weak_alias (__freeifaddrs, freeifaddrs)
libc_hidden_weak (freeifaddrs)
