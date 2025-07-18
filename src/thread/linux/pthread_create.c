#define _GNU_SOURCE
#define ANON_STACK_NAME_SIZE 50
#include "musl_log.h"
#include "pthread_impl.h"
#include "stdio_impl.h"
#include "libc.h"
#include "lock.h"
#include <sys/mman.h>
#include <sys/prctl.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

pid_t getpid(void);

void log_print(const char* info, ...)
{
    va_list ap;
    va_start(ap, info);
    vfprintf(stdout, info, ap);
    va_end(ap);
}

void stack_naming(struct pthread *new) {
	size_t size_len;
	unsigned char *start_addr;
	char name[ANON_STACK_NAME_SIZE];
	if (new->guard_size) {
		snprintf(name, ANON_STACK_NAME_SIZE, "guard:%d", new->tid);
		prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, new->map_base, new->guard_size, name);
		start_addr = new->map_base + new->guard_size;
		size_len = new->map_size - new->guard_size;
		memset(name, 0, ANON_STACK_NAME_SIZE);
	} else {
		start_addr = new->map_base;
		size_len = new->map_size;
	}
	snprintf(name, ANON_STACK_NAME_SIZE, "stack:%d", new->tid);
	prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, start_addr, size_len, name);
};

#ifdef RESERVE_SIGNAL_STACK
#if defined (__LP64__)
#define RESERVE_SIGNAL_STACK_SIZE (32 * 1024)
#else
#define RESERVE_SIGNAL_STACK_SIZE (20 * 1024)
#endif
void __pthread_reserve_signal_stack()
{
	void* stack = mmap(NULL, RESERVE_SIGNAL_STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (stack != MAP_FAILED) {
		if (mprotect(stack, __default_guardsize, PROT_NONE) == -1) {
			munmap(stack, RESERVE_SIGNAL_STACK_SIZE);
			return;
		}
	}

	stack_t signal_stack;
	signal_stack.ss_sp = (uint8_t*)stack + __default_guardsize;
	signal_stack.ss_size = RESERVE_SIGNAL_STACK_SIZE - __default_guardsize;
	signal_stack.ss_flags = 0;
	sigaltstack(&signal_stack, NULL);

	pthread_t self = __pthread_self();
	self->signal_stack = stack;
	char name[ANON_STACK_NAME_SIZE];
	snprintf(name, ANON_STACK_NAME_SIZE, "signal_stack:%d", __pthread_self()->tid);
	prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, signal_stack.ss_sp, signal_stack.ss_size, name);
	return;
}

#ifdef ENABLE_HWASAN
__attribute__((no_sanitize("hwaddress")))
#endif
void __pthread_release_signal_stack()
{
	pthread_t self = __pthread_self();
	if (self->signal_stack == NULL) {
		return;
	}

	stack_t signal_stack, old_stack;
	memset(&signal_stack, 0, sizeof(signal_stack));
	signal_stack.ss_flags = SS_DISABLE;
	sigaltstack(&signal_stack, &old_stack);
	munmap(self->signal_stack, __default_guardsize);
	if (old_stack.ss_flags != SS_DISABLE) {
		munmap(old_stack.ss_sp, old_stack.ss_size);
	}
	self->signal_stack = NULL;
}

weak_alias(__pthread_reserve_signal_stack, pthread_reserve_signal_stack);
weak_alias(__pthread_release_signal_stack, pthread_release_signal_stack);
#endif

static void dummy_0()
{
}
weak_alias(dummy_0, __acquire_ptc);
weak_alias(dummy_0, __release_ptc);
weak_alias(dummy_0, __pthread_tsd_run_dtors);
weak_alias(dummy_0, __do_orphaned_stdio_locks);
weak_alias(dummy_0, __dl_thread_cleanup);
weak_alias(dummy_0, __membarrier_init);

#define TID_ERROR_0 (0)
#define TID_ERROR_INIT (-1)
#define COUNT_ERROR_INIT (-10000)

static int tl_lock_count;
static int tl_lock_waiters;
static int tl_lock_tid_fail = TID_ERROR_INIT;
static int tl_lock_count_tid = TID_ERROR_INIT;
static int tl_lock_count_tid_sub = TID_ERROR_INIT;
static int tl_lock_count_fail = COUNT_ERROR_INIT;
static int thread_list_lock_after_lock = TID_ERROR_INIT;
static int thread_list_lock_pre_unlock = TID_ERROR_INIT;
static int thread_list_lock_pthread_exit = TID_ERROR_INIT;
static int thread_list_lock_tid_overlimit = TID_ERROR_INIT;
static int register_count = 0;

struct call_tl_lock tl_lock_caller_count = { 0 };

int get_tl_lock_count(void)
{
	return tl_lock_count;
}

int get_tl_lock_waiters(void)
{
	return tl_lock_waiters;
}

int get_tl_lock_tid_fail(void)
{
	return tl_lock_tid_fail;
}

int get_tl_lock_count_tid(void)
{
	return tl_lock_count_tid;
}

int get_tl_lock_count_tid_sub(void)
{
	return tl_lock_count_tid_sub;
}

int get_tl_lock_count_fail(void)
{
	return tl_lock_count_fail;
}

int get_thread_list_lock_after_lock(void)
{
	return thread_list_lock_after_lock;
}

int get_thread_list_lock_pre_unlock(void)
{
	return thread_list_lock_pre_unlock;
}

int get_thread_list_lock_pthread_exit(void)
{
	return thread_list_lock_pthread_exit;
}

int get_thread_list_lock_tid_overlimit(void)
{
	return thread_list_lock_tid_overlimit;
}

struct call_tl_lock *get_tl_lock_caller_count(void)
{
	return &tl_lock_caller_count;
}

int get_register_count()
{
	return register_count;
}

void update_register_count()
{
	register_count++;
}

#ifdef ENABLE_HWASAN
__attribute__((no_sanitize("hwaddress")))
#endif
void __tl_lock(void)
{
	int tid = __pthread_self()->tid;
	if (tid == TID_ERROR_0 || tid == TID_ERROR_INIT) {
		tl_lock_tid_fail = TID_ERROR_0;
		tid = __syscall(SYS_gettid);
	}
	if ((thread_list_lock_pthread_exit == tid) &&
	    (thread_list_lock_pthread_exit == __thread_list_lock)) {
			thread_list_lock_tid_overlimit = __thread_list_lock;
	}
	int val = __thread_list_lock;
	if (val == tid) {
		tl_lock_count++;
		tl_lock_count_tid = val;
		return;
	}
	while ((val = a_cas(&__thread_list_lock, 0, tid)))
		__wait(&__thread_list_lock, &tl_lock_waiters, val, 0);
	thread_list_lock_after_lock = __thread_list_lock;
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->tl_lock_unlock_count++;
	}
}

void __tl_unlock(void)
{
	if (tl_lock_count) {
		tl_lock_count--;
		tl_lock_count_tid_sub = __thread_list_lock;
		return;
	}
	thread_list_lock_pre_unlock = __thread_list_lock;
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->tl_lock_unlock_count--;
	}
	a_store(&__thread_list_lock, 0);
	if (tl_lock_waiters) __wake(&__thread_list_lock, 1, 0);
}

void __tl_sync(pthread_t td)
{
	a_barrier();
	int val = __thread_list_lock;
	if (!val) return;
	__wait(&__thread_list_lock, &tl_lock_waiters, val, 0);
	if (tl_lock_waiters) __wake(&__thread_list_lock, 1, 0);
}

#ifdef CXA_THREAD_USE_TLS
extern void __cxa_thread_finalize();
#endif

#ifdef ENABLE_HWASAN
weak void __hwasan_thread_enter();
weak void __hwasan_thread_exit();

__attribute__((no_sanitize("hwaddress")))
#endif
_Noreturn void __pthread_exit(void *result)
{
#ifdef CXA_THREAD_USE_TLS
	// Call thread_local dtors.
	__cxa_thread_finalize();
#endif
	pthread_t self = __pthread_self();
	sigset_t set;

#ifdef FEATURE_PTHREAD_CANCEL
	self->canceldisable = 1;
	self->cancelasync = 0;
#endif
	self->result = result;

	while (self->cancelbuf) {
		void (*f)(void *) = self->cancelbuf->__f;
		void *x = self->cancelbuf->__x;
		self->cancelbuf = self->cancelbuf->__next;
		f(x);
	}

	__pthread_tsd_run_dtors();

	__block_app_sigs(&set);

	/* This atomic potentially competes with a concurrent pthread_detach
	 * call; the loser is responsible for freeing thread resources. */
	int state = a_cas(&self->detach_state, DT_JOINABLE, DT_EXITING);

	if (state==DT_DETACHED && self->map_base) {
		/* Since __unmapself bypasses the normal munmap code path,
		 * explicitly wait for vmlock holders first. This must be
		 * done before any locks are taken, to avoid lock ordering
		 * issues that could lead to deadlock. */
		__vm_wait();
	}

	/* Access to target the exiting thread with syscalls that use
	 * its kernel tid is controlled by killlock. For detached threads,
	 * any use past this point would have undefined behavior, but for
	 * joinable threads it's a valid usage that must be handled.
	 * Signals must be blocked since pthread_kill must be AS-safe. */
	LOCK(self->killlock);

	/* The thread list lock must be AS-safe, and thus depends on
	 * application signals being blocked above. */
	__tl_lock();
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->__pthread_exit_tl_lock++;
	}

#ifdef RESERVE_SIGNAL_STACK
	__pthread_release_signal_stack();
#endif
	/* If this is the only thread in the list, don't proceed with
	 * termination of the thread, but restore the previous lock and
	 * signal state to prepare for exit to call atexit handlers. */
	if (self->next == self) {
		if (get_tl_lock_caller_count()) {
			get_tl_lock_caller_count()->__pthread_exit_tl_lock--;
		}
		__tl_unlock();
		UNLOCK(self->killlock);
		self->detach_state = state;
		__restore_sigs(&set);
#ifdef ENABLE_HWASAN
		__hwasan_thread_exit();
#endif
		exit(0);
	}

	/* At this point we are committed to thread termination. */

	/* After the kernel thread exits, its tid may be reused. Clear it
	 * to prevent inadvertent use and inform functions that would use
	 * it that it's no longer available. At this point the killlock
	 * may be released, since functions that use it will consistently
	 * see the thread as having exited. Release it now so that no
	 * remaining locks (except thread list) are held if we end up
	 * resetting need_locks below. */
	self->tid = 0;
	UNLOCK(self->killlock);

	/* Process robust list in userspace to handle non-pshared mutexes
	 * and the detached thread case where the robust list head will
	 * be invalid when the kernel would process it. */
	__vm_lock();
	volatile void *volatile *rp;
	while ((rp=self->robust_list.head) && rp != &self->robust_list.head) {
		pthread_mutex_t *m = (void *)((char *)rp
			- offsetof(pthread_mutex_t, _m_next));
		int waiters = m->_m_waiters;
		int priv = (m->_m_type & 128) ^ 128;
		self->robust_list.pending = rp;
		self->robust_list.head = *rp;
		int cont = a_swap(&m->_m_lock, 0x40000000);
		self->robust_list.pending = 0;
		if (cont < 0 || waiters)
			__wake(&m->_m_lock, 1, priv);
	}
	__vm_unlock();

	__do_orphaned_stdio_locks();
	__dl_thread_cleanup();

	/* Last, unlink thread from the list. This change will not be visible
	 * until the lock is released, which only happens after SYS_exit
	 * has been called, via the exit futex address pointing at the lock.
	 * This needs to happen after any possible calls to LOCK() that might
	 * skip locking if process appears single-threaded. */
	if (!--libc.threads_minus_1) libc.need_locks = -1;
	self->next->prev = self->prev;
	self->prev->next = self->next;
	self->prev = self->next = self;

	if (state==DT_DETACHED && self->map_base) {
		/* Detached threads must block even implementation-internal
		 * signals, since they will not have a stack in their last
		 * moments of existence. */
		__block_all_sigs(&set);

		/* Robust list will no longer be valid, and was already
		 * processed above, so unregister it with the kernel. */
		if (self->robust_list.off)
			__syscall(SYS_set_robust_list, 0, 3*sizeof(long));

		/* The following call unmaps the thread's stack mapping
		 * and then exits without touching the stack. */
		if(tl_lock_count != 0) {
			tl_lock_count_fail = tl_lock_count;
			tl_lock_count = 0;
		}
		thread_list_lock_pthread_exit = __thread_list_lock;
		if (get_tl_lock_caller_count()) {
			get_tl_lock_caller_count()->__pthread_exit_tl_lock--;
			get_tl_lock_caller_count()->tl_lock_unlock_count--;
		}
		__unmapself(self->map_base, self->map_size);
	}

	/* Wake any joiner. */
	a_store(&self->detach_state, DT_EXITED);
	__wake(&self->detach_state, 1, 1);

#ifdef ENABLE_HWASAN
	__hwasan_thread_exit();
#endif

	// If a thread call __tl_lock and call __pthread_exit without
	// call __tl_unlock, the value of tl_lock_count will appear
	// non-zero value, here set it to zero.
	if(tl_lock_count != 0) {
		tl_lock_count_fail = tl_lock_count;
		tl_lock_count = 0;
	}
	thread_list_lock_pthread_exit = __thread_list_lock;
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->__pthread_exit_tl_lock--;
		get_tl_lock_caller_count()->tl_lock_unlock_count--;
	}

	for (;;) __syscall(SYS_exit, 0);
}

void __do_cleanup_push(struct __ptcb *cb)
{
	struct pthread *self = __pthread_self();
	cb->__next = self->cancelbuf;
	self->cancelbuf = cb;
}

void __do_cleanup_pop(struct __ptcb *cb)
{
	__pthread_self()->cancelbuf = cb->__next;
}

struct start_args {
	void *(*start_func)(void *);
	void *start_arg;
	volatile int control;
	unsigned long sig_mask[_NSIG/8/sizeof(long)];
};

#ifdef ENABLE_HWASAN
__attribute__((no_sanitize("hwaddress")))
#endif
static int start(void *p)
{
#ifdef ENABLE_HWASAN
	__hwasan_thread_enter();
#endif
	struct start_args *args = p;
	int state = args->control;
	if (state) {
		if (a_cas(&args->control, 1, 2) == 1)
			__wait(&args->control, 0, 2, 1);
		if (args->control) {
			__syscall(SYS_set_tid_address, &args->control);
			for (;;) __syscall(SYS_exit, 0);
		}
	}
	__syscall(SYS_rt_sigprocmask, SIG_SETMASK, &args->sig_mask, 0, _NSIG/8);
#ifdef RESERVE_SIGNAL_STACK
	__pthread_reserve_signal_stack();
#endif
	__pthread_exit(args->start_func(args->start_arg));
	return 0;
}

#ifdef ENABLE_HWASAN
__attribute__((no_sanitize("hwaddress")))
#endif
static int start_c11(void *p)
{
#ifdef RESERVE_SIGNAL_STACK
	__pthread_reserve_signal_stack();
#endif
#ifdef ENABLE_HWASAN
	__hwasan_thread_enter();
#endif
	struct start_args *args = p;
	int (*start)(void*) = (int(*)(void*)) args->start_func;
	__pthread_exit((void *)(uintptr_t)start(args->start_arg));
	return 0;
}

#define ROUND(x) (((x)+PAGE_SIZE-1)&-PAGE_SIZE)

/* pthread_key_create.c overrides this */
static volatile size_t dummy = 0;
weak_alias(dummy, __pthread_tsd_size);
static void *dummy_tsd[1] = { 0 };
weak_alias(dummy_tsd, __pthread_tsd_main);

static FILE *volatile dummy_file = 0;
weak_alias(dummy_file, __stdin_used);
weak_alias(dummy_file, __stdout_used);
weak_alias(dummy_file, __stderr_used);

static void init_file_lock(FILE *f)
{
	if (f && f->lock<0) f->lock = 0;
}

#ifdef ENABLE_HWASAN
__attribute__((no_sanitize("hwaddress")))
#endif
int __pthread_create(pthread_t *restrict res, const pthread_attr_t *restrict attrp, void *(*entry)(void *), void *restrict arg)
{
	int ret, c11 = (attrp == __ATTRP_C11_THREAD);
	size_t size, guard;
	struct pthread *self, *new;
	unsigned char *map = 0, *stack = 0, *tsd = 0, *stack_limit;
	unsigned flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
		| CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS
		| CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_DETACHED;
	pthread_attr_t attr = { 0 };
	sigset_t set;

	if (!libc.can_do_threads) {
		MUSL_LOGE("pthread_create: can't do threads, err: %{public}s", strerror(errno));
		return ENOSYS;
	}
	self = __pthread_self();
	if (!libc.threaded) {
		for (FILE *f = *__ofl_lock(); f; f = f->next)
			init_file_lock(f);
		__ofl_unlock();
		init_file_lock(__stdin_used);
		init_file_lock(__stdout_used);
		init_file_lock(__stderr_used);
		__syscall(SYS_rt_sigprocmask, SIG_UNBLOCK, SIGPT_SET, 0, _NSIG/8);
		self->tsd = (void **)__pthread_tsd_main;
		__membarrier_init();
		libc.threaded = 1;
	}
	if (attrp && !c11) attr = *attrp;

	__acquire_ptc();
	if (!attrp || c11) {
		attr._a_stacksize = __default_stacksize;
		attr._a_guardsize = __default_guardsize;
	}

	if (attr._a_stackaddr) {
		size_t need = libc.tls_size + __pthread_tsd_size;
		size = attr._a_stacksize;
		stack = (void *)(attr._a_stackaddr & -16);
		stack_limit = (void *)(attr._a_stackaddr - size);
		/* Use application-provided stack for TLS only when
		 * it does not take more than ~12% or 2k of the
		 * application's stack space. */
		if (need < size / 8 && need < 2048) {
			tsd = stack - __pthread_tsd_size;
			stack = tsd - libc.tls_size;
			memset(stack, 0, need);
		} else {
			size = ROUND(need);
		}
		guard = 0;
	} else {
		guard = ROUND(attr._a_guardsize);
		size = guard + ROUND(attr._a_stacksize
			+ libc.tls_size +  __pthread_tsd_size);
	}

	if (!tsd) {
		if (guard) {
			map = __mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
			if (map == MAP_FAILED) {
				MUSL_LOGE("pthread_create: mmap PROT_NONE failed, err:%{public}s", strerror(errno));
				goto fail;
			}
			if (__mprotect(map+guard, size-guard, PROT_READ|PROT_WRITE)
			    && errno != ENOSYS) {
				MUSL_LOGE("pthread_create: mprotect failed, err:%{public}s", strerror(errno));
				__munmap(map, size);
				goto fail;
			}
		} else {
			map = __mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
			if (map == MAP_FAILED) {
				MUSL_LOGE("pthread_create: mmap PROT_READ|PROT_WRITE failed, err:%{public}s", strerror(errno));
				goto fail;
			}
		}
		tsd = map + size - __pthread_tsd_size;
		if (!stack) {
			stack = tsd - libc.tls_size;
			stack_limit = map + guard;
		}
	}

	new = __copy_tls(tsd - libc.tls_size);
	new->map_base = map;
	new->map_size = size;
	new->stack = stack;
	new->stack_size = stack - stack_limit;
	new->guard_size = guard;
	new->self = new;
	new->pid = getpid();
	new->proc_tid = -1;
	new->tsd = (void *)tsd;
	new->locale = &libc.global_locale;
	if (attr._a_detach) {
		new->detach_state = DT_DETACHED;
	} else {
		new->detach_state = DT_JOINABLE;
	}
	new->robust_list.head = &new->robust_list.head;
	new->canary = self->canary;
	new->sysinfo = self->sysinfo;
	/* This is for GWP_ASAN to copy the configure to child thread. */
	new->gwp_asan_tls = self->gwp_asan_tls;

	/* Setup argument structure for the new thread on its stack.
	 * It's safe to access from the caller only until the thread
	 * list is unlocked. */
	stack -= (uintptr_t)stack % sizeof(uintptr_t);
	stack -= sizeof(struct start_args);
	struct start_args *args = (void *)stack;
	args->start_func = entry;
	args->start_arg = arg;
	args->control = attr._a_sched ? 1 : 0;

	/* Application signals (but not the synccall signal) must be
	 * blocked before the thread list lock can be taken, to ensure
	 * that the lock is AS-safe. */
	__block_app_sigs(&set);

	/* Ensure SIGCANCEL is unblocked in new thread. This requires
	 * working with a copy of the set so we can restore the
	 * original mask in the calling thread. */
	memcpy(&args->sig_mask, &set, sizeof args->sig_mask);
	args->sig_mask[(SIGCANCEL-1)/8/sizeof(long)] &=
		~(1UL<<((SIGCANCEL-1)%(8*sizeof(long))));

	__tl_lock();
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->__pthread_create_tl_lock++;
	}
	if (!libc.threads_minus_1++) libc.need_locks = 1;
	ret = __clone((c11 ? start_c11 : start), stack, flags, args, &new->tid, TP_ADJ(new), &__thread_list_lock);
	int cloneErrno = 0;
	/* All clone failures translate to EAGAIN. If explicit scheduling
	 * was requested, attempt it before unlocking the thread list so
	 * that the failed thread is never exposed and so that we can
	 * clean up all transient resource usage before returning. */
	if (ret < 0) {
		cloneErrno = ret;
		ret = -EAGAIN;
	} else if (attr._a_sched) {
		ret = __syscall(SYS_sched_setscheduler,
			new->tid, attr._a_policy, &attr._a_prio);
		if (a_swap(&args->control, ret ? 3 : 0) == 2)
			__wake(&args->control, 1, 1);
		if (ret)
			__wait(&args->control, 0, 3, 0);
	}

	if (ret >= 0) {
		stack_naming(new);

		new->next = self->next;
		new->prev = self;
		new->next->prev = new;
		new->prev->next = new;
	} else {
		if (!--libc.threads_minus_1) libc.need_locks = 0;
	}
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->__pthread_create_tl_lock--;
	}
	__tl_unlock();
	__restore_sigs(&set);
	__release_ptc();

	if (ret < 0) {
		if (map) __munmap(map, size);
		MUSL_LOGE("pthread_create: ret: %{public}d, cloneErrno: %{public}d, err: %{public}s",
			ret, cloneErrno, strerror(errno));
		return -ret;
	}

	*res = new;
	return 0;
fail:
	__release_ptc();
	return EAGAIN;
}

weak_alias(__pthread_exit, pthread_exit);
weak_alias(__pthread_create, pthread_create);

struct pthread* __pthread_list_find(pthread_t thread_id, const char* info)
{
    struct pthread *thread = (struct pthread *)thread_id;
    if (NULL == thread) {
        log_print("invalid pthread_t (0) passed to %s\n", info);
        return NULL;
    }

    struct pthread *self = __pthread_self();
    if (thread == self) {
        return thread;
    }
    struct pthread *t = self;
    t = t->next ;
    while (t != self) {
        if (t == thread) return thread;
        t = t->next ;
    }
    log_print("invalid pthread_t %p passed to %s\n", thread, info);
    return NULL;
}

pid_t __pthread_gettid_np(pthread_t t)
{
	sigset_t set;
	__block_app_sigs(&set);
    __tl_lock();
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->__pthread_gettid_np_tl_lock++;
	}
    struct pthread* thread = __pthread_list_find(t, "pthread_gettid_np");
	if (get_tl_lock_caller_count()) {
		get_tl_lock_caller_count()->__pthread_gettid_np_tl_lock--;
	}
    __tl_unlock();
	__restore_sigs(&set);
    return thread ? thread->tid : -1;
}
weak_alias(__pthread_gettid_np, pthread_gettid_np);
