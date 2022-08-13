#ifndef MALLOC_IMPL_H
#define MALLOC_IMPL_H

#include <sys/mman.h>
#include "malloc_config.h"

hidden void *__expand_heap(size_t *);

hidden void __malloc_donate(char *, char *);

hidden void *__memalign(size_t, size_t);

struct chunk {
	size_t psize, csize;
	struct chunk *next, *prev;
};

struct bin {
	volatile int lock[2];
	struct chunk *head;
	struct chunk *tail;
#ifdef MALLOC_FREELIST_HARDENED
	void *key;
#endif
};

#define SIZE_ALIGN (4*sizeof(size_t))
#define SIZE_MASK (-SIZE_ALIGN)
#define OVERHEAD (2*sizeof(size_t))
#define MMAP_THRESHOLD (0x1c00*SIZE_ALIGN)
#define DONTCARE 16
#define RECLAIM 163840

#ifdef MALLOC_FREELIST_QUARANTINE
#define QUARANTINE_MEM_SIZE 16384
#define QUARANTINE_THRESHOLD (QUARANTINE_MEM_SIZE / QUARANTINE_NUM)
#define QUARANTINE_N_THRESHOLD 32
#define QUARANTINE_NUM 8
#endif

#define CHUNK_SIZE(c) ((c)->csize & -2)
#define CHUNK_PSIZE(c) ((c)->psize & -2)
#define PREV_CHUNK(c) ((struct chunk *)((char *)(c) - CHUNK_PSIZE(c)))
#define NEXT_CHUNK(c) ((struct chunk *)((char *)(c) + CHUNK_SIZE(c)))
#define MEM_TO_CHUNK(p) (struct chunk *)((char *)(p) - OVERHEAD)
#define CHUNK_TO_MEM(c) (void *)((char *)(c) + OVERHEAD)
#define BIN_TO_CHUNK(i) (MEM_TO_CHUNK(&mal.bins[i].head))

#ifdef MALLOC_FREELIST_QUARANTINE
#define QUARANTINE_TO_CHUNK(i) (MEM_TO_CHUNK(&mal.quarantine[i].head))
#endif

#define C_INUSE  ((size_t)1)

#define IS_MMAPPED(c) !((c)->csize & (C_INUSE))

hidden void __bin_chunk(struct chunk *);

hidden extern int __malloc_replaced;

hidden void *internal_malloc(size_t n);

hidden void internal_free(void *p);

hidden void *internal_calloc(size_t m, size_t n);

hidden void *internal_realloc(void *p, size_t n);

#endif
