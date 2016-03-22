#include <stdio.h>
#include <netbsd_sockutil.h>
#include <sys/netbsd_sockio.h>
#include <net/netbsd_if.h>

static char *new_netmask = "255.255.255.0";

void
set_ipaddr(struct ifnet *ifp, char *ip)
{
        struct sockaddr_in *sin;
        struct ifreq ifr;
        int rc;

        memset(&ifr, 0, sizeof(struct ifreq));

        printf("Setting ip addr: %s\n", ip);
        sin = (struct sockaddr_in *)&(ifr.ifr_addr);
        sin->sin_family = AF_INET;
        sin->sin_len = sizeof(struct sockaddr_in);
        if (inet_pton(AF_INET, ip, &sin->sin_addr) < 0) {
                printf("Error converting ip to hex, ip: %s\n", ip);
                return;
        }
        if ((rc = in_control(0, SIOCSIFADDR, &ifr, ifp)) != 0) {
                printf("Error setting ipaddr: %s, error: %s\n", ip,
                        strerror(rc));
                return;
        }
        memset(&ifr, 0, sizeof(struct ifreq));
        sin = (struct sockaddr_in *)&(ifr.ifr_addr);
        sin->sin_family = AF_INET;
        sin->sin_len = sizeof(struct sockaddr_in);
        if (inet_pton(AF_INET, new_netmask, &sin->sin_addr) < 0) {
                printf("Error converting ip to hex, ip: %s\n", new_netmask);
                return;
        }
        if ((rc = in_control(0, SIOCSIFNETMASK, &ifr, ifp)) != 0) {
                printf("Error setting netmask: %s, error: %s\n", new_netmask,
                        strerror(rc));
                        return;
        }
	return;
}

