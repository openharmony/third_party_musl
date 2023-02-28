#ifndef MALLOC_IMPL_H
#define MALLOC_IMPL_H

#include <sys/mman.h>
#include "malloc_config.h"

hidden void *__expand_heap(size_t *);

hidden void __malloc_donate(char *, char *);

hidden void *__memalign(size_t, size_t);

#ifdef MUSL_ITERATE_AND_STATS_API 
typedef struct occupied_bin_s {
	struct chunk *head, *tail;
	volatile int lock[2];
} occupied_bin_t;
#endif

struct chunk {
	size_t psize, csize;
#ifdef MALLOC_RED_ZONE
	size_t usize;
	size_t state;
#endif
#ifdef MUSL_ITERATE_AND_STATS_API
	size_t thread_id;
	struct chunk *next_occupied, *prev_occupied;
#endif
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

#ifdef MUSL_ITERATE_AND_STATS_API
typedef void (*malloc_iterate_callback)(void* base, size_t size, void* arg);

hidden occupied_bin_t *__get_occupied_bin_by_idx(size_t bin_index);
hidden occupied_bin_t *__get_occupied_bin(struct chunk *c);
hidden occupied_bin_t *__get_current_occupied_bin();
hidden void __push_chunk(struct chunk *c);
hidden void __pop_chunk(struct chunk *c);
hidden size_t __get_total_heap_space(void);
#endif

#define BINS_COUNT 64

#define SIZE_MASK (-SIZE_ALIGN)

#ifdef MUSL_ITERATE_AND_STATS_API
#define OCCUPIED_BIN_COUNT 64
#define OCCUPIED_LIST_OVERHEAD (2*sizeof(void*))
#define ITERATE_AND_STATS_OVERHEAD (sizeof(size_t) + OCCUPIED_LIST_OVERHEAD)
#else
#define ITERATE_AND_STATS_OVERHEAD (0)
#endif

#ifdef MUSL_ITERATE_AND_STATS_API
#ifndef MALLOC_RED_ZONE
#define SIZE_ALIGN (8*sizeof(size_t))
#define OVERHEAD (2*sizeof(size_t) + ITERATE_AND_STATS_OVERHEAD)
#else
#define SIZE_ALIGN (16*sizeof(size_t))
#define OVERHEAD (4*sizeof(size_t) + ITERATE_AND_STATS_OVERHEAD)
#endif
#else
#ifndef MALLOC_RED_ZONE
#define SIZE_ALIGN (4*sizeof(size_t))
#define OVERHEAD (2*sizeof(size_t))
#else
#define SIZE_ALIGN (8*sizeof(size_t))
#define OVERHEAD (4*sizeof(size_t))
#endif
#endif

#define MMAP_THRESHOLD (0x1c00*SIZE_ALIGN)
#ifndef MALLOC_RED_ZONE
#ifdef MUSL_ITERATE_AND_STATS_API
#define DONTCARE OVERHEAD
#else
#define DONTCARE 16
#endif
#else
#define DONTCARE OVERHEAD
#define POINTER_USAGE (2*sizeof(void *))
#endif
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

#ifdef MALLOC_RED_ZONE
#define M_STATE_FREE   0x00
#define M_STATE_USED   0x01U
#define M_STATE_BRK    0x02U
#define M_STATE_MMAP   0x04U

#define M_RZ_NONE   0x00
#define M_RZ_POISON 0x10U

#define M_STATE_MASK   0xffU
#define M_CHECKSUM_SHIFT 8

#define POISON_PERIOD 31
#endif

#define C_INUSE  ((size_t)1)

#define IS_MMAPPED(c) !((c)->csize & (C_INUSE))

hidden void __bin_chunk(struct chunk *);

hidden extern int __malloc_replaced;

hidden void *internal_malloc(size_t n);

hidden void internal_free(void *p);

hidden void *internal_calloc(size_t m, size_t n);

hidden void *internal_realloc(void *p, size_t n);

hidden size_t internal_malloc_usable_size(void *p);

#ifdef MALLOC_RED_ZONE
hidden void chunk_checksum_set(struct chunk *c);

hidden int chunk_checksum_check(struct chunk *c);
#endif

#endif
