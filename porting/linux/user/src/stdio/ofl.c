#include "stdio_impl.h"
#include "lock.h"
#include "fork_impl.h"
#include <stdlib.h>

#define DEFAULT_ALLOC_FILE (8)

static FILE *ofl_head = NULL;
static FILE *ofl_free = NULL;

static volatile int ofl_lock[1];
volatile int *const __stdio_ofl_lockptr = ofl_lock;

FILE **__ofl_lock()
{
	LOCK(ofl_lock);
	return &ofl_head;
}

void __ofl_unlock()
{
	UNLOCK(ofl_lock);
}

FILE *__ofl_alloc()
{
	unsigned char *fsb = NULL;
	size_t cnt = 0;
	FILE *f = NULL;

	LOCK(ofl_lock);
	if (ofl_free) {
		f = ofl_free;
		ofl_free = ofl_free->next;
		UNLOCK(ofl_lock);

		return f;
	}
	UNLOCK(ofl_lock);

	/* alloc new FILEs(8) */
	fsb = (unsigned char *)malloc(DEFAULT_ALLOC_FILE * sizeof(FILE));
	if (!fsb) {
		return NULL;
	}

	LOCK(ofl_lock);
	ofl_free = (FILE*)fsb;
	ofl_free->prev = NULL;
	f = ofl_free;

	for (cnt = 1; cnt < DEFAULT_ALLOC_FILE; cnt++) {
		FILE *tmp = (FILE*)(fsb + cnt * sizeof(FILE));
		tmp->next = NULL;
		f->next = tmp;
		tmp->prev = f;
		f = f->next;
	}

	/* reset and move to next free FILE */
	f = ofl_free;
	ofl_free = ofl_free->next;
	if (ofl_free) {
		ofl_free->prev = NULL;
	}

	UNLOCK(ofl_lock);
	return f;
}

void __ofl_free(FILE *f)
{
	LOCK(ofl_lock);
	if (!f) {
		return;
	}

	/* remove from head list */
	if (f->prev) {
		f->prev->next = f->next;
	}
	if (f->next) {
		f->next->prev = f->prev;
	}
	if (f == ofl_head) {
		ofl_head = f->next;
	}

	/* push to free list */
	f->next = ofl_free;
	if (ofl_free) {
		ofl_free->prev = f;
	}
	ofl_free = f;

	UNLOCK(ofl_lock);
}
