#ifndef NETBSD_MUTEX_H
#define NETBSD_MUTEX_H
#include <pthread.h>

#define splvm()		0
#define splnet() 	0
#define splsoftnet()	0
#define splx(s) 	do{ } while (0)

typedef pthread_mutex_t kmutex_t;
typedef pthread_cond_t kcondvar_t;

#define MUTEX_DEFAULT		1

static inline int mutex_init(kmutex_t *mtx, int type)
{
	return 0; //pthread_mutex_init(mtx, NULL);
}

static inline int mutex_enter(kmutex_t *mtx)
{
	return 0; //pthread_mutex_lock(mtx);
}

static inline int mutex_exit(kmutex_t *mtx)
{
	return 0; //pthread_mutex_unlock(mtx);
}

#endif
