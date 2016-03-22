#include <netbsd_stdio.h>
#include <netbsd_stdlib.h>

#include <netbsd_mpool.h>

mpool_t *mpool_create(int nbufs, int size)
{
	mpool_t *mp;

	mp = malloc(sizeof(mpool_t));
	if (mp == NULL) {
		return NULL;
	}
	mp->b_list = (void **)calloc(nbufs, sizeof(void *));
	if (mp->b_list == NULL) {
		return NULL;
	}
	mp->n_bufs = nbufs;
	mp->n_avail_bufs = 0;
	mp->b_size = size;

	printf("Created mpool of # bufs: %d, size: %d\n", nbufs, size);
	return mp;
}

void *mpool_get(mpool_t *mp)
{
	if (mp->n_avail_bufs) {
		// printf("Returing buf at: %p\n", mp->b_list[mp->n_avail_bufs - 1]);
		return mp->b_list[--mp->n_avail_bufs];
	} else {
		return calloc(1, mp->b_size);
	}
}

void mpool_put(mpool_t *mp, void *buf)
{
	if (mp->n_avail_bufs < mp->n_bufs) {
		memset(buf, 0, mp->b_size);
		mp->b_list[mp->n_avail_bufs++] = buf;
		//printf("Adding to free buf: %p\n", mp->b_list[mp->n_avail_bufs - 1]);
	} else {
		//printf("Freeing buf: %p\n", buf);
		free(buf);
	}
}
