/*      $NetBSD: bswap.h,v 1.16 2009/08/08 21:23:15 christos Exp $      */

/* Written by Manuel Bouyer. Public domain */

#ifndef NETBSD_SYS_BSWAP_H
#define NETBSD_SYS_BSWAP_H

//#ifndef _LOCORE
#include <sys/netbsd_cdefs.h>
#include <netbsd_types.h>

__BEGIN_DECLS
/* Always declare the functions in case their address is taken (etc) */
#if defined(_KERNEL) || defined(_STANDALONE) || !defined(__BSWAP_RENAME)
uint16_t bswap16(uint16_t);
uint32_t bswap32(uint32_t);
#else
uint16_t bswap16(uint16_t) __RENAME(__bswap16);
uint32_t bswap32(uint32_t) __RENAME(__bswap32);
#endif
uint64_t bswap64(uint64_t);
__END_DECLS

//#if defined(__GNUC__) && defined(__OPTIMIZE__) && !defined(__lint__)

/* machine/byte_swap.h might have defined inline versions */
#ifndef __BYTE_SWAP_U64_VARIABLE
#define	__BYTE_SWAP_U64_VARIABLE bswap_64
#endif

#ifndef __BYTE_SWAP_U32_VARIABLE
#define	__BYTE_SWAP_U32_VARIABLE bswap_32
#endif

#ifndef __BYTE_SWAP_U16_VARIABLE
#define	__BYTE_SWAP_U16_VARIABLE bswap_16
#endif

#define	__byte_swap_u64_constant(x) \
	(__CAST(uint64_t, \
	 ((((x) & 0xff00000000000000ull) >> 56) | \
	  (((x) & 0x00ff000000000000ull) >> 40) | \
	  (((x) & 0x0000ff0000000000ull) >> 24) | \
	  (((x) & 0x000000ff00000000ull) >>  8) | \
	  (((x) & 0x00000000ff000000ull) <<  8) | \
	  (((x) & 0x0000000000ff0000ull) << 24) | \
	  (((x) & 0x000000000000ff00ull) << 40) | \
	  (((x) & 0x00000000000000ffull) << 56))))

#define	__byte_swap_u32_constant(x) \
	(__CAST(uint32_t, \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))))

#define	__byte_swap_u16_constant(x) \
	(__CAST(uint16_t, \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))))

#define	bswap64(x) \
	(__builtin_constant_p((x)) ? \
	 __byte_swap_u64_constant(x) : __BYTE_SWAP_U64_VARIABLE(x))

#define	bswap32(x) \
	(__builtin_constant_p((x)) ? \
	 __byte_swap_u32_constant(x) : __BYTE_SWAP_U32_VARIABLE(x))
#define	bswap16(x) \
	(__builtin_constant_p((x)) ? \
	 __byte_swap_u16_constant(x) : __BYTE_SWAP_U16_VARIABLE(x))

// #endif /* __GNUC__ && __OPTIMIZE__ */
// #endif /* !_LOCORE */

#endif /* !NETBSD_SYS_BSWAP_H */
