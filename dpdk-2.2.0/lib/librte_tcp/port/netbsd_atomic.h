#ifndef NETBSD_ATOMIC_H
#define NETBSD_ATOMIC_H

static inline unsigned int atomic_inc_uint(unsigned int *a)
{
	(*a)++;
}

#define atomic_dec_uint_nv atomic_dec_uint
static inline unsigned int atomic_dec_uint(unsigned int *a)
{
	(*a)--;
}
#endif
