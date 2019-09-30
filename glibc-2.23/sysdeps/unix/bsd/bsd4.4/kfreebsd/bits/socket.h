/* System-specific socket constants and types.  FreeBSD version.
   Copyright (C) 1991-1992,1994-1999,2000-2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef __BITS_SOCKET_H
#define __BITS_SOCKET_H	1

#if !defined _SYS_SOCKET_H && !defined _NETINET_IN_H
# error "Never include <bits/socket.h> directly; use <sys/socket.h> instead."
#endif

#define	__need_size_t
#define __need_NULL
#include <stddef.h>

#include <limits.h>		/* XXX Is this allowed?  */
#include <bits/types.h>

/* Type for length arguments in socket calls.  */
#ifndef __socklen_t_defined
typedef __socklen_t socklen_t;
# define __socklen_t_defined
#endif


/* Types of sockets.  */
enum __socket_type
{
  SOCK_STREAM = 1,		/* Sequenced, reliable, connection-based
				   byte streams.  */
#define SOCK_STREAM SOCK_STREAM
  SOCK_DGRAM = 2,		/* Connectionless, unreliable datagrams
				   of fixed maximum length.  */
#define SOCK_DGRAM SOCK_DGRAM
  SOCK_RAW = 3,			/* Raw protocol interface.  */
#define SOCK_RAW SOCK_RAW
  SOCK_RDM = 4,			/* Reliably-delivered messages.  */
#define SOCK_RDM SOCK_RDM
  SOCK_SEQPACKET = 5,		/* Sequenced, reliable, connection-based,
				   datagrams of fixed maximum length.  */
#define SOCK_SEQPACKET SOCK_SEQPACKET

  /* Flags to be ORed into the type parameter of socket and socketpair and
     used for the flags parameter of paccept.  */

  SOCK_CLOEXEC = 0x10000000,	/* Atomically set close-on-exec flag for the
				new descriptor(s).  */
#define SOCK_CLOEXEC SOCK_CLOEXEC
  SOCK_NONBLOCK = 0x20000000	/* Atomically mark descriptor(s) as
				non-blocking.  */
#define SOCK_NONBLOCK SOCK_NONBLOCK
};

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto {
	unsigned short	sp_family;		/* address family */
	unsigned short	sp_protocol;		/* protocol */
};

/* Protocol families.  */
#define	PF_UNSPEC	0	/* Unspecified.  */
#define	PF_LOCAL	1	/* Local to host (pipes and file-domain).  */
#define	PF_UNIX		PF_LOCAL /* Old BSD name for PF_LOCAL.  */
#define	PF_FILE		PF_LOCAL /* POSIX name for PF_LOCAL.  */
#define	PF_INET		2	/* IP protocol family.  */
#define	PF_IMPLINK	3	/* ARPAnet IMP protocol.  */
#define	PF_PUP		4	/* PUP protocols.  */
#define	PF_CHAOS	5	/* MIT Chaos protocols.  */
#define	PF_NETBIOS	6	/* SMB protocols.  */
#define	PF_ISO		7	/* ISO protocols.  */
#define	PF_OSI		PF_ISO
#define	PF_ECMA		8	/* ECMA protocols.  */
#define	PF_DATAKIT	9	/* AT&T Datakit protocols.  */
#define	PF_CCITT	10	/* CCITT protocols (X.25 et al).  */
#define	PF_SNA		11	/* IBM SNA protocol.  */
#define	PF_DECnet	12	/* DECnet protocols.  */
#define	PF_DLI		13	/* Direct data link interface.  */
#define	PF_LAT		14	/* DEC Local Area Transport protocol.  */
#define	PF_HYLINK	15	/* NSC Hyperchannel protocol.  */
#define	PF_APPLETALK	16	/* Don't use this.  */
#define	PF_ROUTE	17	/* Internal Routing Protocol.  */
#define	PF_LINK		18	/* Link layer interface.  */
#define	PF_XTP		19	/* eXpress Transfer Protocol (no AF).  */
#define	PF_COIP		20	/* Connection-oriented IP, aka ST II.  */
#define	PF_CNT		21	/* Computer Network Technology.  */
#define PF_RTIP		22	/* Help Identify RTIP packets.  **/
#define	PF_IPX		23	/* Novell Internet Protocol.  */
#define	PF_SIP		24	/* Simple Internet Protocol.  */
#define PF_PIP		25	/* Help Identify PIP packets.  */
#define PF_ISDN		26	/* Integrated Services Digital Network.  */
#define PF_KEY		27	/* Internal key-management function.  */
#define PF_INET6	28	/* IP version 6.  */
#define PF_NATM		29	/* Native ATM access.  */
#define PF_ATM		30	/* ATM.  */
#define PF_HDRCMPLT	31	/* Used by BPF to not rewrite headers in
				   interface output routine.  */
#define PF_NETGRAPH	32	/* Netgraph sockets.  */
#define	PF_MAX		33

/* Address families.  */
#define	AF_UNSPEC	PF_UNSPEC
#define	AF_LOCAL	PF_LOCAL
#define	AF_UNIX		PF_UNIX
#define	AF_FILE		PF_FILE
#define	AF_INET		PF_INET
#define	AF_IMPLINK	PF_IMPLINK
#define	AF_PUP		PF_PUP
#define	AF_CHAOS	PF_CHAOS
#define	AF_NETBIOS	PF_NETBIOS
#define	AF_ISO		PF_ISO
#define	AF_OSI		PF_OSI
#define	AF_ECMA		PF_ECMA
#define	AF_DATAKIT	PF_DATAKIT
#define	AF_CCITT	PF_CCITT
#define	AF_SNA		PF_SNA
#define	AF_DECnet	PF_DECnet
#define	AF_DLI		PF_DLI
#define	AF_LAT		PF_LAT
#define	AF_HYLINK	PF_HYLINK
#define	AF_APPLETALK	PF_APPLETALK
#define	AF_ROUTE	PF_ROUTE
#define	AF_LINK		PF_LINK
#define	pseudo_AF_XTP	PF_XTP
#define	AF_COIP		PF_COIP
#define	AF_CNT		PF_CNT
#define pseudo_AF_RTIP	PF_RTIP
#define	AF_IPX		PF_IPX
#define	AF_SIP		PF_SIP
#define pseudo_AF_PIP	PF_PIP
#define AF_ISDN		PF_ISDN
#define AF_E164		AF_ISDN		/* CCITT E.164 recommendation.  */
#define pseudo_AF_KEY	PF_KEY
#define AF_INET6	PF_INET6
#define AF_NATM		PF_NATM
#define AF_ATM		PF_ATM
#define pseudo_AF_HDRCMPLT PF_HDRCMPLT
#define AF_NETGRAPH	PF_NETGRAPH
#define	AF_MAX		PF_MAX

/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN	128	/* 5 on the original 4.4 BSD.  */

/* Get the definition of the macro to define the common sockaddr members.  */
#include <bits/sockaddr.h>

/* Structure describing a generic socket address.  */
struct sockaddr
  {
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
  };


/* Structure large enough to hold any socket address (with the historical
   exception of AF_UNIX).  We reserve 128 bytes.  */
#if ULONG_MAX > 0xffffffff
# define __ss_aligntype	__uint64_t
#else
# define __ss_aligntype	__uint32_t
#endif
#define _SS_SIZE	128
#define _SS_PADSIZE	(_SS_SIZE - (2 * sizeof (__ss_aligntype)))

struct sockaddr_storage
  {
    __SOCKADDR_COMMON (ss_);	/* Address family, etc.  */
    __ss_aligntype __ss_align;	/* Force desired alignment.  */
    char __ss_padding[_SS_PADSIZE];
  };


/* Bits in the FLAGS argument to `send', `recv', et al.  */
enum
  {
    MSG_OOB		= 0x01,	/* Process out-of-band data.  */
#define MSG_OOB MSG_OOB
    MSG_PEEK		= 0x02,	/* Peek at incoming messages.  */
#define MSG_PEEK MSG_PEEK
    MSG_DONTROUTE	= 0x04,	/* Don't use local routing.  */
#define MSG_DONTROUTE MSG_DONTROUTE
    MSG_EOR		= 0x08,	/* Data completes record.  */
#define MSG_EOR MSG_EOR
    MSG_TRUNC		= 0x10,	/* Data discarded before delivery.  */
#define MSG_TRUNC MSG_TRUNC
    MSG_CTRUNC		= 0x20,	/* Control data lost before delivery.  */
#define MSG_CTRUNC MSG_CTRUNC
    MSG_WAITALL		= 0x40,	/* Wait for full request or error.  */
#define MSG_WAITALL MSG_WAITALL
    MSG_DONTWAIT	= 0x80,	/* This message should be nonblocking.  */
#define MSG_DONTWAIT MSG_DONTWAIT
    MSG_EOF		= 0x100, /* Data completes connection.  */
#define MSG_EOF MSG_EOF
    MSG_NOTIFICATION	= 0x2000,/* SCTP notification */
#define MSG_NOTIFICATION MSG_NOTIFICATION
    MSG_NBIO		= 0x4000,/*FIONBIO mode, used by fifofs */
#define MSG_NBIO MSG_NBIO
    MSG_COMPAT		= 0x8000,/* Used in sendit().  */
#define MSG_COMPAT MSG_COMPAT
    MSG_NOSIGNAL	= 0x20000 /* do not generate SIGPIPE on EOF */
#define MSG_NOSIGNAL MSG_NOSIGNAL
  };


/* Structure describing messages sent by
   `sendmsg' and received by `recvmsg'.  */
struct msghdr
  {
    void *msg_name;		/* Address to send to/receive from.  */
    socklen_t msg_namelen;	/* Length of address data.  */

    struct iovec *msg_iov;	/* Vector of data to send/receive into.  */
    int msg_iovlen;		/* Number of elements in the vector.  */

    void *msg_control;		/* Ancillary data (eg BSD filedesc passing). */
    socklen_t msg_controllen;	/* Ancillary data buffer length.  */

    int msg_flags;		/* Flags in received message.  */
  };

/* Structure used for storage of ancillary data object information.  */
struct cmsghdr
  {
    socklen_t cmsg_len;		/* Length of data in cmsg_data plus length
				   of cmsghdr structure.  */
    int cmsg_level;		/* Originating protocol.  */
    int cmsg_type;		/* Protocol specific type.  */
#if (!defined __STRICT_ANSI__ && __GNUC__ >= 2) || __STDC_VERSION__ >= 199901L
    __extension__ unsigned char __cmsg_data  __flexarr __attribute__ ((aligned (__alignof__(size_t)))); /* Ancillary data.  */
#endif
  };

#define CMSG_NXTHDR(mhdr, cmsg) __cmsg_nxthdr (mhdr, cmsg)

#define CMSG_FIRSTHDR(mhdr) \
  ((size_t) (mhdr)->msg_controllen >= sizeof (struct cmsghdr)		      \
   ? (struct cmsghdr *) (mhdr)->msg_control : (struct cmsghdr *) NULL)

#define CMSG_ALIGN(len) (((len) + sizeof (size_t) - 1) \
			   & (size_t) ~(sizeof (size_t) - 1))
#define CMSG_SPACE(len) (CMSG_ALIGN (len) \
			 + CMSG_ALIGN (sizeof (struct cmsghdr)))
#define CMSG_LEN(len)   (CMSG_ALIGN (sizeof (struct cmsghdr)) + (len))

/* Ancillary data object manipulation macros.  */
#if (!defined __STRICT_ANSI__ && __GNUC__ >= 2) || __STDC_VERSION__ >= 199901L
# define CMSG_DATA(cmsg) ((cmsg)->__cmsg_data)
#else
# define CMSG_DATA(cmsg) ((unsigned char *) (cmsg) + CMSG_ALIGN(sizeof (struct cmsghdr)))
#endif

extern struct cmsghdr *__cmsg_nxthdr (struct msghdr *__mhdr,
				      struct cmsghdr *__cmsg) __THROW;
#ifdef __USE_EXTERN_INLINES
# ifndef _EXTERN_INLINE
#  define _EXTERN_INLINE __extern_inline
# endif
_EXTERN_INLINE struct cmsghdr *
__NTH (__cmsg_nxthdr (struct msghdr *__mhdr, struct cmsghdr *__cmsg))
{
  if ((size_t) __cmsg->cmsg_len < sizeof (struct cmsghdr))
    /* The kernel header does this so there may be a reason.  */
    return 0;

  __cmsg = (struct cmsghdr *) ((unsigned char *) __cmsg
			       + CMSG_ALIGN (__cmsg->cmsg_len));
  if ((unsigned char *) (__cmsg + 1) > ((unsigned char *) __mhdr->msg_control
					+ __mhdr->msg_controllen)
      || ((unsigned char *) __cmsg + CMSG_ALIGN (__cmsg->cmsg_len)
	  > ((unsigned char *) __mhdr->msg_control + __mhdr->msg_controllen)))
    /* No more entries.  */
    return 0;
  return __cmsg;
}
#endif	/* Use `extern inline'.  */

/* Socket level message types.  */
enum
  {
    SCM_RIGHTS = 0x01,		/* Access rights (array of int).  */
#define SCM_RIGHTS SCM_RIGHTS
    SCM_TIMESTAMP = 0x02,	/* Timestamp (struct timeval).  */
#define SCM_TIMESTAMP SCM_TIMESTAMP
    SCM_CREDS = 0x03		/* Process creds (struct cmsgcred).  */
#define SCM_CREDS SCM_CREDS
  };

/* Unfortunately, BSD practice dictates this structure be of fixed size.
   If there are more than CMGROUP_MAX groups, the list is truncated.
   (On GNU systems, the `cmcred_euid' field is just the first in the
   list of effective UIDs.)  */
#define CMGROUP_MAX	16

/* Structure delivered by SCM_CREDS.  This describes the identity of the
   sender of the data simultaneously received on the socket.  By BSD
   convention, this is included only when a sender on a AF_LOCAL socket
   sends cmsg data of this type and size; the sender's structure is
   ignored, and the system fills in the various IDs of the sender process.  */
struct cmsgcred
  {
    __pid_t cmcred_pid;
    __uid_t cmcred_uid;
    __uid_t cmcred_euid;
    __gid_t cmcred_gid;
    short cmcred_ngroups;
    __gid_t cmcred_groups[CMGROUP_MAX];
  };

/* Protocol number used to manipulate socket-level options
   with `getsockopt' and `setsockopt'.  */
#define	SOL_SOCKET	0xffff

/* Socket-level options for `getsockopt' and `setsockopt'.  */
enum
  {
    SO_DEBUG = 0x0001,		/* Record debugging information.  */
#define SO_DEBUG SO_DEBUG
    SO_ACCEPTCONN = 0x0002,	/* Accept connections on socket.  */
#define SO_ACCEPTCONN SO_ACCEPTCONN
    SO_REUSEADDR = 0x0004,	/* Allow reuse of local addresses.  */
#define SO_REUSEADDR SO_REUSEADDR
    SO_KEEPALIVE = 0x0008,	/* Keep connections alive and send
				   SIGPIPE when they die.  */
#define SO_KEEPALIVE SO_KEEPALIVE
    SO_DONTROUTE = 0x0010,	/* Don't do local routing.  */
#define SO_DONTROUTE SO_DONTROUTE
    SO_BROADCAST = 0x0020,	/* Allow transmission of
				   broadcast messages.  */
#define SO_BROADCAST SO_BROADCAST
    SO_USELOOPBACK = 0x0040,	/* Use the software loopback to avoid
				   hardware use when possible.  */
#define SO_USELOOPBACK SO_USELOOPBACK
    SO_LINGER = 0x0080,		/* Block on close of a reliable
				   socket to transmit pending data.  */
#define SO_LINGER SO_LINGER
    SO_OOBINLINE = 0x0100,	/* Receive out-of-band data in-band.  */
#define SO_OOBINLINE SO_OOBINLINE
    SO_REUSEPORT = 0x0200,	/* Allow local address and port reuse.  */
#define SO_REUSEPORT SO_REUSEPORT
    SO_TIMESTAMP = 0x0400,	/* Timestamp received dgram traffic.  */
#define SO_TIMESTAMP SO_TIMESTAMP
    SO_SNDBUF = 0x1001,		/* Send buffer size.  */
#define SO_SNDBUF SO_SNDBUF
    SO_RCVBUF = 0x1002,		/* Receive buffer.  */
#define SO_RCVBUF SO_RCVBUF
    SO_SNDLOWAT = 0x1003,	/* Send low-water mark.  */
#define SO_SNDLOWAT SO_SNDLOWAT
    SO_RCVLOWAT = 0x1004,	/* Receive low-water mark.  */
#define SO_RCVLOWAT SO_RCVLOWAT
    SO_SNDTIMEO = 0x1005,	/* Send timeout.  */
#define SO_SNDTIMEO SO_SNDTIMEO
    SO_RCVTIMEO = 0x1006,	/* Receive timeout.  */
#define SO_RCVTIMEO SO_RCVTIMEO
    SO_ERROR = 0x1007,		/* Get and clear error status.  */
#define SO_ERROR SO_ERROR
    SO_STYLE = 0x1008,		/* Get socket connection style.  */
#define SO_STYLE SO_STYLE
    SO_TYPE = SO_STYLE,		/* Compatible name for SO_STYLE.  */
#define SO_TYPE SO_TYPE
    SO_LABEL = 0x1009,
#define SO_LABEL SO_LABEL
    SO_PEERLABEL = 0x1010,
#define SO_PEERLABEL SO_PEERLABEL
    SO_LISTENQLIMIT = 0x1011,
#define SO_LISTENQLIMIT SO_LISTENQLIMIT
    SO_LISTENQLEN = 0x1012,
#define SO_LISTENQLEN SO_LISTENQLEN
    SO_LISTENINCQLEN = 0x1013,
#define SO_LISTENINCQLEN SO_LISTENINCQLEN
    SO_SETFIB = 0x1014,
#define SO_SETFIB SO_SETFIB
    SO_USER_COOKIE = 0x1015,
#define SO_USER_COOKIE SO_USER_COOKIE
    SO_PROTOCOL = 0x1016,
#define SO_PROTOCOL SO_PROTOCOL
    SO_PROTOTYPE = SO_PROTOCOL
#define SO_PROTOTYPE SO_PROTOTYPE
  };

/* Socket options. */
#define LOCAL_PEERCRED          0x001   /* retrieve peer credentials */
#define LOCAL_CREDS             0x002   /* pass credentials to receiver */
#define LOCAL_CONNWAIT          0x004   /* connects block until accepted */

/* Structure used to manipulate the SO_LINGER option.  */
struct linger
  {
    int l_onoff;		/* Nonzero to linger on close.  */
    int l_linger;		/* Time to linger.  */
  };

/* Magic IPv4 addresses defined in FreeBSD version of <netinet/in.h>, but not
   included in Glibc version of the same header. */
#define	INADDR_ALLRPTS_GROUP	(u_int32_t)0xe0000016	/* 224.0.0.22, IGMPv3 */
#define	INADDR_CARP_GROUP	(u_int32_t)0xe0000012	/* 224.0.0.18 */
#define	INADDR_PFSYNC_GROUP	(u_int32_t)0xe00000f0	/* 224.0.0.240 */
#define	INADDR_ALLMDNS_GROUP	(u_int32_t)0xe00000fb	/* 224.0.0.251 */

#ifdef __USE_MISC

struct sf_hdtr;

__BEGIN_DECLS

extern int bsd_sendfile (int __in_fd, int __out_sock,
			 __off_t __in_offset, size_t __nbytes,
			 struct sf_hdtr *__hdtr, __off_t *__sockbytes,
			 int __flags) __THROW;

__END_DECLS

#endif /* Use BSD */

#endif	/* bits/socket.h */
