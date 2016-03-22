#include <sys/netbsd_socketvar.h>
#include <sys/netbsd_socket.h>
#include <sys/netbsd_mbuf.h>
#include <netinet/netbsd_in.h>
#include <netbsd_errno.h>
#include <netbsd_sockintf.h>
#include <sys/netbsd_uio.h>
#include <netbsd_limits.h>

/*
 * Create new socket identifier for the listening port of interest. The
 * connect * callback function will be called with the existing socket 
 * id & new socket id will be called when a new connection is recieved
 */
int
so_createnew_listenport (int dom, struct socket **so, int portno,
	struct sockaddr_in *isa, int backlog,
	int (*accept_cb)(struct socket *, struct socket *), ssize_t
	(*recv_cb)(struct socket *, struct mbuf *m))
{
	int rc;
	struct mbuf *m;
	struct socket *new_so;
	struct sockaddr *sa;
	int len;

	/* Create basic new socket */
	rc = socreate(AF_INET, so, SOCK_STREAM, 0, NULL, NULL);
	if (rc < 0) {
		return rc;
	}
	new_so = *so;
	new_so->accept_cb = accept_cb;
	new_so->recv_cb = recv_cb;

	m = m_get(M_WAIT, MT_SONAME);
	len = m->m_len = sizeof(struct sockaddr);
	memcpy(mtod(m, void *), (void *)isa, len);

	sa = mtod(m, struct sockaddr *);
	sa->sa_len = sizeof(struct sockaddr);
	rc = sobind(new_so, m, NULL); 
	if (rc != 0) {
		sofree(new_so);
		m_free(m);
		return (rc);
	}

	rc = solisten(new_so, backlog, NULL);
	if (rc != 0) {
		sofree(new_so);
		m_free(m);
		return (rc);
	}
	m_free(m);
	return 0;
}

int
so_createnew_connectport(int dom, struct socket **so, int portno,
	struct sockaddr_in *isa, int (*connect_cb)(struct socket *),
	ssize_t (*recv_cb)(struct socket *, struct mbuf *m))
{
	int rc;
	struct socket *new_so;
	struct sockaddr *sa;
	int len;
	struct mbuf *m;

	rc = socreate(AF_INET, so, SOCK_STREAM, 0, NULL, NULL);
	if (rc < 0) {
		return rc;
	}
	
	new_so = *so;
	new_so->connect_cb = connect_cb;
	new_so->recv_cb = recv_cb;

	m = m_get(M_WAIT, MT_SONAME);
	len = m->m_len = sizeof(struct sockaddr);
	memcpy(mtod(m, void *), (void *)isa, len);
	
	sa = mtod(m, struct sockaddr *);
	sa->sa_len = sizeof(struct sockaddr);
	rc = soconnect(new_so, m , NULL);
	if (rc != 0) {
		sofree(new_so);
		m_free(m);
		*so = NULL;
		return (rc);
	}
	m_free(m);
	
	return 0;
}	

static int
_sogetsockname(struct socket *so, int which, struct sockaddr *sa, socklen_t *sa_len)
{
	struct mbuf *m;
	int rc;

	m = m_getclr(M_WAIT, MT_SONAME);
	MCLAIM(m, so->so_mowner);
	
	if (which == PRU_PEERADDR && (so->so_state &(SS_ISCONNECTED |
		SS_ISCONFIRMING)) == 0) {
		rc = ENOTCONN;
	} else {
		rc = (*so->so_proto->pr_usrreq)(so, which, NULL, m, NULL,
			NULL);
	}
	
	if (rc != 0) {
		m_free(m);
		return rc;
	}
	
	if (*sa_len > m->m_len) {
		*sa_len = m->m_len;
	}
	memcpy(sa, mtod(m, void *), *sa_len);

	return 0;
}

int
sogetsockname(struct socket *so, struct sockaddr *sa, socklen_t *sa_len)
{
	return _sogetsockname(so, PRU_SOCKADDR, sa, sa_len);
}

int
sogetpeername(struct socket *so, struct sockaddr *sa, socklen_t *sa_len)
{
	return _sogetsockname(so, PRU_PEERADDR, sa, sa_len);
}

ssize_t
soreadv(struct socket *so, struct iovec *iovp, int iovcnt)
{
	struct uio auio;
	int rc;
	ssize_t cnt;
	int i;
	int flags = MSG_DONTWAIT;

	auio.uio_iov = iovp;
	auio.uio_iovcnt = iovcnt;
	auio.uio_rw = UIO_READ;
	auio.uio_vmspace = NULL;

	auio.uio_resid = 0;
	for (i = 0; i < iovcnt; i++, iovp++) {
		auio.uio_resid += iovp->iov_len;
		if (iovp->iov_len > SSIZE_MAX || auio.uio_resid > SSIZE_MAX) {
			return -EINVAL;
		}
	}
	cnt = auio.uio_resid;
	if (so->so_receive) {
		rc = so->so_receive(so, NULL, &auio, NULL, NULL, &flags);
		if (rc != 0) {
			return -rc;
		}
	}
	
	return (cnt - auio.uio_resid);
}

ssize_t sowritev(struct socket *so, struct iovec *iovp, int iovcnt)
{
	struct uio auio;
	int i;
	int cnt;
	int rc;

	auio.uio_iov = iovp;
	auio.uio_iovcnt = iovcnt;
	auio.uio_rw = UIO_WRITE;
	auio.uio_vmspace = NULL;

	auio.uio_resid = 0;
	for (i = 0; i < iovcnt; i++, iovp++) {
		auio.uio_resid += iovp->iov_len;
		if (iovp->iov_len > SSIZE_MAX || auio.uio_resid > SSIZE_MAX) {
			return -EINVAL;
		}
	}
	
	cnt = auio.uio_resid;
	if (so->so_send) {
		rc = so->so_send(so, NULL, &auio, NULL, NULL, 0);
		if (rc != 0) {
			return -rc;
		}
	}
	
	return (cnt);
}
