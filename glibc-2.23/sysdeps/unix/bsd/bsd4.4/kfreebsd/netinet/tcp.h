/* netinet/tcp.h
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

/*
 * Copyright (c) 1982, 1986, 1993
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
 *
 *	@(#)tcp.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_TCP_H
#define _NETINET_TCP_H	1

#include <features.h>
#include <sys/queue.h>

/*
 * User-settable options (used with setsockopt).
 */
#define	TCP_NODELAY	 1	/* Don't delay send to coalesce packets  */
#define	TCP_MAXSEG	 2	/* Set maximum segment size  */
#define TCP_NOPUSH	 4	/* Don't push last block of write  */
#define TCP_NOOPT	 8	/* Don't use TCP options  */
#define TCP_MD5SIG	16	/* use MD5 digests (RFC2385) */
#define TCP_INFO	32	/* retrieve tcp_info structure */
#define TCP_CONGESTION  64	/* get/set congestion control algorithm */

#ifdef __USE_MISC
# include <sys/types.h>

typedef	u_int32_t tcp_seq;
typedef	u_int32_t tcp_cc;	/* connection count, per RFC 1644 */

/* Miscellaneous constants */
#define MAX_SACK_BLKS   6       /* Max # SACK blocks stored at receiver side */   
#define TCP_MAX_SACK    4       /* MAX # SACKs sent in any segment */

/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr
  {
    __extension__ union
    {
      struct
      {
    u_int16_t th_sport;		/* source port */
    u_int16_t th_dport;		/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int8_t th_x2:4;		/* (unused) */
    u_int8_t th_off:4;		/* data offset */
#  endif
#  if __BYTE_ORDER == __BIG_ENDIAN
    u_int8_t th_off:4;		/* data offset */
    u_int8_t th_x2:4;		/* (unused) */
#  endif
    u_int8_t th_flags;
#  define TH_FIN	0x01
#  define TH_SYN	0x02
#  define TH_RST	0x04
#  define TH_PUSH	0x08
#  define TH_ACK	0x10
#  define TH_URG	0x20
    u_int16_t th_win;		/* window */
    u_int16_t th_sum;		/* checksum */
    u_int16_t th_urp;		/* urgent pointer */
};
      struct
      {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
#  elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
#  else
#   error "Adjust your <bits/endian.h> defines"
#  endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};
    };
};

enum
{
  TCP_ESTABLISHED = 1,
  TCP_SYN_SENT,
  TCP_SYN_RECV,
  TCP_FIN_WAIT1,
  TCP_FIN_WAIT2,
  TCP_TIME_WAIT,
  TCP_CLOSE,
  TCP_CLOSE_WAIT,
  TCP_LAST_ACK,
  TCP_LISTEN,
  TCP_CLOSING   /* now a valid state */
};

# define TCPOPT_EOL		0
# define TCPOPT_NOP		1
# define TCPOPT_MAXSEG		2
# define    TCPOLEN_MAXSEG		4
# define TCPOPT_WINDOW		3
# define    TCPOLEN_WINDOW		3
# define TCPOPT_SACK_PERMITTED	4		/* Experimental */
# define    TCPOLEN_SACK_PERMITTED	2
# define TCPOPT_SACK		5		/* Experimental */
# define TCPOPT_TIMESTAMP	8
# define    TCPOLEN_TIMESTAMP	10
# define    TCPOLEN_TSTAMP_APPA	(TCPOLEN_TIMESTAMP+2) /* appendix A */
# define    TCPOPT_TSTAMP_HDR	\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|TCPOPT_TIMESTAMP<<8|TCPOLEN_TIMESTAMP)
#define	TCPOPT_CC		11		/* CC options: RFC-1644 */
#define TCPOPT_CCNEW		12
#define TCPOPT_CCECHO		13
#define	   TCPOLEN_CC			6
#define	   TCPOLEN_CC_APPA		(TCPOLEN_CC+2)
#define	   TCPOPT_CC_HDR(ccopt)		\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|(ccopt)<<8|TCPOLEN_CC)

/*
 * Default maximum segment size for TCP.
 * With an IP MSS of 576, this is 536,
 * but 512 is probably more convenient.
 * This should be defined as MIN(512, IP_MSS - sizeof (struct tcpiphdr)).
 */
# define TCP_MSS	512

/*
 * Default maximum segment size for TCP6.
 * With an IP MSS of 1280, this is 1220,
 * but 1024 is probably more convenient.
 * This should be defined as MIN(1024, IP6_MSS - sizeof (struct tcpip6hdr)).
 */
# define TCP6_MSS	1024

# define TCP_MAXWIN	65535	/* largest value for (unscaled) window */
# define TTCP_CLIENT_SND_WND 4096 /* default send window for T/TCP client */

# define TCP_MAX_WINSHIFT	14	/* maximum window shift */

#define TCP_MAXBURST		4	/* maximum segments in a burst */

#define TCP_MAXHLEN	(0xf<<2)	/* max length of header in bytes */
#define TCP_MAXOLEN	(TCP_MAXHLEN - sizeof (struct tcphdr))
					/* max space left for options */

# define SOL_TCP		6	/* TCP level */


# define TCPI_OPT_TIMESTAMPS	1
# define TCPI_OPT_SACK		2
# define TCPI_OPT_WSCALE	4
# define TCPI_OPT_ECN		8

/* Values for tcpi_state.  */
enum tcp_ca_state
{
  TCP_CA_Open = 0,
  TCP_CA_Disorder = 1,
  TCP_CA_CWR = 2,
  TCP_CA_Recovery = 3,
  TCP_CA_Loss = 4
};

struct tcp_info
{
  u_int8_t	tcpi_state;
  u_int8_t	tcpi_ca_state;
  u_int8_t	tcpi_retransmits;
  u_int8_t	tcpi_probes;
  u_int8_t	tcpi_backoff;
  u_int8_t	tcpi_options;
  u_int8_t	tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

  u_int32_t	tcpi_rto;
  u_int32_t	tcpi_ato;
  u_int32_t	tcpi_snd_mss;
  u_int32_t	tcpi_rcv_mss;

  u_int32_t	tcpi_unacked;
  u_int32_t	tcpi_sacked;
  u_int32_t	tcpi_lost;
  u_int32_t	tcpi_retrans;
  u_int32_t	tcpi_fackets;

  /* Times. */
  u_int32_t	tcpi_last_data_sent;
  u_int32_t	tcpi_last_ack_sent;	/* Not remembered, sorry.  */
  u_int32_t	tcpi_last_data_recv;
  u_int32_t	tcpi_last_ack_recv;

  /* Metrics. */
  u_int32_t	tcpi_pmtu;
  u_int32_t	tcpi_rcv_ssthresh;
  u_int32_t	tcpi_rtt;
  u_int32_t	tcpi_rttvar;
  u_int32_t	tcpi_snd_ssthresh;
  u_int32_t	tcpi_snd_cwnd;
  u_int32_t	tcpi_advmss;
  u_int32_t	tcpi_reordering;
};

#endif /* Misc.  */

#endif /* netinet/tcp.h */
