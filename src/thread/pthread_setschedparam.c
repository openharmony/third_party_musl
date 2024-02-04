#include "pthread_impl.h"
#include "lock.h"

int pthread_setschedparam(pthread_t t, int policy, const struct sched_param *param)
{
	int r;
	sigset_t set;
	__block_app_sigs(&set);
	LOCK(t->killlock);
#ifdef __LITEOS_A__
	r = !t->tid ? ESRCH : -__syscall(SYS_sched_setscheduler, t->tid, policy, param, MUSL_TYPE_THREAD);
#else
	r = !t->tid ? ESRCH : -__syscall(SYS_sched_setscheduler, t->tid, policy, param);
#endif
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}
