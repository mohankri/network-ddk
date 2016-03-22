#ifndef NETBSD_CONST_H
#define NETBSD_CONST_H

#define NBBY		8 /* Number of bits in a byte.. */
#define M_NOWAIT	0x0001
#define M_WAITOK	0x0000

#define MSIZE		512

#define MCLSHIFT	11
#define MCLBYTES	(1 << MCLSHIFT)

#define		HZ	1000000000
#define		TRUE 	1
#define 	FALSE 	0
#define 	true	TRUE
#define 	false 	FALSE

extern const unsigned char	hexdigits[];

#define imin(a,b) min(a,b)
#define lmin(a,b) min(a,b)

#define ETHER_ALIGN	2

#endif
