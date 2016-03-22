#ifndef NETBSD_UTIL_H
#define NETBSD_UTIL_H
#include <assert.h>
// #include <netbsd_types.h>

#define aprint_error(...)
#define aprint_error_dev(...)
#define aprint_verbose_dev(...)
#define log(...)	
#define min(a,b)	((a < b) ? a : b)
#define max(a,b)	((a > b) ? a : b)

#define MAX		max
#define MIN		min

#define panic(...)	assert(0)
#define KASSERT(x)	assert(x)
#define KDASSERT(x)	assert(x)
#define CTASSERT(x)	assert(x)

#define time_second	uptime()

#include <sys/sysinfo.h>
#include <string.h>
#include <sys/time.h>

#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#define	roundup2(x, m)	(((x) + (m) - 1) & ~((m) - 1))
#define ALIGNBYTES	(sizeof(long) - 1)
#define ALIGN(p)	(((u_long)(p) + ALIGNBYTES) &~ALIGNBYTES)

#if 0
static inline unsigned int uptime(void)
{
	struct sysinfo si;

	sysinfo(&si);

	return si.uptime;
	rc = clock_gettime(CLOCK_MONOTONIC, &ts);
}
#endif

#define KERNEL_LOCK(...)	do { } while (0)
#define KERNEL_UNLOCK_ONE(...)  do { } while (0)

static inline size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t len = strlen(src);

	if (len < size) {
		memcpy(dest, src, len + 1);
	} else {
		if (size) {
			memcpy(dest, src, size - 1);
			dest[size - 1] = 0;
		}
	}
	return len;
}

static inline void getnanotime(struct timespec *ts)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec;

	return;
}

unsigned int uptime(void);

unsigned long long getuptimenanosec(void);
unsigned long long rdtsc(void);

#define copyout(x,y,l)	memcpy(y, x, l)

#endif
