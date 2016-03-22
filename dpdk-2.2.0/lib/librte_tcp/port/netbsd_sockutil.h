#ifndef NETBSD_SOCKUTIL_H
#define NETBSD_SOCKUTIL_H

#include <sys/netbsd_socket.h>
#include <netinet/netbsd_in.h>

const char *inet_ntoa(struct in_addr in);
int inet_pton(int af, const char *src, void *dst);

#endif
