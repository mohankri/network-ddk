
#define	IFNAME_MAX	16

enum {
	DPDK_IF_TYPE_NET = 0x1,
	DPDK_IF_TYPE_MAX = 0x2,
};

struct mbuf;
struct hw_if;

typedef struct dpdk_if_ops {
	int (*open)(char *name, int);
	int (*input)(struct hw_if *, struct mbuf **);
	int (*output)(struct hw_if *, struct mbuf *);
	int (*poll)(struct hw_if *, struct timeval *);
	int (*close)(struct hw_if *);
} dpdk_if_ops_t;

typedef struct hw_if {
	unsigned char if_name[IFNAME_MAX];
	dpdk_if_ops_t *ops;
} hw_if_t;

int register_dpdk_interface(void);
int register_interface(int type, dpdk_if_ops_t *);
hw_if_t * dpdk_if_open(char *, int type);
int dpdk_if_input(hw_if_t *hwif, struct mbuf **m);
int hw_if_output(hw_if_t *hwif, struct mbuf *m);
int dpdk_if_poll(hw_if_t *hwif, struct timeval *tv);
int dpdk_if_close(hw_if_t *hwif);

#define hw_if_ouput dpdk_if_output
