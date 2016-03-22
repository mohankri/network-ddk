#ifndef NETBSD_TYPES_H
#define NETBSD_TYPES_H
#include <stdint.h>
#include <sys/types.h>

typedef void 		*paddr_t;
#ifndef BOOL
#define BOOL
typedef unsigned char bool;
#endif
// typedef uint16_t	device_t;
typedef unsigned long vaddr_t;
typedef unsigned long vsize_t;

#endif
