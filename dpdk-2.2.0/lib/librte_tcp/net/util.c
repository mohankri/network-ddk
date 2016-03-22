#include <netbsd_stdio.h>
#include <netbsd_linux_time.h>
#include <netbsd_util.h>
// #include <netbsd_types.h>

unsigned int uptime(void)
{
	int rc;
	struct timespec ts;
	unsigned int ns;

	rc = clock_gettime(CLOCK_MONOTONIC, &ts);
	if (rc < 0) {
		assert(0);
	}
	ns = ((unsigned int)ts.tv_sec);

	return ns;
}

unsigned long long getuptimenanosec(void)
{
	int rc;
	struct timespec ts;
	unsigned long long ns;

	rc = clock_gettime(CLOCK_MONOTONIC, &ts);
	if (rc >= 0) {
		ns = ((unsigned long long)ts.tv_sec * 1000000000ULL) +
			(unsigned long long)ts.tv_nsec;
		return ns;
	}

	assert(0);
}

unsigned long long rdtsc(void)
{
	unsigned long hi, lo;
	asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
	return ((unsigned long long)(hi) << 32) | lo;
}
