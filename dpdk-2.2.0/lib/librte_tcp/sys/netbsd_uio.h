/*	$NetBSD: uio.h,v 1.34 2006/03/01 12:38:32 yamt Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993, 1994
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
 *	@(#)uio.h	8.5 (Berkeley) 2/22/94
 */

#ifndef NETBSD_SYS_UIO_H
#define	NETBSD_SYS_UIO_H

#ifndef __UIO_EXPOSE
#define __UIO_EXPOSE
#endif

#include <sys/netbsd_featuretest.h>
#include <netbsd_string.h>

struct iovec {
	void	*iov_base;	/* Base address. */
	size_t	 iov_len;	/* Length. */
};

// #include "sys/netbsd_ansi.h"

#if 0
#ifndef	off_t
typedef	__off_t		off_t;	/* file offset */
#define	off_t		__off_t
#endif
#endif

enum	uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE		/* from system space */
};

#ifdef __UIO_EXPOSE

struct uio {
	struct	iovec *uio_iov;	/* pointer to array of iovecs */
	int	uio_iovcnt;	/* number of iovecs in array */
	off_t	uio_offset;	/* offset into file this uio corresponds to */
	size_t	uio_resid;	/* residual i/o count */
	enum	uio_rw uio_rw;	/* see above */
	struct	vmspace *uio_vmspace;
};
#define	UIO_SETUP_SYSSPACE(uio)	uio_setup_sysspace(uio)

#endif /* __UIO_EXPOSE */

/*
 * Limits
 */
/* Deprecated: use IOV_MAX from <limits.h> instead. */
#define UIO_MAXIOV	1024		/* max 1K of iov's */

#define UIO_SMALLIOV	8		/* 8 on stack, else malloc */

void uio_setup_sysspace(struct uio *);

#if 0
#ifndef	_KERNEL
#include <sys/netbsd_cdefs.h>

__BEGIN_DECLS
#if defined(_NETBSD_SOURCE)
ssize_t preadv(int, const struct iovec *, int, off_t);
ssize_t pwritev(int, const struct iovec *, int, off_t);
#endif /* _NETBSD_SOURCE */
ssize_t	readv(int, const struct iovec *, int);
ssize_t	writev(int, const struct iovec *, int);
__END_DECLS
#else
int ureadc(int, struct uio *);
#endif /* !_KERNEL */
#endif
static inline int
uiomove(void *buf, size_t n, struct uio *uio)
{
	size_t cnt;
	char *cp = buf;
	struct iovec *iov;

	while (n > 0 && uio->uio_resid) {
		iov = uio->uio_iov;
		cnt = iov->iov_len;

		if (cnt == 0) {
			uio->uio_iov++;
			uio->uio_iovcnt--;
			continue;
		}

		if (cnt > n)
			cnt = n;

		if (uio->uio_rw == UIO_READ) {
			memcpy(iov->iov_base, cp, cnt);
		} else {
			memcpy(cp, iov->iov_base, cnt);
		}

		iov->iov_base = (char *)iov->iov_base + cnt;
		iov->iov_len -= cnt;
		uio->uio_resid -= cnt;
		uio->uio_offset += cnt;
		cp += cnt;
		n -= cnt;
	}

	return 0;
}
#endif /* !NETBSD_SYS_UIO_H */
