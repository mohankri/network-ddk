/*	$NetBSD: accf_data.c,v 1.6 2009/09/02 14:56:57 tls Exp $	*/

/*-
 * Copyright (c) 2000 Alfred Perlstein <alfred@FreeBSD.org>
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/netbsd_cdefs.h>

#define ACCEPT_FILTER_MOD
#include <sys/netbsd_queue.h>
#include <sys/netbsd_socketvar.h>
#include <netinet/netbsd_accept_filter.h>

/* accept filter that holds a socket until data arrives */

static void	sohasdata(struct socket *so, void *arg, int events, int waitflag);

static struct accept_filter accf_data_filter = {
	.accf_name = "dataready",
	.accf_callback = sohasdata,
};

/* XXX pseudo-device */
void	accf_dataattach(int);

void
accf_dataattach(int junk)
{

}
#if 0
static int
accf_dataready_modcmd(modcmd_t cmd, void *arg)
{

	switch (cmd) {
	case MODULE_CMD_INIT:
		return accept_filt_add(&accf_data_filter);

	case MODULE_CMD_FINI:
		return accept_filt_del(&accf_data_filter);

	default:
		return ENOTTY;
	}
}
#endif
static void
sohasdata(struct socket *so, void *arg, int events, int waitflag)
{
#if 0
	if (!soreadable(so))
		return;
	so->so_upcall = NULL;
	so->so_rcv.sb_flags &= ~SB_UPCALL;
	soisconnected(so);
#endif
	return;
}
