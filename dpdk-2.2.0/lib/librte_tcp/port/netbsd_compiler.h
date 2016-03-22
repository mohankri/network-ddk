#ifndef NETBSD_COMPILER_H
#define NETBSD_COMPILER_H
// #include <linux/compiler.h>

#include <sys/cdefs.h>
#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)
#define __predict_false	unlikely
#define __predict_true	likely

#endif
