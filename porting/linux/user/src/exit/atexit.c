#include <stdlib.h>
#include <stdint.h>
#include "libc.h"
#include "lock.h"
#include "fork_impl.h"
#include "dynlink.h"
#include "musl_log.h"

#define malloc __libc_malloc
#define calloc __libc_calloc
#define realloc undef
#define free undef

/* Ensure that at least 32 atexit handlers can be registered without malloc */
#define COUNT 32

static struct fl
{
	struct fl *next;
	void (*f[COUNT])(void *);
	void *a[COUNT];
	void *dso[COUNT];
	struct dso *internal_dso[COUNT]; // the internal dso weekptr, used for dlclose
} builtin, *head;

static int slot;
static volatile int lock[1];
volatile int *const __atexit_lockptr = lock;

void __funcs_on_exit()
{
	void (*func)(void *), *arg;
	LOCK(lock);
	for (; head; head=head->next, slot=COUNT) while(slot-->0) {
		if (head->f[slot] != NULL) {
			func = head->f[slot];
			arg = head->a[slot];
			UNLOCK(lock);
			func(arg);
			LOCK(lock);
		}
	}
	UNLOCK(lock);
}

void __cxa_finalize(void *dso)
{
	void (*func)(void *), *arg;
	struct fl *head_tmp = head;
	int slot_tmp = slot;

	LOCK(lock);
	for (; head_tmp; head_tmp=head_tmp->next, slot_tmp=COUNT) while(slot_tmp-->0) {
		if (dso == head_tmp->dso[slot_tmp]) {
			func = head_tmp->f[slot_tmp];
			arg = head_tmp->a[slot_tmp];
			UNLOCK(lock);
			func(arg);
			LOCK(lock);

			head_tmp->dso[slot_tmp] = NULL;
			head_tmp->f[slot_tmp] = NULL;
		}
	}
	UNLOCK(lock);
}

static void call(void *p);

int __cxa_atexit(void (*func)(void *), void *arg, void *dso)
{
	struct dso *p = NULL;
	LOCK(lock);

	/* Defer initialization of head so it can be in BSS */
	if (!head) head = &builtin;

	// if called from atexit, check callback ptr mem range.
#if (defined(FEATURE_ATEXIT_CB_PROTECT))
	if ((func == (void *)call) && (dso == NULL)) {
		p = addr2dso((size_t)arg);
		if (p == NULL) {
			UNLOCK(lock);
			MUSL_LOGE("call atexit with invalid callback ptr=%{public}p", arg);
			return -1;
		}
	}
#endif

	/* If the current function list is full, add a new one */
	if (slot==COUNT) {
		struct fl *new_fl = calloc(sizeof(struct fl), 1);
		if (!new_fl) {
			UNLOCK(lock);
			return -1;
		}
		new_fl->next = head;
		head = new_fl;
		slot = 0;
	}

	/* Append function to the list. */
	head->f[slot] = func;
	head->a[slot] = arg;
	head->dso[slot] = dso;
	head->internal_dso[slot] = p;

	slot++;

	UNLOCK(lock);
	return 0;
}

static void call(void *p)
{
	if (p != NULL)
		((void (*)(void))(uintptr_t)p)();
}

int atexit(void (*func)(void))
{
	return __cxa_atexit(call, (void *)(uintptr_t)func, 0);
}

int invalidate_exit_funcs(struct dso *p)
{
	struct fl *head_tmp = head;
	int slot_tmp = slot;

	LOCK(lock);
	for (; head_tmp; head_tmp=head_tmp->next, slot_tmp=COUNT) {
		while(slot_tmp-->0) {
			// if found exit callback relative to this dso, and
			if (p == head_tmp->internal_dso[slot_tmp]) {
				if ((head_tmp->dso[slot_tmp] == NULL) && head_tmp->f[slot_tmp] == (void *)call) {
					MUSL_LOGD("invalidate callback ptr=%{public}p when uninstall %{public}%s", head_tmp->a[slot_tmp], p->name);
					head_tmp->a[slot_tmp] = NULL;
				}
			}
		}
	}
	UNLOCK(lock);

	return 0;
}