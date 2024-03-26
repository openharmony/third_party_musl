#include <unistd.h>
#include "pthread_impl.h"
#include "lock.h"

int pthread_kill(pthread_t t, int sig)
{
	int r;
	sigset_t set;
	/* Block not just app signals, but internal ones too, since
	 * pthread_kill is used to implement pthread_cancel, which
	 * must be async-cancel-safe. */
	__block_all_sigs(&set);
	LOCK(t->killlock);
#ifndef __LITEOS__
	r = t->tid ? -__syscall(__NR_tgkill, getpid(), t->tid, sig)
		: (sig+0U >= _NSIG ? EINVAL : 0);
#else
	r = t->tid ? -__syscall(SYS_tkill, t->tid, sig)
		: (sig+0U >= _NSIG ? EINVAL : 0);
#endif
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}
