#include <malloc.h>
#include "malloc_impl.h"
#include "malloc_config.h"
#ifdef MALLOC_RED_ZONE
#include "atomic.h"
#endif

#ifdef USE_JEMALLOC
extern size_t je_malloc_usable_size(void *p);
#endif

hidden void *(*const __realloc_dep)(void *, size_t) = realloc;

#ifdef HOOK_ENABLE
size_t __libc_malloc_usable_size(void* p)
#else
size_t malloc_usable_size(void *p)
#endif
{
	return internal_malloc_usable_size(p);
}

size_t internal_malloc_usable_size(void* p)
{
#ifdef USE_JEMALLOC
	return je_malloc_usable_size(p);
#endif

#ifndef MALLOC_RED_ZONE
	return p ? CHUNK_SIZE(MEM_TO_CHUNK(p)) - OVERHEAD : 0;
#else
	struct chunk *c;

	if (!p) {
		return 0;
	}

	c = MEM_TO_CHUNK(p);
	if (chunk_checksum_check(c)) {
		a_crash();
	}
	if (!(c->state & M_STATE_USED)) {
		return 0;
	}
	return c->usize;
#endif
}
