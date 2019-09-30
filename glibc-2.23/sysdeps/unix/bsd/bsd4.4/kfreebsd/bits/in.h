/* Copyright (C) 1997, 2000, 2002 Free Software Foundation, Inc.
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

/* FreeBSD version.  */

#ifndef _NETINET_IN_H
# error "Never use <bits/in.h> directly; include <netinet/in.h> instead."
#endif


/* Link numbers.  */
#define	IMPLINK_IP		155
#define	IMPLINK_LOWEXPER	156
#define	IMPLINK_HIGHEXPER	158

#define IPPROTO_DIVERT          258             /* divert pseudo-protocol */

/* To select the IP level.  */
#define SOL_IP	0

/*
 * Options for use with [gs]etsockopt at the IP level.
 * First word of comment is data type; bool is stored in int.
 */
#define	IP_OPTIONS		1    /* buf/ip_opts; set/get IP options */
#define	IP_HDRINCL		2    /* int; header is included with data */
#define	IP_TOS			3    /* int; IP type of service and preced. */
#define	IP_TTL			4    /* int; IP time to live */
#define	IP_RECVOPTS		5    /* bool; receive all IP opts w/dgram */
#define	IP_RECVRETOPTS		6    /* bool; receive IP opts for response */
#define	IP_RECVDSTADDR		7    /* bool; receive IP dst addr w/dgram */
#define	IP_SENDSRCADDR		IP_RECVDSTADDR /* cmsg_type to set src addr */
#define	IP_RETOPTS		8    /* ip_opts; set/get IP options */
#define	IP_MULTICAST_IF		9    /* struct in_addr *or* struct ip_mreqn;
				      * set/get IP multicast i/f  */
#define	IP_MULTICAST_TTL	10   /* u_char; set/get IP multicast ttl */
#define	IP_MULTICAST_LOOP	11   /* u_char; set/get IP multicast loopback */
#define	IP_ADD_MEMBERSHIP	12   /* ip_mreq; add an IP group membership */
#define	IP_DROP_MEMBERSHIP	13   /* ip_mreq; drop an IP group membership */
#define	IP_MULTICAST_VIF	14   /* set/get IP mcast virt. iface */
#define	IP_RSVP_ON		15   /* enable RSVP in kernel */
#define	IP_RSVP_OFF		16   /* disable RSVP in kernel */
#define	IP_RSVP_VIF_ON		17   /* set RSVP per-vif socket */
#define	IP_RSVP_VIF_OFF		18   /* unset RSVP per-vif socket */
#define	IP_PORTRANGE		19   /* int; range to choose for unspec port */
#define	IP_RECVIF		20   /* bool; receive reception if w/dgram */
/* for IPSEC */
#define	IP_IPSEC_POLICY		21   /* int; set/get security policy */
#define	IP_FAITH		22   /* bool; accept FAITH'ed connections */

#define	IP_ONESBCAST		23   /* bool: send all-ones broadcast */
#define	IP_NONLOCALOK		24   /* bool: allow bind to spoof non-local addresses;
					requires kernel compile option IP_NONLOCALBIND */

#define	IP_FW_TABLE_ADD		40   /* add entry */
#define	IP_FW_TABLE_DEL		41   /* delete entry */
#define	IP_FW_TABLE_FLUSH	42   /* flush table */
#define	IP_FW_TABLE_GETSIZE	43   /* get table size */
#define	IP_FW_TABLE_LIST	44   /* list table contents */

#define	IP_FW_ADD		50   /* add a firewall rule to chain */
#define	IP_FW_DEL		51   /* delete a firewall rule from chain */
#define	IP_FW_FLUSH		52   /* flush firewall rule chain */
#define	IP_FW_ZERO		53   /* clear single/all firewall counter(s) */
#define	IP_FW_GET		54   /* get entire firewall rule chain */
#define	IP_FW_RESETLOG		55   /* reset logging counters */

#define IP_FW_NAT_CFG           56   /* add/config a nat rule */
#define IP_FW_NAT_DEL           57   /* delete a nat rule */
#define IP_FW_NAT_GET_CONFIG    58   /* get configuration of a nat rule */
#define IP_FW_NAT_GET_LOG       59   /* get log of a nat rule */

#define	IP_DUMMYNET_CONFIGURE	60   /* add/configure a dummynet pipe */
#define	IP_DUMMYNET_DEL		61   /* delete a dummynet pipe from chain */
#define	IP_DUMMYNET_FLUSH	62   /* flush dummynet */
#define	IP_DUMMYNET_GET		64   /* get entire dummynet pipes */

#define	IP_RECVTTL		65   /* bool; receive IP TTL w/dgram */
#define	IP_MINTTL		66   /* minimum TTL for packet or drop */
#define	IP_DONTFRAG		67   /* don't fragment packet */

/* IPv4 Source Filter Multicast API [RFC3678] */
#define	IP_ADD_SOURCE_MEMBERSHIP	70   /* join a source-specific group */
#define	IP_DROP_SOURCE_MEMBERSHIP	71   /* drop a single source */
#define	IP_BLOCK_SOURCE			72   /* block a source */
#define	IP_UNBLOCK_SOURCE		73   /* unblock a source */

/* The following option is private; do not use it from user applications. */
#define	IP_MSFILTER			74   /* set/get filter list */

/* Protocol Independent Multicast API [RFC3678] */
#define	MCAST_JOIN_GROUP		80   /* join an any-source group */
#define	MCAST_LEAVE_GROUP		81   /* leave all sources for group */
#define	MCAST_JOIN_SOURCE_GROUP		82   /* join a source-specific group */
#define	MCAST_LEAVE_SOURCE_GROUP	83   /* leave a single source */
#define	MCAST_BLOCK_SOURCE		84   /* block a source */
#define	MCAST_UNBLOCK_SOURCE		85   /* unblock a source */

/*
 * Defaults and limits for options
 */
#define	IP_DEFAULT_MULTICAST_TTL  1	/* normally limit m'casts to 1 hop  */
#define	IP_DEFAULT_MULTICAST_LOOP 1	/* normally hear sends if a member  */

/*
 * The imo_membership vector for each socket is now dynamically allocated at
 * run-time, bounded by USHRT_MAX, and is reallocated when needed, sized
 * according to a power-of-two increment.
 */
#define	IP_MIN_MEMBERSHIPS	31
#define	IP_MAX_MEMBERSHIPS	4095
#define	IP_MAX_SOURCE_FILTER	1024	/* # of filters per socket, per group */

/*
 * Filter modes; also used to represent per-socket filter mode internally.
 */
 
#define MCAST_UNDEFINED 0       /* fmode: not yet defined */
#define	MCAST_INCLUDE	1	/* fmode: include these source(s) */
#define	MCAST_EXCLUDE	2	/* fmode: exclude these source(s) */

/*
 * Argument for IP_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
#define	IP_PORTRANGE_DEFAULT	0	/* default range */
#define	IP_PORTRANGE_HIGH	1	/* "high" - request firewall bypass */
#define	IP_PORTRANGE_LOW	2	/* "low" - vouchsafe security */

/*
 * Names for IP sysctl objects
 */
#define	IPCTL_FORWARDING	1	/* act as router */
#define	IPCTL_SENDREDIRECTS	2	/* may send redirects when forwarding */
#define	IPCTL_DEFTTL		3	/* default TTL */
#ifdef notyet
#define	IPCTL_DEFMTU		4	/* default MTU */
#endif
#define IPCTL_RTEXPIRE		5	/* cloned route expiration time */
#define IPCTL_RTMINEXPIRE	6	/* min value for expiration time */
#define IPCTL_RTMAXCACHE	7	/* trigger level for dynamic expire */
#define	IPCTL_SOURCEROUTE	8	/* may perform source routes */
#define	IPCTL_DIRECTEDBROADCAST	9	/* may re-broadcast received packets */
#define IPCTL_INTRQMAXLEN	10	/* max length of netisr queue */
#define	IPCTL_INTRQDROPS	11	/* number of netisr q drops */
#define	IPCTL_STATS		12	/* ipstat structure */
#define	IPCTL_ACCEPTSOURCEROUTE	13	/* may accept source routed packets */
#define	IPCTL_FASTFORWARDING	14	/* use fast IP forwarding code */
#define	IPCTL_KEEPFAITH		15	/* FAITH IPv4->IPv6 translater ctl */
#define	IPCTL_GIF_TTL		16	/* default TTL for gif encap packet */
#define	IPCTL_MAXID		17

/* Structure used to describe IP options for IP_OPTIONS and IP_RETOPTS.
   The `ip_dst' field is used for the first-hop gateway when using a
   source route (this gets put into the header proper).  */
struct ip_opts
  {
    struct in_addr ip_dst;	/* First hop; zero without source route.  */
    char ip_opts[40];		/* Actually variable in size.  */
  };

/* Options for use with `getsockopt' and `setsockopt' at the IPv6 level.
   The first word in the comment at the right is the data type used;
   "bool" means a boolean value stored in an `int'.  */
#define IPV6_SOCKOPT_RESERVED1	3  /* reserved for future use */
#define IPV6_UNICAST_HOPS	4  /* int; IP6 hops */
#define IPV6_MULTICAST_IF	9  /* u_int; set/get IP6 multicast i/f  */
#define IPV6_MULTICAST_HOPS	10 /* int; set/get IP6 multicast hops */
#define IPV6_MULTICAST_LOOP	11 /* u_int; set/get IP6 multicast loopback */
#define IPV6_JOIN_GROUP		12 /* ip6_mreq; join a group membership */
#define IPV6_LEAVE_GROUP	13 /* ip6_mreq; leave a group membership */
#define IPV6_PORTRANGE		14 /* int; range to choose for unspec port */
#define ICMP6_FILTER		18 /* icmp6_filter; icmp6 filter */

#define IPV6_CHECKSUM		26 /* int; checksum offset for raw socket */
#define IPV6_V6ONLY		27 /* bool; make AF_INET6 sockets v6 only */

#define IPV6_IPSEC_POLICY	28 /* struct; get/set security policy */
#define IPV6_FAITH		29 /* bool; accept FAITH'ed connections */

#define IPV6_FW_ADD		30 /* add a firewall rule to chain */
#define IPV6_FW_DEL		31 /* delete a firewall rule from chain */
#define IPV6_FW_FLUSH		32 /* flush firewall rule chain */
#define IPV6_FW_ZERO		33 /* clear single/all firewall counter(s) */
#define IPV6_FW_GET		34 /* get entire firewall rule chain */
#define IPV6_RTHDRDSTOPTS	35 /* ip6_dest; send dst option before rthdr */

#define IPV6_RECVPKTINFO	36 /* bool; recv if, dst addr */
#define IPV6_RECVHOPLIMIT	37 /* bool; recv hop limit */
#define IPV6_RECVRTHDR		38 /* bool; recv routing header */
#define IPV6_RECVHOPOPTS	39 /* bool; recv hop-by-hop option */
#define IPV6_RECVDSTOPTS	40 /* bool; recv dst option after rthdr */

#define IPV6_USE_MIN_MTU	42 /* bool; send packets at the minimum MTU */
#define IPV6_RECVPATHMTU	43 /* bool; notify an according MTU */
#define IPV6_PATHMTU		44 /* mtuinfo; get the current path MTU (sopt),
				      4 bytes int; MTU notification (cmsg) */

#define IPV6_PKTINFO		46 /* in6_pktinfo; send if, src addr */
#define IPV6_HOPLIMIT		47 /* int; send hop limit */
#define IPV6_NEXTHOP		48 /* sockaddr; next hop addr */
#define IPV6_HOPOPTS		49 /* ip6_hbh; send hop-by-hop option */
#define IPV6_DSTOPTS		50 /* ip6_dest; send dst option befor rthdr */
#define IPV6_RTHDR		51 /* ip6_rthdr; send routing header */

#define IPV6_RECVTCLASS		57 /* bool; recv traffic class values */

#define IPV6_AUTOFLOWLABEL	59 /* bool; attach flowlabel automagically */

#define IPV6_TCLASS		61 /* int; send traffic class value */
#define IPV6_DONTFRAG		62 /* bool; disable IPv6 fragmentation */

#define IPV6_PREFER_TEMPADDR	63 /* int; prefer temporary addresses as
                                    * the source address.
				    */

/* Obsolete synonyms for the above.  */
#define IPV6_ADD_MEMBERSHIP	IPV6_JOIN_GROUP
#define IPV6_DROP_MEMBERSHIP	IPV6_LEAVE_GROUP
#define IPV6_RXHOPOPTS		IPV6_HOPOPTS
#define IPV6_RXDSTOPTS		IPV6_DSTOPTS

/* Socket level values for IPv6.  */
#define SOL_IPV6	41
#define SOL_ICMPV6	58

/*
 * Defaults and limits for options
 */
#define IPV6_DEFAULT_MULTICAST_HOPS 1   /* normally limit m'casts to 1 hop */
#define IPV6_DEFAULT_MULTICAST_LOOP 1   /* normally hear sends if a member */

/*
 * Argument for IPV6_PORTRANGE:
 * - which range to search when port is unspecified at bind() or connect()
 */
#define IPV6_PORTRANGE_DEFAULT  0       /* default range */
#define IPV6_PORTRANGE_HIGH     1       /* "high" - request firewall bypass */
#define IPV6_PORTRANGE_LOW      2       /* "low" - vouchsafe security */

/* Routing header options for IPv6.  */
#define IPV6_RTHDR_LOOSE	0	/* Hop doesn't need to be neighbour. */
#define IPV6_RTHDR_STRICT	1	/* Hop must be a neighbour.  */

#define IPV6_RTHDR_TYPE_0	0	/* IPv6 Routing header type 0.  */

/*
 * Names for IP sysctl objects
 */
#define IPV6CTL_FORWARDING	1	/* act as router */
#define IPV6CTL_SENDREDIRECTS	2	/* may send redirects when forwarding*/
#define IPV6CTL_DEFHLIM		3	/* default Hop-Limit */
#define IPV6CTL_FORWSRCRT	5	/* forward source-routed dgrams */
#define IPV6CTL_STATS		6	/* stats */
#define IPV6CTL_MRTSTATS	7	/* multicast forwarding stats */
#define IPV6CTL_MRTPROTO	8	/* multicast routing protocol */
#define IPV6CTL_MAXFRAGPACKETS	9	/* max packets reassembly queue */
#define IPV6CTL_SOURCECHECK	10	/* verify source route and intf */
#define IPV6CTL_SOURCECHECK_LOGINT 11	/* minimume logging interval */
#define IPV6CTL_ACCEPT_RTADV	12
#define IPV6CTL_KEEPFAITH	13
#define IPV6CTL_LOG_INTERVAL	14
#define IPV6CTL_HDRNESTLIMIT	15
#define IPV6CTL_DAD_COUNT	16
#define IPV6CTL_AUTO_FLOWLABEL	17
#define IPV6CTL_DEFMCASTHLIM	18
#define IPV6CTL_GIF_HLIM	19	/* default HLIM for gif encap packet */
#define IPV6CTL_KAME_VERSION	20
#define IPV6CTL_USE_DEPRECATED	21	/* use deprecated addr (RFC2462 5.5.4) */
#define IPV6CTL_RR_PRUNE	22	/* walk timer for router renumbering */
#define IPV6CTL_V6ONLY		24
#define IPV6CTL_RTEXPIRE	25	/* cloned route expiration time */
#define IPV6CTL_RTMINEXPIRE	26	/* min value for expiration time */
#define IPV6CTL_RTMAXCACHE	27	/* trigger level for dynamic expire */

#define IPV6CTL_USETEMPADDR	32	/* use temporary addresses (RFC3041) */
#define IPV6CTL_TEMPPLTIME	33	/* preferred lifetime for tmpaddrs */
#define IPV6CTL_TEMPVLTIME	34	/* valid lifetime for tmpaddrs */
#define IPV6CTL_AUTO_LINKLOCAL	35	/* automatic link-local addr assign */
#define IPV6CTL_RIP6STATS	36	/* raw_ip6 stats */
#define IPV6CTL_PREFER_TEMPADDR	37	/* prefer temporary addr as src */
#define IPV6CTL_ADDRCTLPOLICY	38	/* get/set address selection policy */
#define IPV6CTL_USE_DEFAULTZONE	39	/* use default scope zone */

#define IPV6CTL_MAXFRAGS	41	/* max fragments */
#define IPV6CTL_MCAST_PMTU	44	/* enable pMTU discovery for multicast? */

/* New entries should be added here from current IPV6CTL_MAXID value. */
/* to define items, should talk with KAME guys first, for *BSD compatibility */
/* 42-44 is already used in KAME */
#define IPV6CTL_STEALTH		45
#define	ICMPV6CTL_ND6_ONLINKNSRFC4861	47
#define IPV6CTL_MAXID		48
