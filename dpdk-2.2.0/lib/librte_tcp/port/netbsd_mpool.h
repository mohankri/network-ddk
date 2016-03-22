#ifndef NETBSD_MPOOL_H
#define NETBSD_MPOOL_H

typedef struct mpool {
	int n_bufs;
	void **b_list;
	int n_avail_bufs;
	int b_size;
} mpool_t;

mpool_t *mpool_create(int nbufs, int size);

void *mpool_get(mpool_t *mp);

void mpool_put(mpool_t *mp, void *buf);
#endif
