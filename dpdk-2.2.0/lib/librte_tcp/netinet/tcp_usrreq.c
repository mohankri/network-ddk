/*	$NetBSD: tcp_usrreq.c,v 1.157 2009/09/16 15:23:05 pooka Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*-
 * Copyright (c) 1997, 1998, 2005, 2006 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe and Kevin M. Lahey of the Numerical Aerospace Simulation
 * Facility, NASA Ames Research Center.
 * This code is derived from software contributed to The NetBSD Foundation
 * by Charles M. Hannum.
 * This code is derived from software contributed to The NetBSD Foundation
 * by Rui Paulo.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 1982, 1986, 1988, 1993, 1995
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
 * 3. Neither the name of the University nor the names of its contributors
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
 *	@(#)tcp_usrreq.c	8.5 (Berkeley) 6/21/95
 */

#include <opt_inet.h>
#include <netbsd_string.h>
#include <netbsd_types.h>
//#include <sys/netbsd_systm.h>
#include <sys/netbsd_mbuf.h>
#include <sys/netbsd_socket.h>
#include <sys/netbsd_socketvar.h>
#include <sys/netbsd_protosw.h>
#include <netbsd_errno.h>
#include <sys/netbsd_domain.h>
#include <sys/netbsd_uidinfo.h>

#include <net/netbsd_if.h>
#include <net/netbsd_route.h>

#include <netinet/netbsd_in.h>
#include <netinet/netbsd_in_systm.h>
#include <netinet/netbsd_in_var.h>
#include <netinet/netbsd_ip.h>
#include <netinet/netbsd_in_pcb.h>
#include <netinet/netbsd_ip_var.h>
#include <netinet/netbsd_in_offload.h>

#ifdef INET6
#ifndef INET
#include <netinet/netbsd_in.h>
#endif
#include <netinet/ip6.h>
#include <netinet6/in6_pcb.h>
#include <netinet6/ip6_var.h>
#include <netinet6/scope6_var.h>
#endif

#include <netinet/netbsd_tcp.h>
#include <netinet/netbsd_tcp_fsm.h>
#include <netinet/netbsd_tcp_seq.h>
#include <netinet/netbsd_tcp_timer.h>
#include <netinet/netbsd_tcp_var.h>
//#include <netinet/netbsd_tcp_private.h>
#include <netinet/netbsd_tcp_congctl.h>
#include <netinet/netbsd_tcpip.h>
#include <netinet/netbsd_tcp_debug.h>

#include "opt_tcp_space.h"

#ifdef IPSEC
#include <netinet6/ipsec.h>
#endif /*IPSEC*/

/*
 * TCP protocol interface to socket abstraction.
 */

int	hz = HZ;
/*
 * Process a TCP user request for TCP tb.  If this is a send request
 * then m is the mbuf chain of send data.  If this is a timer expiration
 * (called from the software clock routine), then timertype tells which timer.
 */
/*ARGSUSED*/
int
tcp_usrreq(struct socket *so, int req,
    struct mbuf *m, struct mbuf *nam, struct mbuf *control, struct lwp *l)
{
	struct inpcb *inp;
#ifdef INET6
	struct in6pcb *in6p;
#endif
	struct tcpcb *tp = NULL;
	int s;
	int error = 0;
#ifdef TCP_DEBUG
	int ostate = 0;
#endif
	int family;	/* family of the socket */

	family = so->so_proto->pr_domain->dom_family;

	if (req == PRU_CONTROL) {
		switch (family) {
#ifdef INET
		case PF_INET:
			return (in_control(so, (long)m, (void *)nam,
			    (struct ifnet *)control));
#endif
#ifdef INET6
		case PF_INET6:
			return (in6_control(so, (long)m, (void *)nam,
			    (struct ifnet *)control));
#endif
		default:
			return EAFNOSUPPORT;
		}
	}

	s = splsoftnet();

	if (req == PRU_PURGEIF) {
		mutex_enter(softnet_lock);
		//new_softnet_mutex_enter();
		switch (family) {
#ifdef INET
		case PF_INET:
			in_pcbpurgeif0(&tcbtable, (struct ifnet *)control);
			in_purgeif((struct ifnet *)control);
			in_pcbpurgeif(&tcbtable, (struct ifnet *)control);
			break;
#endif
#ifdef INET6
		case PF_INET6:
			in6_pcbpurgeif0(&tcbtable, (struct ifnet *)control);
			in6_purgeif((struct ifnet *)control);
			in6_pcbpurgeif(&tcbtable, (struct ifnet *)control);
			break;
#endif
		default:
			//new_softnet_mutex_exit();
			mutex_exit(softnet_lock);
			splx(s);
			return (EAFNOSUPPORT);
		}
		//new_softnet_mutex_exit();
		mutex_exit(softnet_lock);
		splx(s);
		return (0);
	}

	if (req == PRU_ATTACH)
		sosetlock(so);

	switch (family) {
#ifdef INET
	case PF_INET:
		inp = sotoinpcb(so);
#ifdef INET6
		in6p = NULL;
#endif
		break;
#endif
#ifdef INET6
	case PF_INET6:
		inp = NULL;
		in6p = sotoin6pcb(so);
		break;
#endif
	default:
		splx(s);
		return EAFNOSUPPORT;
	}

#ifdef DIAGNOSTIC
#ifdef INET6
	if (inp && in6p)
		panic("tcp_usrreq: both inp and in6p set to non-NULL");
#endif
	if (req != PRU_SEND && req != PRU_SENDOOB && control)
		panic("tcp_usrreq: unexpected control mbuf");
#endif
	/*
	 * When a TCP is attached to a socket, then there will be
	 * a (struct inpcb) pointed at by the socket, and this
	 * structure will point at a subsidary (struct tcpcb).
	 */
#ifndef INET6
	if (inp == 0 && req != PRU_ATTACH)
#else
	if ((inp == 0 && in6p == 0) && req != PRU_ATTACH)
#endif
	{
		error = EINVAL;
		goto release;
	}
#ifdef INET
	if (inp) {
		tp = intotcpcb(inp);
		/* WHAT IF TP IS 0? */
#ifdef KPROF
		tcp_acounts[tp->t_state][req]++;
#endif
#ifdef TCP_DEBUG
		ostate = tp->t_state;
#endif
	}
#endif
#ifdef INET6
	if (in6p) {
		tp = in6totcpcb(in6p);
		/* WHAT IF TP IS 0? */
#ifdef KPROF
		tcp_acounts[tp->t_state][req]++;
#endif
#ifdef TCP_DEBUG
		ostate = tp->t_state;
#endif
	}
#endif

	switch (req) {

	/*
	 * TCP attaches to socket via PRU_ATTACH, reserving space,
	 * and an internet control block.
	 */
	case PRU_ATTACH:
#ifndef INET6
		if (inp != 0)
#else
		if (inp != 0 || in6p != 0)
#endif
		{
			error = EISCONN;
			break;
		}
		error = tcp_attach(so);
		if (error)
			break;
		if ((so->so_options & SO_LINGER) && so->so_linger == 0)
			so->so_linger = TCP_LINGERTIME;
		tp = sototcpcb(so);
		break;

	/*
	 * PRU_DETACH detaches the TCP protocol from the socket.
	 */
	case PRU_DETACH:
		tp = tcp_disconnect(tp);
		break;

	/*
	 * Give the socket an address.
	 */
	case PRU_BIND:
		switch (family) {
#ifdef INET
		case PF_INET:
			error = in_pcbbind(inp, nam);
			break;
#endif
#ifdef INET6
		case PF_INET6:
			error = in6_pcbbind(in6p, nam, l);
			if (!error) {
				/* mapped addr case */
				if (IN6_IS_ADDR_V4MAPPED(&in6p->in6p_laddr))
					tp->t_family = AF_INET;
				else
					tp->t_family = AF_INET6;
			}
			break;
#endif
		}
		break;

	/*
	 * Prepare to accept connections.
	 */
	case PRU_LISTEN:
#ifdef INET
		if (inp && inp->inp_lport == 0) {
			error = in_pcbbind(inp, NULL);
			if (error)
				break;
		}
#endif
#ifdef INET6
		if (in6p && in6p->in6p_lport == 0) {
			error = in6_pcbbind(in6p, NULL, l);
			if (error)
				break;
		}
#endif
		tp->t_state = TCPS_LISTEN;
		break;

	/*
	 * Initiate connection to peer.
	 * Create a template for use in transmissions on this connection.
	 * Enter SYN_SENT state, and mark socket as connecting.
	 * Start keep-alive timer, and seed output sequence space.
	 * Send initial segment on connection.
	 */
	case PRU_CONNECT:
#ifdef INET
		if (inp) {
			if (inp->inp_lport == 0) {
				error = in_pcbbind(inp, NULL);
				if (error)
					break;
			}
			error = in_pcbconnect(inp, nam);
		}
#endif
#ifdef INET6
		if (in6p) {
			if (in6p->in6p_lport == 0) {
				error = in6_pcbbind(in6p, NULL, l);
				if (error)
					break;
			}
			error = in6_pcbconnect(in6p, nam, l);
			if (!error) {
				/* mapped addr case */
				if (IN6_IS_ADDR_V4MAPPED(&in6p->in6p_faddr))
					tp->t_family = AF_INET;
				else
					tp->t_family = AF_INET6;
			}
		}
#endif
		if (error)
			break;
		tp->t_template = tcp_template(tp);
		if (tp->t_template == 0) {
#ifdef INET
			if (inp)
				in_pcbdisconnect(inp);
#endif
#ifdef INET6
			if (in6p)
				in6_pcbdisconnect(in6p);
#endif
			error = ENOBUFS;
			break;
		}
		/*
		 * Compute window scaling to request.
		 * XXX: This should be moved to tcp_output().
		 */
		while (tp->request_r_scale < TCP_MAX_WINSHIFT &&
		    (TCP_MAXWIN << tp->request_r_scale) < sb_max)
			tp->request_r_scale++;
		soisconnecting(so);
		// TCP_STATINC(TCP_STAT_CONNATTEMPT);
		tp->t_state = TCPS_SYN_SENT;
		TCP_TIMER_ARM(tp, TCPT_KEEP, tp->t_keepinit);
		tp->iss = tcp_new_iss(tp, 0);
		tcp_sendseqinit(tp);
		error = tcp_output(tp);
		break;

	/*
	 * Create a TCP connection between two sockets.
	 */
	case PRU_CONNECT2:
		error = EOPNOTSUPP;
		break;

	/*
	 * Initiate disconnect from peer.
	 * If connection never passed embryonic stage, just drop;
	 * else if don't need to let data drain, then can just drop anyways,
	 * else have to begin TCP shutdown process: mark socket disconnecting,
	 * drain unread data, state switch to reflect user close, and
	 * send segment (e.g. FIN) to peer.  Socket will be really disconnected
	 * when peer sends FIN and acks ours.
	 *
	 * SHOULD IMPLEMENT LATER PRU_CONNECT VIA REALLOC TCPCB.
	 */
	case PRU_DISCONNECT:
		tp = tcp_disconnect(tp);
		break;

	/*
	 * Accept a connection.  Essentially all the work is
	 * done at higher levels; just return the address
	 * of the peer, storing through addr.
	 */
	case PRU_ACCEPT:
#ifdef INET
		if (inp)
			in_setpeeraddr(inp, nam);
#endif
#ifdef INET6
		if (in6p)
			in6_setpeeraddr(in6p, nam);
#endif
		break;

	/*
	 * Mark the connection as being incapable of further output.
	 */
	case PRU_SHUTDOWN:
		socantsendmore(so);
		tp = tcp_usrclosed(tp);
		if (tp)
			error = tcp_output(tp);
		break;

	/*
	 * After a receive, possibly send window update to peer.
	 */
	case PRU_RCVD:
		/*
		 * soreceive() calls this function when a user receives
		 * ancillary data on a listening socket. We don't call
		 * tcp_output in such a case, since there is no header
		 * template for a listening socket and hence the kernel
		 * will panic.
		 */
		if ((so->so_state & (SS_ISCONNECTED|SS_ISCONNECTING)) != 0)
			(void) tcp_output(tp);
		break;

	/*
	 * Do a send by putting data in output queue and updating urgent
	 * marker if URG set.  Possibly send more data.
	 */
	case PRU_SEND:
		if (control && control->m_len) {
			m_freem(control);
			m_freem(m);
			error = EINVAL;
			break;
		}
		sbappendstream(&so->so_snd, m);
		error = tcp_output(tp);
		break;

	/*
	 * Abort the TCP.
	 */
	case PRU_ABORT:
		tp = tcp_drop(tp, ECONNABORTED);
		break;

	case PRU_SENSE:
		/*
		 * stat: don't bother with a blocksize.
		 */
		splx(s);
		return (0);

	case PRU_RCVOOB:
		if (control && control->m_len) {
			m_freem(control);
			m_freem(m);
			error = EINVAL;
			break;
		}
		if ((so->so_oobmark == 0 &&
		    (so->so_state & SS_RCVATMARK) == 0) ||
		    so->so_options & SO_OOBINLINE ||
		    tp->t_oobflags & TCPOOB_HADDATA) {
			error = EINVAL;
			break;
		}
		if ((tp->t_oobflags & TCPOOB_HAVEDATA) == 0) {
			error = EWOULDBLOCK;
			break;
		}
		m->m_len = 1;
		*mtod(m, char *) = tp->t_iobc;
		if (((long)nam & MSG_PEEK) == 0)
			tp->t_oobflags ^= (TCPOOB_HAVEDATA | TCPOOB_HADDATA);
		break;

	case PRU_SENDOOB:
		if (sbspace(&so->so_snd) < -512) {
			m_freem(m);
			error = ENOBUFS;
			break;
		}
		/*
		 * According to RFC961 (Assigned Protocols),
		 * the urgent pointer points to the last octet
		 * of urgent data.  We continue, however,
		 * to consider it to indicate the first octet
		 * of data past the urgent section.
		 * Otherwise, snd_up should be one lower.
		 */
		sbappendstream(&so->so_snd, m);
		tp->snd_up = tp->snd_una + so->so_snd.sb_cc;
		tp->t_force = 1;
		error = tcp_output(tp);
		tp->t_force = 0;
		break;

	case PRU_SOCKADDR:
#ifdef INET
		if (inp)
			in_setsockaddr(inp, nam);
#endif
#ifdef INET6
		if (in6p)
			in6_setsockaddr(in6p, nam);
#endif
		break;

	case PRU_PEERADDR:
#ifdef INET
		if (inp)
			in_setpeeraddr(inp, nam);
#endif
#ifdef INET6
		if (in6p)
			in6_setpeeraddr(in6p, nam);
#endif
		break;

	default:
		panic("tcp_usrreq");
	}
#ifdef TCP_DEBUG
	if (tp && (so->so_options & SO_DEBUG))
		tcp_trace(TA_USER, ostate, tp, NULL, req);
#endif

release:
	splx(s);
	return (error);
}

static void
change_keepalive(struct socket *so, struct tcpcb *tp)
{
	tp->t_maxidle = tp->t_keepcnt * tp->t_keepintvl;
	TCP_TIMER_DISARM(tp, TCPT_KEEP);
	TCP_TIMER_DISARM(tp, TCPT_2MSL);

	if (tp->t_state == TCPS_SYN_RECEIVED ||
	    tp->t_state == TCPS_SYN_SENT) {
		TCP_TIMER_ARM(tp, TCPT_KEEP, tp->t_keepinit);
	} else if (so->so_options & SO_KEEPALIVE && 
	    tp->t_state <= TCPS_CLOSE_WAIT) {
		TCP_TIMER_ARM(tp, TCPT_KEEP, tp->t_keepintvl);
	} else {
		TCP_TIMER_ARM(tp, TCPT_KEEP, tp->t_keepidle);
	}

	if ((tp->t_state == TCPS_FIN_WAIT_2) && (tp->t_maxidle > 0))
		TCP_TIMER_ARM(tp, TCPT_2MSL, tp->t_maxidle);
}


int
tcp_ctloutput(int op, struct socket *so, struct sockopt *sopt)
{
	int error = 0, s;
	struct inpcb *inp;
#ifdef INET6
	struct in6pcb *in6p;
#endif
	struct tcpcb *tp;
	u_int ui;
	int family;	/* family of the socket */
	int level, optname, optval;

	level = sopt->sopt_level;
	optname = sopt->sopt_name;

	family = so->so_proto->pr_domain->dom_family;

	s = splsoftnet();
	switch (family) {
#ifdef INET
	case PF_INET:
		inp = sotoinpcb(so);
#ifdef INET6
		in6p = NULL;
#endif
		break;
#endif
#ifdef INET6
	case PF_INET6:
		inp = NULL;
		in6p = sotoin6pcb(so);
		break;
#endif
	default:
		splx(s);
		panic("%s: af %d", __func__, family);
	}
#ifndef INET6
	if (inp == NULL)
#else
	if (inp == NULL && in6p == NULL)
#endif
	{
		splx(s);
		return (ECONNRESET);
	}
	if (level != IPPROTO_TCP) {
		switch (family) {
#ifdef INET
		case PF_INET:
			error = ip_ctloutput(op, so, sopt);
			break;
#endif
#ifdef INET6
		case PF_INET6:
			error = ip6_ctloutput(op, so, sopt);
			break;
#endif
		}
		splx(s);
		return (error);
	}
	if (inp)
		tp = intotcpcb(inp);
#ifdef INET6
	else if (in6p)
		tp = in6totcpcb(in6p);
#endif
	else
		tp = NULL;

	switch (op) {
	case PRCO_SETOPT:
		switch (optname) {
#ifdef TCP_SIGNATURE
		case TCP_MD5SIG:
			error = sockopt_getint(sopt, &optval);
			if (error)
				break;
			if (optval > 0)
				tp->t_flags |= TF_SIGNATURE;
			else
				tp->t_flags &= ~TF_SIGNATURE;
			break;
#endif /* TCP_SIGNATURE */

		case TCP_NODELAY:
			error = sockopt_getint(sopt, &optval);
			if (error)
				break;
			if (optval)
				tp->t_flags |= TF_NODELAY;
			else
				tp->t_flags &= ~TF_NODELAY;
			break;

		case TCP_MAXSEG:
			error = sockopt_getint(sopt, &optval);
			if (error)
				break;
			if (optval > 0 && optval <= tp->t_peermss)
				tp->t_peermss = optval; /* limit on send size */
			else
				error = EINVAL;
			break;
#ifdef notyet
		case TCP_CONGCTL:
			/* XXX string overflow XXX */
			error = tcp_congctl_select(tp, sopt->sopt_data);
			break;
#endif

		case TCP_KEEPIDLE:
			error = sockopt_get(sopt, &ui, sizeof(ui));
			if (error)
				break;
			if (ui > 0) {
				tp->t_keepidle = ui;
				change_keepalive(so, tp);
			} else
				error = EINVAL;
			break;

		case TCP_KEEPINTVL:
			error = sockopt_get(sopt, &ui, sizeof(ui));
			if (error)
				break;
			if (ui > 0) {
				tp->t_keepintvl = ui;
				change_keepalive(so, tp);
			} else
				error = EINVAL;
			break;

		case TCP_KEEPCNT:
			error = sockopt_get(sopt, &ui, sizeof(ui));
			if (error)
				break;
			if (ui > 0) {
				tp->t_keepcnt = ui;
				change_keepalive(so, tp);
			} else
				error = EINVAL;
			break;

		case TCP_KEEPINIT:
			error = sockopt_get(sopt, &ui, sizeof(ui));
			if (error)
				break;
			if (ui > 0) {
				tp->t_keepinit = ui;
				change_keepalive(so, tp);
			} else
				error = EINVAL;
			break;

		default:
			error = ENOPROTOOPT;
			break;
		}
		break;

	case PRCO_GETOPT:
		switch (optname) {
#ifdef TCP_SIGNATURE
		case TCP_MD5SIG:
			optval = (tp->t_flags & TF_SIGNATURE) ? 1 : 0;
			error = sockopt_set(sopt, &optval, sizeof(optval));
			break;
#endif
		case TCP_NODELAY:
			optval = tp->t_flags & TF_NODELAY;
			error = sockopt_set(sopt, &optval, sizeof(optval));
			break;
		case TCP_MAXSEG:
			optval = tp->t_peermss;
			error = sockopt_set(sopt, &optval, sizeof(optval));
			break;
#ifdef notyet
		case TCP_CONGCTL:
			break;
#endif
		default:
			error = ENOPROTOOPT;
			break;
		}
		break;
	}
	splx(s);
	return (error);
}

#ifndef TCP_SENDSPACE
#define	TCP_SENDSPACE	1024*1024
#endif
int	tcp_sendspace = TCP_SENDSPACE;
#ifndef TCP_RECVSPACE
#define	TCP_RECVSPACE	1024*1024
#endif
int	tcp_recvspace = TCP_RECVSPACE;

/*
 * Attach TCP protocol to socket, allocating
 * internet protocol control block, tcp control block,
 * bufer space, and entering LISTEN state if to accept connections.
 */
int
tcp_attach(struct socket *so)
{
	struct tcpcb *tp;
	struct inpcb *inp;
#ifdef INET6
	struct in6pcb *in6p;
#endif
	int error;
	int family;	/* family of the socket */

	family = so->so_proto->pr_domain->dom_family;

#ifdef MBUFTRACE
	so->so_mowner = &tcp_sock_mowner;
	so->so_rcv.sb_mowner = &tcp_sock_rx_mowner;
	so->so_snd.sb_mowner = &tcp_sock_tx_mowner;
#endif
	if (so->so_snd.sb_hiwat == 0 || so->so_rcv.sb_hiwat == 0) {
		error = soreserve(so, tcp_sendspace, tcp_recvspace);
		if (error)
			return (error);
	}

	so->so_rcv.sb_flags |= SB_AUTOSIZE;
	so->so_snd.sb_flags |= SB_AUTOSIZE;

	switch (family) {
#ifdef INET
	case PF_INET:
		error = in_pcballoc(so, &tcbtable);
		if (error)
			return (error);
		inp = sotoinpcb(so);
#ifdef INET6
		in6p = NULL;
#endif
		break;
#endif
#ifdef INET6
	case PF_INET6:
		error = in6_pcballoc(so, &tcbtable);
		if (error)
			return (error);
		inp = NULL;
		in6p = sotoin6pcb(so);
		break;
#endif
	default:
		return EAFNOSUPPORT;
	}
	if (inp)
		tp = tcp_newtcpcb(family, (void *)inp);
#ifdef INET6
	else if (in6p)
		tp = tcp_newtcpcb(family, (void *)in6p);
#endif
	else
		tp = NULL;

	if (tp == 0) {
		int nofd = so->so_state & SS_NOFDREF;	/* XXX */

		so->so_state &= ~SS_NOFDREF;	/* don't free the socket yet */
#ifdef INET
		if (inp)
			in_pcbdetach(inp);
#endif
#ifdef INET6
		if (in6p)
			in6_pcbdetach(in6p);
#endif
		so->so_state |= nofd;
		return (ENOBUFS);
	}
	tp->t_state = TCPS_CLOSED;
	return (0);
}

/*
 * Initiate (or continue) disconnect.
 * If embryonic state, just send reset (once).
 * If in ``let data drain'' option and linger null, just drop.
 * Otherwise (hard), mark socket disconnecting and drop
 * current input data; switch states based on user close, and
 * send segment to peer (with FIN).
 */
struct tcpcb *
tcp_disconnect(struct tcpcb *tp)
{
	struct socket *so;

	if (tp->t_inpcb)
		so = tp->t_inpcb->inp_socket;
#ifdef INET6
	else if (tp->t_in6pcb)
		so = tp->t_in6pcb->in6p_socket;
#endif
	else
		so = NULL;

	if (TCPS_HAVEESTABLISHED(tp->t_state) == 0)
		tp = tcp_close(tp);
	else if ((so->so_options & SO_LINGER) && so->so_linger == 0)
		tp = tcp_drop(tp, 0);
	else {
		soisdisconnecting(so);
		sbflush(&so->so_rcv);
		tp = tcp_usrclosed(tp);
		if (tp)
			(void) tcp_output(tp);
	}
	return (tp);
}

/*
 * User issued close, and wish to trail through shutdown states:
 * if never received SYN, just forget it.  If got a SYN from peer,
 * but haven't sent FIN, then go to FIN_WAIT_1 state to send peer a FIN.
 * If already got a FIN from peer, then almost done; go to LAST_ACK
 * state.  In all other cases, have already sent FIN to peer (e.g.
 * after PRU_SHUTDOWN), and just have to play tedious game waiting
 * for peer to send FIN or not respond to keep-alives, etc.
 * We can let the user exit from the close as soon as the FIN is acked.
 */
struct tcpcb *
tcp_usrclosed(struct tcpcb *tp)
{

	switch (tp->t_state) {

	case TCPS_CLOSED:
	case TCPS_LISTEN:
	case TCPS_SYN_SENT:
		tp->t_state = TCPS_CLOSED;
		tp = tcp_close(tp);
		break;

	case TCPS_SYN_RECEIVED:
	case TCPS_ESTABLISHED:
		tp->t_state = TCPS_FIN_WAIT_1;
		break;

	case TCPS_CLOSE_WAIT:
		tp->t_state = TCPS_LAST_ACK;
		break;
	}
	if (tp && tp->t_state >= TCPS_FIN_WAIT_2) {
		struct socket *so;
		if (tp->t_inpcb)
			so = tp->t_inpcb->inp_socket;
#ifdef INET6
		else if (tp->t_in6pcb)
			so = tp->t_in6pcb->in6p_socket;
#endif
		else
			so = NULL;
		if (so)
			soisdisconnected(so);
		/*
		 * If we are in FIN_WAIT_2, we arrived here because the
		 * application did a shutdown of the send side.  Like the
		 * case of a transition from FIN_WAIT_1 to FIN_WAIT_2 after
		 * a full close, we start a timer to make sure sockets are
		 * not left in FIN_WAIT_2 forever.
		 */
		if ((tp->t_state == TCPS_FIN_WAIT_2) && (tp->t_maxidle > 0))
			TCP_TIMER_ARM(tp, TCPT_2MSL, tp->t_maxidle);
	}
	return (tp);
}

static inline int
copyout_uid(struct socket *sockp, void *oldp, size_t *oldlenp)
{
	size_t sz;
	int error;
	uid_t uid;

	uid = sockp->so_uidinfo->ui_uid;
	if (oldp) {
		sz = MIN(sizeof(uid), *oldlenp);
		memcpy(oldp, &uid, sz);
		if (error)
			return error;
	}
	*oldlenp = sizeof(uid);
	return 0;
}

static inline int
inet4_ident_core(struct in_addr raddr, u_int rport,
    struct in_addr laddr, u_int lport,
    void *oldp, size_t *oldlenp,
    struct lwp *l, int dodrop)
{
	struct inpcb *inp;
	struct socket *sockp;

	inp = in_pcblookup_connect(&tcbtable, raddr, rport, laddr, lport);
	
	if (inp == NULL || (sockp = inp->inp_socket) == NULL)
		return ESRCH;

	if (dodrop) {
		struct tcpcb *tp;
		int error;
		
		if (inp == NULL || (tp = intotcpcb(inp)) == NULL ||
		    (inp->inp_socket->so_options & SO_ACCEPTCONN) != 0)
			return ESRCH;

		(void)tcp_drop(tp, ECONNABORTED);
		return 0;
	}
	else
		return copyout_uid(sockp, oldp, oldlenp);
}

#ifdef INET6
static inline int
inet6_ident_core(struct in6_addr *raddr, u_int rport,
    struct in6_addr *laddr, u_int lport,
    void *oldp, size_t *oldlenp,
    struct lwp *l, int dodrop)
{
	struct in6pcb *in6p;
	struct socket *sockp;

	in6p = in6_pcblookup_connect(&tcbtable, raddr, rport, laddr, lport, 0);

	if (in6p == NULL || (sockp = in6p->in6p_socket) == NULL)
		return ESRCH;
	
	if (dodrop) {
		struct tcpcb *tp;
		int error;
		
		if (in6p == NULL || (tp = in6totcpcb(in6p)) == NULL ||
		    (in6p->in6p_socket->so_options & SO_ACCEPTCONN) != 0)
			return ESRCH;

		error = kauth_authorize_network(l->l_cred, KAUTH_NETWORK_SOCKET,
		    KAUTH_REQ_NETWORK_SOCKET_DROP, in6p->in6p_socket, tp, NULL);
		if (error)
			return (error);

		(void)tcp_drop(tp, ECONNABORTED);
		return 0;
	}
	else
		return copyout_uid(sockp, oldp, oldlenp);
}
#endif
#if 0
/*
 * sysctl helper routine for the net.inet.tcp.drop and
 * net.inet6.tcp6.drop nodes.
 */
#define sysctl_net_inet_tcp_drop sysctl_net_inet_tcp_ident

void
tcp_usrreq_init(void)
{

#ifdef INET
	sysctl_net_inet_tcp_setup2(NULL, PF_INET, "inet", "tcp");
#endif
#ifdef INET6
	sysctl_net_inet_tcp_setup2(NULL, PF_INET6, "inet6", "tcp6");
#endif
}
#endif
