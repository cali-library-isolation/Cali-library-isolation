/* This header is used on many systems but for GNU we have 
   almost everything defined in the standard header.  */

#include <sys/socket.h>

/*-
 * Copyright (c) 1982, 1986, 1990, 1993
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
 *	@(#)socketvar.h	8.3 (Berkeley) 2/19/95
 *
 * $FreeBSD: src/sys/sys/socketvar.h,v 1.171.2.1.2.1 2009/10/25 01:10:29 kensmith Exp $
 */

#ifndef _SYS_SOCKETVAR_H_
#define _SYS_SOCKETVAR_H_

#include <sys/queue.h>			/* for TAILQ macros */
#include <sys/selinfo.h>		/* for struct selinfo */
#include <sys/_lock.h>
#include <sys/_mutex.h>
#include <sys/_sx.h>

struct socket;
struct vnet;

struct	xsockbuf {
	u_int	sb_cc;
	u_int	sb_hiwat;
	u_int	sb_mbcnt;
	u_int   sb_mcnt;
	u_int   sb_ccnt;
	u_int	sb_mbmax;
	int	sb_lowat;
	int	sb_timeo;
	short	sb_flags;
};

/*
 * Variables for socket buffering.
 */
struct	sockbuf {
	struct	selinfo sb_sel;	/* process selecting read/write */
	struct	mtx sb_mtx;	/* sockbuf lock */
	struct	sx sb_sx;	/* prevent I/O interlacing */
	short	sb_state;	/* (c/d) socket state on sockbuf */
#define	sb_startzero	sb_mb
	struct	mbuf *sb_mb;	/* (c/d) the mbuf chain */
	struct	mbuf *sb_mbtail; /* (c/d) the last mbuf in the chain */
	struct	mbuf *sb_lastrecord;	/* (c/d) first mbuf of last
					 * record in socket buffer */
	struct	mbuf *sb_sndptr; /* (c/d) pointer into mbuf chain */
	u_int	sb_sndptroff;	/* (c/d) byte offset of ptr into chain */
	u_int	sb_cc;		/* (c/d) actual chars in buffer */
	u_int	sb_hiwat;	/* (c/d) max actual char count */
	u_int	sb_mbcnt;	/* (c/d) chars of mbufs used */
	u_int   sb_mcnt;        /* (c/d) number of mbufs in buffer */
	u_int   sb_ccnt;        /* (c/d) number of clusters in buffer */
	u_int	sb_mbmax;	/* (c/d) max chars of mbufs to use */
	u_int	sb_ctl;		/* (c/d) non-data chars in buffer */
	int	sb_lowat;	/* (c/d) low water mark */
	int	sb_timeo;	/* (c/d) timeout for read/write */
	short	sb_flags;	/* (c/d) flags, see below */
	int	(*sb_upcall)(struct socket *, void *, int); /* (c/d) */
	void	*sb_upcallarg;	/* (c/d) */
};


/*
 * Kernel structure per socket.
 * Contains send and receive buffer queues,
 * handle on protocol and pointer to protocol
 * private data and error information.
 */
typedef	u_quad_t so_gen_t;


/*-
 * Locking key to struct socket:
 * (a) constant after allocation, no locking required.
 * (b) locked by SOCK_LOCK(so).
 * (c) locked by SOCKBUF_LOCK(&so->so_rcv).
 * (d) locked by SOCKBUF_LOCK(&so->so_snd).
 * (e) locked by ACCEPT_LOCK().
 * (f) not locked since integer reads/writes are atomic.
 * (g) used only as a sleep/wakeup address, no value.
 * (h) locked by global mutex so_global_mtx.
 */
struct socket {
	int	so_count;		/* (b) reference count */
	short	so_type;		/* (a) generic type, see socket.h */
	short	so_options;		/* from socket call, see socket.h */
	short	so_linger;		/* time to linger while closing */
	short	so_state;		/* (b) internal state flags SS_* */
	int	so_qstate;		/* (e) internal state flags SQ_* */
	void	*so_pcb;		/* protocol control block */
	struct	vnet *so_vnet;		/* network stack instance */
	struct	protosw *so_proto;	/* (a) protocol handle */
/*
 * Variables for connection queuing.
 * Socket where accepts occur is so_head in all subsidiary sockets.
 * If so_head is 0, socket is not related to an accept.
 * For head socket so_incomp queues partially completed connections,
 * while so_comp is a queue of connections ready to be accepted.
 * If a connection is aborted and it has so_head set, then
 * it has to be pulled out of either so_incomp or so_comp.
 * We allow connections to queue up based on current queue lengths
 * and limit on number of queued connections for this socket.
 */
	struct	socket *so_head;	/* (e) back pointer to listen socket */
	TAILQ_HEAD(, socket) so_incomp;	/* (e) queue of partial unaccepted connections */
	TAILQ_HEAD(, socket) so_comp;	/* (e) queue of complete unaccepted connections */
	TAILQ_ENTRY(socket) so_list;	/* (e) list of unaccepted connections */
	u_short	so_qlen;		/* (e) number of unaccepted connections */
	u_short	so_incqlen;		/* (e) number of unaccepted incomplete
					   connections */
	u_short	so_qlimit;		/* (e) max number queued connections */
	short	so_timeo;		/* (g) connection timeout */
	u_short	so_error;		/* (f) error affecting connection */
	struct	sigio *so_sigio;	/* [sg] information for async I/O or
					   out of band data (SIGURG) */
	u_long	so_oobmark;		/* (c) chars to oob mark */
	TAILQ_HEAD(, aiocblist) so_aiojobq; /* AIO ops waiting on socket */

	struct sockbuf so_rcv, so_snd;

	struct	ucred *so_cred;		/* (a) user credentials */
	struct	label *so_label;	/* (b) MAC label for socket */
	struct	label *so_peerlabel;	/* (b) cached MAC label for peer */
	/* NB: generation count must not be first. */
	so_gen_t so_gencnt;		/* (h) generation count */
	void	*so_emuldata;		/* (b) private data for emulators */
 	struct so_accf {
		struct	accept_filter *so_accept_filter;
		void	*so_accept_filter_arg;	/* saved filter args */
		char	*so_accept_filter_str;	/* saved user args */
	} *so_accf;
	int so_fibnum;		/* routing domain for this socket */
};

/*
 * Externalized form of struct socket used by the sysctl(3) interface.
 */
struct xsocket {
	size_t	xso_len;	/* length of this structure */
	struct	socket *xso_so;	/* makes a convenient handle sometimes */
	short	so_type;
	short	so_options;
	short	so_linger;
	short	so_state;
	caddr_t	so_pcb;		/* another convenient handle */
	int	xso_protocol;
	int	xso_family;
	u_short	so_qlen;
	u_short	so_incqlen;
	u_short	so_qlimit;
	short	so_timeo;
	u_short	so_error;
	pid_t	so_pgid;
	u_long	so_oobmark;
	struct xsockbuf so_rcv, so_snd;
	uid_t	so_uid;		/* XXX */
};

#endif /* !_SYS_SOCKETVAR_H_ */
