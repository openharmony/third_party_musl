#include <time.h>
#include <setjmp.h>
#include <limits.h>
#include "pthread_impl.h"
#include "atomic.h"

struct ksigevent {
	union sigval sigev_value;
	int sigev_signo;
	int sigev_notify;
	int sigev_tid;
};

struct start_args {
	volatile int b;
	struct sigevent *sev;
};

/*
* barrier val:
* 0: init state
* 2: child thread arrive: when child thread is the first thread to arrive, will wait, otherwise, will wake
* 3: parent thread arrive: when parent thread is the first thread to arrive, will wait, otherwise, will wake
* 4: parent thread should wait for the child to finish synchronizing (state: CHILD_DONE)
* 5: child thread done
*/
enum ThreadState {
	INIT_STATE = 0,
	CHILD_ARRIVE = 2,
	PARENT_ARRIVE,
	PARENT_WAIT,
	CHILD_DONE,
};

static void dummy_0()
{
}
weak_alias(dummy_0, __pthread_tsd_run_dtors);

static void cleanup_fromsig(void *p)
{
	pthread_t self = __pthread_self();
	__pthread_tsd_run_dtors();
#ifdef FEATURE_PTHREAD_CANCEL
	self->cancel = 0;
	self->canceldisable = 0;
	self->cancelasync = 0;
#endif
	self->cancelbuf = 0;
	__reset_tls();
	longjmp(p, 1);
}

static void __child_sync(volatile int *barrier)
{
	if (a_swap(barrier, CHILD_ARRIVE) == INIT_STATE) {
		__wait(barrier, 0, CHILD_ARRIVE, 0);
	} else {
		__wake(barrier, 1, 0);
	}

	a_swap(barrier, CHILD_DONE);
	__wake(barrier, 1, 0);
}

static void __parent_sync(volatile int *barrier)
{
	if (a_swap(barrier, PARENT_ARRIVE) == CHILD_ARRIVE) {
		__wake(barrier, 1, 0);
	} else {
		__wait(barrier, 0, PARENT_ARRIVE, 0);
	}

	if (a_swap(barrier, PARENT_WAIT) != CHILD_DONE) {
		__wait(barrier, 0, PARENT_WAIT, 0);
	}
}

static void *start(void *arg)
{
	pthread_t self = __pthread_self();
	struct start_args *args = arg;
	jmp_buf jb;

	void (*notify)(union sigval) = args->sev->sigev_notify_function;
	union sigval val = args->sev->sigev_value;

	__child_sync(&args->b);
#ifdef FEATURE_PTHREAD_CANCEL
	if (self->cancel)
		return 0;
#endif
	for (;;) {
		siginfo_t si;
		while (sigwaitinfo(SIGTIMER_SET, &si) < 0);
		if (si.si_code == SI_TIMER && !setjmp(jb)) {
#ifndef HWASAN_REMOVE_CLEANUP
			pthread_cleanup_push(cleanup_fromsig, jb);
#endif
			notify(val);
#ifndef HWASAN_REMOVE_CLEANUP
			pthread_cleanup_pop(1);
#endif
		}
		if (self->timer_id < 0) break;
	}
	__syscall(SYS_timer_delete, self->timer_id & INT_MAX);
	return 0;
}

int timer_create(clockid_t clk, struct sigevent *restrict evp, timer_t *restrict res)
{
	static volatile int init = 0;
	pthread_t td;
	pthread_attr_t attr;
	int r;
	struct start_args args;
	struct ksigevent ksev, *ksevp=0;
	int timerid;
	sigset_t set;

	switch (evp ? evp->sigev_notify : SIGEV_SIGNAL) {
	case SIGEV_NONE:
	case SIGEV_SIGNAL:
	case SIGEV_THREAD_ID:
		if (evp) {
			ksev.sigev_value = evp->sigev_value;
			ksev.sigev_signo = evp->sigev_signo;
			ksev.sigev_notify = evp->sigev_notify;
			if (evp->sigev_notify == SIGEV_THREAD_ID)
				ksev.sigev_tid = evp->sigev_notify_thread_id;
			else
				ksev.sigev_tid = 0;
			ksevp = &ksev;
		}
		if (syscall(SYS_timer_create, clk, ksevp, &timerid) < 0)
			return -1;
		*res = (void *)(intptr_t)timerid;
		break;
	case SIGEV_THREAD:
		if (!init) {
			struct sigaction sa = { .sa_handler = SIG_DFL };
			__libc_sigaction(SIGTIMER, &sa, 0);
			a_store(&init, 1);
		}
		if (evp->sigev_notify_attributes)
			attr = *evp->sigev_notify_attributes;
		else
			pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		a_store(&args.b, 0);
		args.sev = evp;

		__block_app_sigs(&set);
		__syscall(SYS_rt_sigprocmask, SIG_BLOCK, SIGTIMER_SET, 0, _NSIG/8);
		r = pthread_create(&td, &attr, start, &args);
		__restore_sigs(&set);
		if (r) {
			errno = r;
			return -1;
		}

		ksev.sigev_value.sival_ptr = 0;
		ksev.sigev_signo = SIGTIMER;
		ksev.sigev_notify = SIGEV_THREAD_ID;
		ksev.sigev_tid = td->tid;
		if (syscall(SYS_timer_create, clk, &ksev, &timerid) < 0) {
			timerid = -1;
#ifdef FEATURE_PTHREAD_CANCEL
			td->cancel = 1;
#endif
		}
		td->timer_id = timerid;
		__parent_sync(&args.b);
		if (timerid < 0) return -1;
		*res = (void *)(INTPTR_MIN | (uintptr_t)td>>1);
		break;
	default:
		errno = EINVAL;
		return -1;
	}

	return 0;
}
