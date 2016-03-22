#ifndef NETBSD_SOCKINTF_H
#define NETBSD_SOCKINTF_H

#include <sys/netbsd_socket.h>
#include <sys/netbsd_socketvar.h>
#include <netinet/netbsd_in.h>

int sogetsockname(struct socket *so, struct sockaddr *sa, socklen_t *sa_len);
int sogetpeername(struct socket *so, struct sockaddr *sa, socklen_t *sa_len);
int so_createnew_listenport (int dom, struct socket **so, int portno,
	struct sockaddr_in *isa, int backlog,
	int (*accept_cb)(struct socket *, struct socket *),
	ssize_t (*recv_cb)(struct socket *, struct mbuf *m));

int so_createnew_connectport(int dom, struct socket **so, int portno,
	struct sockaddr_in *isa, int (*connect_cb)(struct socket *),
	ssize_t (*recv_cb)(struct socket *, struct mbuf *m));
#endif
