#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <net/netbsd_if.h>
#include <sys/netbsd_mbuf.h>

#include "dpdk_net.h"

dpdk_if_ops_t *hw_if_ops[DPDK_IF_TYPE_MAX];
struct	ifnet gifnet[32];

int
dpdk_if_close(hw_if_t *hwif)
{
	free(hwif);
	return (0);
}

void
init_ifnet(struct ifnet *ifnet)
{
	uint8_t mac[6] = {0x0, 0x15, 0x17, 0xB1, 0x46, 0xEC};
	char *ip = "172.16.1.252";
	memset(ifnet, 0, sizeof(struct ifnet));
	if_attach(ifnet);
	ether_ifattach(ifnet, mac);
	set_ipaddr(ifnet, ip);
	ifnet->if_csum_flags_rx = M_CSUM_TCPv4;
	printf("attach %s with %02X %02X %02X %02X %02X %02X\n", ip, mac[0],
			mac[1], mac[2], mac[3], mac[4], mac[5]);
	return;
}

hw_if_t *
dpdk_if_open(char *name, int type)
{
	hw_if_t *hwif;
	if (type >= DPDK_IF_TYPE_MAX) {
		return NULL;
	}
	hwif = calloc(1, sizeof(hw_if_t));
	if (!hwif) {
		return NULL;
	}
	snprintf(hwif->if_name, IFNAME_MAX-1, "%s", name);
	hwif->ops = hw_if_ops[type];
	init_ifnet(&gifnet[0]);
	printf("%s called %s %d\n", __FUNCTION__, name, type);
	return (hwif);
}

int
dpdk_if_input(hw_if_t *hwif, struct mbuf **m)
{
	printf("%s called \n", __FUNCTION__);
	printf("%s called %p\n", __FUNCTION__, hwif);
	return (0);
}

int
hw_if_output(hw_if_t *hwif, struct mbuf *m)
{
	printf("%s called \n", __FUNCTION__);
	printf("%s called %p\n", __FUNCTION__, hwif);
	return (0);
}

int
dpdk_if_poll(hw_if_t *hwif, struct timeval *tv)
{
	printf("%s called \n", __FUNCTION__);
	printf("%s called %p\n", __FUNCTION__, hwif);
	return (0);
}

int
register_interface(int type, dpdk_if_ops_t *ops)
{
	if (type >= DPDK_IF_TYPE_MAX) {
		return -EINVAL;
	}
	hw_if_ops[type] = ops;
	return (0);
}
