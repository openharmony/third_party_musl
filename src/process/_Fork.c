#include <unistd.h>
#include <signal.h>
#include "syscall.h"
#include "libc.h"
#include "lock.h"
#include "pthread_impl.h"
#include "aio_impl.h"
#ifndef __LITEOS__
#include "proc_xid_impl.h"
#endif

static void dummy(int x) { }
weak_alias(dummy, __aio_atfork);

pid_t _Fork(void)
{
	pid_t ret;
	sigset_t set;
	__block_all_sigs(&set);
	__aio_atfork(-1);
	LOCK(__abort_lock);

#ifndef __LITEOS__
	MUSL_LOGI("_Fork __syscall Begin");
#endif

#ifdef SYS_fork
	ret = __syscall(SYS_fork);
#else
	ret = __syscall(SYS_clone, SIGCHLD, 0);
#endif

#ifndef __LITEOS__
	MUSL_LOGI("_Fork __syscall End");
#endif

	if (!ret) {
		pthread_t self = __pthread_self();
		self->tid = __syscall(SYS_gettid);
#ifdef __LITEOS_A__
		self->pid = __syscall(SYS_getpid);
#else
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
	__aio_atfork(!ret);
	__restore_sigs(&set);
	return __syscall_ret(ret);
}
