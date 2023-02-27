#include <stdlib.h>

#ifndef USE_JEMALLOC
#include <stdint.h>
#include <errno.h>
#include "malloc_impl.h"
#include "malloc_config.h"
#else
extern void* je_memalign(size_t align, size_t len);
#endif

void *__memalign(size_t align, size_t len)
{
#ifdef USE_JEMALLOC
	return je_memalign(align, len);
#else
	unsigned char *mem, *new;

	if ((align & -align) != align) {
		errno = EINVAL;
		return 0;
	}

	if (len > SIZE_MAX - align || __malloc_replaced) {
		errno = ENOMEM;
		return 0;
	}

	if (align <= SIZE_ALIGN)
		return malloc(len);

	if (!(mem = malloc(len + align-1)))
		return 0;

	new = (void *)((uintptr_t)mem + align-1 & -align);

	if (new == mem) return mem;
	struct chunk *c = MEM_TO_CHUNK(mem);
	struct chunk *n = MEM_TO_CHUNK(new);

#ifdef MUSL_ITERATE_AND_STATS_API
	__pop_chunk(c);
	__push_chunk(n);
#endif

	if (IS_MMAPPED(c)) {
		/* Apply difference between aligned and original
		 * address to the "extra" field of mmapped chunk.
		 */
		n->psize = c->psize + (new-mem);
		n->csize = c->csize - (new-mem);
#ifdef MALLOC_RED_ZONE
		n->usize = len;
		n->state = M_STATE_MMAP | M_STATE_USED;
		chunk_checksum_set(n);
#endif
		return new;
	}

	struct chunk *t = NEXT_CHUNK(c);

	/* Split the allocated chunk into two chunks. The aligned part
	 * that will be used has the size in its footer reduced by the
	 * difference between the aligned and original addresses, and
	 * the resulting size copied to its header. A new header and
	 * footer are written for the split-off part to be freed. */
	n->psize = c->csize = C_INUSE | (new-mem);
	n->csize = t->psize -= new-mem;

#ifdef MALLOC_RED_ZONE
	/* Update extra overhead */
	c->usize = OVERHEAD;
	c->state = M_STATE_BRK;
	chunk_checksum_set(c);
	n->usize = len;
	n->state = M_STATE_BRK | M_STATE_USED;
	chunk_checksum_set(n);
#endif

	__bin_chunk(c);
	return new;
#endif
}

weak_alias(__memalign, memalign);
