#include <stdio.h>

static inline int
arc4random()
{
	return (random());
}

#if 0
static inline void *
kern_malloc(unsigned long size, struct malloc_type *type, int flags)
{
//        return (malloc(size));
}
#endif
