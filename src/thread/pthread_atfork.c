#include <pthread.h>
#include <errno.h>
#include "libc.h"
#include "lock.h"

#define malloc __libc_malloc
#define calloc undef
#define realloc undef
#define free undef

static struct atfork_funcs {
	void (*prepare)(void);
	void (*parent)(void);
	void (*child)(void);
	struct atfork_funcs *prev, *next;
} *funcs, *gwpfuncs;

static volatile int lock[1];

void __fork_handler(int who)
{
	struct atfork_funcs *p;
	if (!funcs) return;
	if (who < 0) {
		LOCK(lock);
		for (p=funcs; p; p = p->next) {
			if (p->prepare) p->prepare();
			funcs = p;
		}
        // The gwpasan prepare must be executed last,and the gwpasan handler must be executed first.
		if (gwpfuncs && gwpfuncs->prepare) gwpfuncs->prepare();
	} else {
        // The gwpasan child will unlock malloc mutex, so execute it first to avoid deadlocks.
		if (gwpfuncs && !who && gwpfuncs->parent) gwpfuncs->parent();
		if (gwpfuncs && who && gwpfuncs->child) gwpfuncs->child();
		for (p=funcs; p; p = p->prev) {
			if (!who && p->parent) p->parent();
			else if (who && p->child) p->child();
			funcs = p;
		}
		UNLOCK(lock);
	}
}

int pthread_atfork_for_gwpasan(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
	struct atfork_funcs *new = malloc(sizeof *new);
	if (!new) return ENOMEM;

	LOCK(lock);
	new->prepare = prepare;
	new->parent = parent;
	new->child = child;
	gwpfuncs = new;
	UNLOCK(lock);
	return 0;
}

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
	struct atfork_funcs *new = malloc(sizeof *new);
	if (!new) return ENOMEM;

	LOCK(lock);
	new->next = funcs;
	new->prev = 0;
	new->prepare = prepare;
	new->parent = parent;
	new->child = child;
	if (funcs) funcs->prev = new;
	funcs = new;
	UNLOCK(lock);
	return 0;
}
