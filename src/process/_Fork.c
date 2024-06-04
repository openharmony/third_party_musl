#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "syscall.h"
#include "libc.h"
#include "lock.h"
#include "pthread_impl.h"
#include "aio_impl.h"
#ifndef __LITEOS__
#include "proc_xid_impl.h"
#endif
#include "fork_impl.h"


#define UNUSUAL_FORK_COST_TIME_MILLI 100
#define CLOCK_SECOND_TO_MILLI 1000
#define CLOCK_NANO_TO_MILLI 1000000

static void dummy(int x) { }
weak_alias(dummy, __aio_atfork);

void __post_Fork(int ret)
{
	if (!ret) {
		pthread_t self = __pthread_self();
#ifdef __LITEOS_A__
		self->tid = __syscall(SYS_gettid);
		self->pid = __syscall(SYS_getpid);
#else
		self->tid = __syscall(SYS_set_tid_address, &__thread_list_lock);
		self->pid = self->tid;
#endif
		self->proc_tid = -1;
		self->robust_list.off = 0;
		self->robust_list.pending = 0;
		self->next = self->prev = self;
		__thread_list_lock = 0;
		libc.threads_minus_1 = 0;
#ifndef __LITEOS__
		__clear_proc_pid();
#endif
		if (libc.need_locks) libc.need_locks = -1;
#ifdef __LITEOS_A__
		libc.exit = 0;
		signal(SIGSYS, arm_do_signal);
#endif
	}
	UNLOCK(__abort_lock);
	if (!ret) __aio_atfork(1);
}

pid_t _Fork(void)
{
	pid_t ret;
	sigset_t set;

	__block_all_sigs(&set);
	LOCK(__abort_lock);

#ifndef __LITEOS__
	struct timespec time_start, time_end;
	clock_gettime(CLOCK_MONOTONIC, &time_start);
#endif

#ifdef SYS_fork
	ret = __syscall(SYS_fork);
#else
	ret = __syscall(SYS_clone, SIGCHLD, 0);
#endif
	
#ifndef __LITEOS__
	clock_gettime(CLOCK_MONOTONIC, &time_end);
	int cost_time = (time_end.tv_sec - time_start.tv_sec) * CLOCK_SECOND_TO_MILLI
		+ (time_end.tv_nsec - time_start.tv_nsec) / CLOCK_NANO_TO_MILLI;
	if(cost_time > UNUSUAL_FORK_COST_TIME_MILLI)
		MUSL_LOGE("_Fork __syscall costTime : %{public}d ms",cost_time);
#endif

	__post_Fork(ret);
	__restore_sigs(&set);
	return __syscall_ret(ret);
}
