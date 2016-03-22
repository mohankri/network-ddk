#include <netbsd_stdio.h>
#include <netbsd_stdlib.h>
#include <sys/netbsd_queue.h>
#include <netbsd_util.h>

#include <sys/netbsd_callout.h>

static uint64_t last_ns = 0;
TAILQ_HEAD(, callout_t) cl_sched_q = TAILQ_HEAD_INITIALIZER(cl_sched_q);

void callout_init(callout_t *cs, u_int flags)
{
	memset(cs, 0, sizeof(callout_t));
	// TAILQ_INIT(cs->c_list);
}

void	callout_startup(void)
{
}
void	callout_hardclock(void)
{
}

void	callout_destroy(callout_t *c)
{
	assert((c->c_flags & CALLOUT_PENDING) == 0);
}

void	callout_setfunc(callout_t *c, void (*f)(void *), void *d)
{
	c->c_func = f;
	c->c_arg = d;
}

void	callout_reset(callout_t *c, unsigned long long to_ticks, void (*f)(void *), void *d)
{
	c->c_func = f;
	c->c_arg = d;
	
	callout_schedule(c, to_ticks);
}

void	callout_schedule(callout_t *c, unsigned long long to_ticks)
{
	uint64_t uptime_ns = getuptimenanosec();
	callout_t *cl, *cl_next;
	int scheduled = 0;

	if ((c->c_flags & CALLOUT_PENDING) != 0) {
		/* Remove & re-add */
		TAILQ_REMOVE(&cl_sched_q, c, c_list);
	}
	// printf("Adding timer with ticks: %llu\n", to_ticks);
	c->c_flags &= ~(CALLOUT_FIRED | CALLOUT_INVOKING);
	c->c_time = uptime_ns + (uint64_t)to_ticks;
	
	// printf("Scheduling: %p for: %llu\n", c, c->c_time);
	TAILQ_FOREACH_SAFE(cl, &cl_sched_q, c_list, cl_next) {
		if (cl->c_time > c->c_time) {
			TAILQ_INSERT_BEFORE(cl, c, c_list);
			scheduled = 1;
			break;
		}
	}
	if (c->c_time < last_ns) {
		last_ns = c->c_time;
	}
	if (!scheduled) {
		TAILQ_INSERT_TAIL(&cl_sched_q, c, c_list);
	}
	c->c_flags |= CALLOUT_PENDING;
}

bool	callout_stop(callout_t *c)
{
	int expired;

	if ((c->c_flags & CALLOUT_PENDING) != 0)
		TAILQ_REMOVE(&cl_sched_q, c, c_list);

	expired = ((c->c_flags & CALLOUT_FIRED) != 0);
	c->c_flags &= ~(CALLOUT_PENDING| CALLOUT_FIRED);

	// printf("Stopped: %p\n", c);
	return expired;
}
bool	callout_halt(callout_t *c, void *d)
{
	int expired;

	expired = ((c->c_flags & CALLOUT_FIRED) != 0);
	
	if ((c->c_flags & CALLOUT_PENDING) != 0)
		TAILQ_REMOVE(&cl_sched_q, c, c_list);

	c->c_flags &= ~(CALLOUT_PENDING | CALLOUT_FIRED);

	// printf("Stopped(halt): %p\n", c);
	return expired;
}
bool	callout_pending(callout_t *c)
{
	return ((c->c_flags & CALLOUT_PENDING) != 0);
}
bool	callout_expired(callout_t *c)
{
	return ((c->c_flags & CALLOUT_FIRED) != 0);
}
bool	callout_active(callout_t *c)
{
	return ((c->c_flags & (CALLOUT_FIRED|CALLOUT_PENDING)) != 0);
}

bool	callout_invoking(callout_t *c)
{
	return ((c->c_flags & CALLOUT_INVOKING) != 0);
}
void	callout_ack(callout_t *c)
{
	c->c_flags &= ~CALLOUT_INVOKING;
}

void
callout_run(void)
{
	callout_t *cl, *cl_next;
	uint64_t uptime_ns = getuptimenanosec();
	TAILQ_HEAD(, callout_t) cl_run_q = TAILQ_HEAD_INITIALIZER(cl_run_q);
	int end = 1;
	
	if (!last_ns || uptime_ns < last_ns) {
		return;
	}
	//printf("Current time: %llu\n", uptime_ns);
	TAILQ_FOREACH_SAFE(cl, &cl_sched_q, c_list, cl_next) {
		if (cl->c_time > uptime_ns) {
			last_ns = cl->c_time;
			end = 0;
			break;
		}
		TAILQ_REMOVE(&cl_sched_q, cl, c_list);
		
		// printf("Adding timer to run queue: %p, time: %llu\n", cl,
			// cl->c_time);

		TAILQ_INSERT_TAIL(&cl_run_q, cl, c_list);
		cl->c_flags = (cl->c_flags & ~CALLOUT_PENDING) | (CALLOUT_INVOKING |
			CALLOUT_FIRED);
	}

	if (end) {
		last_ns = 0;
	}

	TAILQ_FOREACH_SAFE(cl, &cl_run_q, c_list, cl_next) {
		cl->c_func(cl->c_arg);
		// printf("Running timer..: %p\n", cl);
	}
}
//void	callout_bind(callout_t *, struct cpu_info *)
//{
//}
