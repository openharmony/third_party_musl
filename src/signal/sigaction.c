#include <signal.h>
#include <errno.h>
#include <string.h>
#include "syscall.h"
#include "pthread_impl.h"
#include "libc.h"
#include "lock.h"
#include "ksigaction.h"

#define NUM_SIGNAL_ACTIONS      16
#define SIGNO2SET(s)            ((sigset_t)1 << (s))
#define NULL_SIGNAL_SET         ((sigset_t)0x00000000)
#define SET_BIT(bitmap, pos)    (bitmap |= (1u << pos))
#define CLEAR_BIT(bitmap, pos)  (bitmap &= ~(1u << pos))
#define CHECK_BIT(bitmap, pos)  ((bitmap & (1u << pos)) ? 1 : 0)
#define SIG_FLAG_NOIGNORE 1

#define QUEUE_INIT(q) \
	do {\
		(q)->head = NULL; \
		(q)->tail = NULL; \
	}\
	while (0)

struct sigactq {
	struct sigactq *flink;
	struct sigaction act;
	unsigned char   signo;
	unsigned char   sigmask;
	unsigned char   reserve[2];
};
typedef struct sigactq sigactq_t;

struct sq_entry_s {
	struct sq_entry_s *flink;
};
typedef struct sq_entry_s sq_entry_t;

struct sigpendq {
	struct sigpendq *flink;
	siginfo_t info;
	unsigned char   type;
};
typedef struct sigpendq sigpendq_t;

struct sq_queue_s {
	sq_entry_t *head;
	sq_entry_t *tail;
};
typedef struct sq_queue_s  sq_queue_t;

typedef void (*sa_sighandler_t)(int);
typedef struct sigaction sigaction_t;

sq_queue_t sig_free_action;
static sigactq_t *sig_action_alloc;
sq_queue_t sig_actionq;
static pthread_spinlock_t sig_lite_lock;

struct sig_default_act {
	unsigned char singNo;
	unsigned char flag;
	sa_sighandler_t action;
};


static void __sig_core(int signo);
static void __sig_kill(int signo);
static void __sig_cont(int signo);
static void __sig_stop(int signo);
static void __sig_ignore(int signo);
static const struct sig_default_act sig_default_action[] = {
	{SIGHUP, 0, __sig_kill},
	{SIGINT, 0, __sig_kill},
	{SIGQUIT, 0, __sig_core},
	{SIGILL, 0, __sig_core},
	{SIGTRAP, 0, __sig_core},
	{SIGABRT, 0, __sig_core},
	{SIGBUS, 0, __sig_core},
	{SIGFPE, 0, __sig_core},
	{SIGKILL, SIG_FLAG_NOIGNORE, __sig_kill},
	{SIGUSR1, 0, __sig_kill},
	{SIGSEGV, 0, __sig_core},
	{SIGUSR2, 0, __sig_kill},
	{SIGPIPE, 0, __sig_kill},
	{SIGALRM, 0, __sig_kill},
	{SIGTERM, 0, __sig_kill},
	{SIGSTKFLT, 0, __sig_kill},
	{SIGCHLD, 0, __sig_ignore},
	{SIGCONT, SIG_FLAG_NOIGNORE, __sig_cont},
	{SIGSTOP, SIG_FLAG_NOIGNORE, __sig_stop},
	{SIGTSTP, 0, __sig_stop},
	{SIGTTIN, 0, __sig_stop},
	{SIGTTOU, 0, __sig_stop},
	{SIGURG, 0, __sig_ignore},
	{SIGXCPU, 0, __sig_core},
	{SIGXFSZ, 0, __sig_core},
	{SIGVTALRM, 0, __sig_kill},
	{SIGPROF, 0, __sig_kill},
	{SIGWINCH, 0, __sig_ignore},
	{SIGIO, 0, __sig_kill},
	{SIGPWR, 0, __sig_kill},
	{SIGSYS, 0, __sig_ignore},
	{32, 0, __sig_ignore},
	{33, 0, __sig_ignore},
	{34, 0, __sig_ignore},
	{35, 0, __sig_ignore},
	{36, 0, __sig_ignore},
	{37, 0, __sig_ignore},
	{38, 0, __sig_ignore},
	{39, 0, __sig_ignore},
	{40, 0, __sig_ignore},
	{41, 0, __sig_ignore},
	{42, 0, __sig_ignore},
	{43, 0, __sig_ignore},
	{44, 0, __sig_ignore},
	{45, 0, __sig_ignore},
	{46, 0, __sig_ignore},
	{47, 0, __sig_ignore},
	{48, 0, __sig_ignore},
	{49, 0, __sig_ignore},
	{50, 0, __sig_ignore},
	{51, 0, __sig_ignore},
	{52, 0, __sig_ignore},
	{53, 0, __sig_ignore},
	{54, 0, __sig_ignore},
	{55, 0, __sig_ignore},
	{56, 0, __sig_ignore},
	{57, 0, __sig_ignore},
	{58, 0, __sig_ignore},
	{59, 0, __sig_ignore},
	{60, 0, __sig_ignore},
	{61, 0, __sig_ignore},
	{62, 0, __sig_ignore},
	{63, 0, __sig_ignore},
	{64, 0, __sig_ignore},
};

static void __sig_core(int signo)
{
	exit(-1);
}

static void __sig_kill(int signo)
{
	exit(-1);
}

static void __sig_cont(int signo)
{
	return;
}

static void __sig_stop(int signo)
{
    return;
}

static void __sig_ignore(int signo)
{
    return;
}
static void __sig_sq_add_last(sq_entry_t *node, sq_queue_t *queue)
{
	node->flink = NULL;
	if (!queue->head) {
		queue->head = node;
		queue->tail = node;
	} else {
		queue->tail->flink = node;
		queue->tail = node;
	}
}

static sigactq_t *__sig_find_action(int sig)
{
	sigactq_t *sigact = NULL;

	for (sigact = (sigactq_t *)sig_actionq.head; ((sigact) && (sigact->signo != sig)); sigact = sigact->flink) {
		;
	}
	return sigact;
}

static sq_entry_t *__sig_sq_remafter(sq_entry_t *node, sq_queue_t *queue)
{
	sq_entry_t *ret = node->flink;

	if (queue->head && ret) {
		if (queue->tail == ret) {
			queue->tail = node;
			node->flink = NULL;
		} else {
			node->flink = ret->flink;
		}

		ret->flink = NULL;
	}

	return ret;
}

static void __sig_sq_remove(sq_entry_t *node, sq_queue_t *queue)
{
	if (queue->head && node) {
		if (node == queue->head) {
			queue->head = node->flink;
			if (node == queue->tail) {
				queue->tail = NULL;
			}
		} else {
			sq_entry_t *prev;
			for (prev = (sq_entry_t *)queue->head; prev && prev->flink != node; prev = prev->flink) {
				;
			}

			if (prev) {
				__sig_sq_remafter(prev, queue);
			}
		}
	}
}

static void __sig_copy_sigaction(sigaction_t *src, sigaction_t *dst)
{
	dst->sa_handler = src->sa_handler;
	dst->sa_mask = src->sa_mask;
	dst->sa_flags = src->sa_flags;
}

static void __sig_remove_sigaction(sq_entry_t *sigact)
{
	__sig_sq_remove(sigact, &sig_actionq);
	__sig_sq_add_last(sigact, &sig_free_action);
}

static int __sig_cannot_catche(int sig, sa_sighandler_t handler)
{
	int i;

	for (i = 0; i < sizeof(sig_default_action) / sizeof(struct sig_default_act); i++) {
		if (sig == sig_default_action[i].singNo) {
			return (sig_default_action[i].flag == SIG_FLAG_NOIGNORE) && (handler != SIG_DFL);
		}
	}
	/* This sig can be catch and ignore return false */
	return 0;
}


static sq_entry_t *__sig_sq_rem_first(sq_queue_t *queue)
{
	sq_entry_t *ret = queue->head;

	if (ret) {
		queue->head = ret->flink;
		if (!queue->head) {
			queue->tail = NULL;
		}

		ret->flink = NULL;
	}

	return ret;
}

static void __sig_operation(unsigned int receivedSigno)
{
	sigactq_t *sigact = NULL;

	for (sigact = (sigactq_t *)sig_actionq.head; (sigact); sigact = sigact->flink) {
		if (sigact->signo == receivedSigno && sigact->act.sa_handler) {
			(*sigact->act.sa_handler)(sigact->signo);
		}
	}
}

void arm_signal_process(unsigned int receivedSig)
{
	__sig_operation(receivedSig);
}

static void __sig_alloc_action_block()
{
	sigactq_t *sigact;
	int i;

	/* Allocate a block of signal actions */
	sig_action_alloc = (sigactq_t *)calloc(NUM_SIGNAL_ACTIONS, (sizeof(sigactq_t)));
	if (sig_action_alloc != NULL) {
		sigact = sig_action_alloc;
		for (i = 0; i < NUM_SIGNAL_ACTIONS; i++) {
			__sig_sq_add_last((sq_entry_t *)sigact++, &sig_free_action);
		}
	}
}

static sigactq_t *__sig_alloc_action()
{
	sigactq_t *sigact;

	/* Try to get the signal action structure from the free list */
	sigact = (sigactq_t *)__sig_sq_rem_first(&sig_free_action);
	/* Check if we got one. */
	if (!sigact) {
		/* Add another block of signal actions to the list */
		__sig_alloc_action_block();

		/* And try again */
		sigact = (sigactq_t *)__sig_sq_rem_first(&sig_free_action);
	}
	return sigact;
}

static void __sig_add_def_action()
{
	int i;
	sigactq_t *sigact = NULL;

	for (i = 0; i < sizeof(sig_default_action) / sizeof(struct sig_default_act); i++) {
		sigact = __sig_alloc_action();
		if (!sigact) {
			return;
		}
		sigact->signo = (unsigned char)sig_default_action[i].singNo;
		__sig_sq_add_last((sq_entry_t *)sigact, &sig_actionq);
		sigact->act.sa_handler = sig_default_action[i].action;
		sigemptyset(&sigact->act.sa_mask);
		sigact->act.sa_flags = sig_default_action[i].flag;
	}
}

static sa_sighandler_t __sig_find_def_action(unsigned char signo)
{
	int i;

	for (i = 0; i < sizeof(sig_default_action) / sizeof(struct sig_default_act); i++) {
		if (signo == sig_default_action[i].singNo) {
			return sig_default_action[i].action;
		}
	}
	return NULL;
}

static int __sig_dfl_opr(int sig, sigactq_t *sigact, const sigaction_t *act)
{
	sa_sighandler_t def_handler = NULL;

	def_handler = __sig_find_def_action(sig);
	if (sigact) {
		if (def_handler == NULL) {
			/* Remove it from signal action queue */
			__sig_remove_sigaction((sq_entry_t *)sigact);
		} else {
			/* Replace it from signal action queue */
			sigact->act.sa_handler = def_handler;
			sigact->act.sa_mask = act->sa_mask;
			sigact->act.sa_flags = act->sa_flags;
		}
	} else {
		if (def_handler) {
			sigact = __sig_alloc_action();
			/* An error has occurred if we could not allocate the sigaction */
			if (!sigact) {
				return -ENOMEM;
			}
			sigact->signo = (unsigned char)sig;
			sigact->act.sa_handler = def_handler;
			sigact->act.sa_mask = act->sa_mask;
			sigact->act.sa_flags = act->sa_flags;
			__sig_sq_add_last((sq_entry_t *)sigact, &sig_actionq);
		}
	}
	return 0;
}

static int __sig_action_opr(int sig, const sigaction_t *act, sigaction_t *oact)
{
	int ret = 0;
	sa_sighandler_t handler = NULL;
	sigactq_t *sigact = NULL;

	if (act == NULL) return -EINVAL;
	if (sig < SIGHUP || sig > (_NSIG - 1)) return -EINVAL;

	handler = act->sa_handler;
	/* Skip sig that can not be catched */
	if (__sig_cannot_catche(sig, handler)) return -EINVAL;

	pthread_spin_lock(&sig_lite_lock);
	sigact = __sig_find_action(sig);
	if (sigact && oact) __sig_copy_sigaction(&sigact->act, oact);

	if (handler == SIG_IGN && sigact) {
		__sig_remove_sigaction((sq_entry_t *)sigact);
	} else if (handler == SIG_DFL) {
		ret = __sig_dfl_opr(sig, sigact, act);
	} else {
		if (sigact == NULL) {
			sigact = __sig_alloc_action();
			/* An error has occurred if we could not allocate the sigaction */
			if (!sigact) {
				pthread_spin_unlock(&sig_lite_lock);
				return -ENOMEM;
			}
			sigact->signo = (unsigned char)sig;
			__sig_sq_add_last((sq_entry_t *)sigact, &sig_actionq);
		}
		sigact->act.sa_handler = handler;
		sigact->act.sa_mask = act->sa_mask;
		sigact->act.sa_flags = act->sa_flags;
	}
	pthread_spin_unlock(&sig_lite_lock);
	return ret;
}

void __sig_init(void)
{
	signal(SIGSYS, arm_do_signal);
	QUEUE_INIT(&sig_free_action);
	pthread_spin_init(&sig_lite_lock, 0);
	__sig_alloc_action_block();
	__sig_add_def_action();
}

static volatile int dummy_lock[1] = { 0 };

extern hidden volatile int __abort_lock[1];

weak_alias(dummy_lock, __abort_lock);

static int unmask_done;
static unsigned long handler_set[_NSIG/(8*sizeof(long))];

void __get_handler_set(sigset_t *set)
{
	memcpy(set, handler_set, sizeof handler_set);
}

volatile int __eintr_valid_flag;

int __libc_sigaction(int sig, const struct sigaction *restrict sa, struct sigaction *restrict old)
{
	sigaction_t ksa, ksa_old;
	unsigned long set[_NSIG/(8*sizeof(long))];
	int r = 0;

	if (sa) {
		if ((uintptr_t)sa->sa_handler > 1UL) {
			a_or_l(handler_set+(sig-1)/(8*sizeof(long)),
				1UL<<(sig-1)%(8*sizeof(long)));

			/* If pthread_create has not yet been called,
			 * implementation-internal signals might not
			 * yet have been unblocked. They must be
			 * unblocked before any signal handler is
			 * installed, so that an application cannot
			 * receive an illegal sigset_t (with them
			 * blocked) as part of the ucontext_t passed
			 * to the signal handler. */
			if (!libc.threaded && !unmask_done) {
				__syscall(SYS_rt_sigprocmask, SIG_UNBLOCK,
					SIGPT_SET, 0, _NSIG/8);
				unmask_done = 1;
			}

			if (!(sa->sa_flags & SA_RESTART)) {
				a_store(&__eintr_valid_flag, 1);
			}
		}
		/* Changing the disposition of SIGABRT to anything but
		 * SIG_DFL requires a lock, so that it cannot be changed
		 * while abort is terminating the process after simply
		 * calling raise(SIGABRT) failed to do so. */
		if (sa->sa_handler != SIG_DFL && sig == SIGABRT) {
			__block_all_sigs(&set);
			LOCK(__abort_lock);
		}
		ksa.sa_handler = sa->sa_handler;
		ksa.sa_flags = sa->sa_flags | SA_RESTORER;
		ksa.sa_restorer = (sa->sa_flags & SA_SIGINFO) ? __restore_rt : __restore;
		memcpy(&ksa.sa_mask, &sa->sa_mask, _NSIG/8);
	}

	if (sig == SIGSYS) {
		return __syscall(SYS_rt_sigaction, sig, sa?&ksa:0, old?&ksa_old:0, _NSIG/8);
	} else {
		r = __sig_action_opr(sig, (const sigaction_t*)sa?&ksa:0, (sigaction_t*)old?&ksa_old:0);
	}
	if (sig == SIGABRT && sa && sa->sa_handler != SIG_DFL) {
		UNLOCK(__abort_lock);
		__restore_sigs(&set);
	}
	if (old && !r) {
		old->sa_handler = ksa_old.sa_handler;
		old->sa_flags = ksa_old.sa_flags;
		memcpy(&old->sa_mask, &ksa_old.sa_mask, _NSIG/8);
	}
	return __syscall_ret(r);
}

int __sigaction(int sig, const struct sigaction *restrict sa, struct sigaction *restrict old)
{
	if (sig-32U < 3 || sig-1U >= _NSIG-1) {
		errno = EINVAL;
		return -1;
	}
	return __libc_sigaction(sig, sa, old);
}

weak_alias(__sigaction, sigaction);
