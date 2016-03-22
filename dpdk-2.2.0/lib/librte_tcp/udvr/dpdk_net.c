#include <stdio.h>
#include <sys/time.h>

#include "dpdk_net.h"

static dpdk_if_ops_t dpdk_if_ops = {
	.open = dpdk_if_open,
	.input = dpdk_if_input,
	.output = hw_if_output,
	.poll = dpdk_if_poll,
	.close = dpdk_if_close,
};

int
register_dpdk_interface(void)
{
	int rc;
	if ((rc = register_interface(DPDK_IF_TYPE_NET,
					&dpdk_if_ops)) < 0) {
		return rc; 
	}
	ifinit1();
	ifinit();
	soinit();
	domaininit(0);
	printf("%s called\n", __FUNCTION__);
	return (0);
}
