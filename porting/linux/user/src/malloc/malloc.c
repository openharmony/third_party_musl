#define _GNU_SOURCE
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include "libc.h"
#include "atomic.h"
#include "pthread_impl.h"
#include "malloc_impl.h"
#include "malloc_random.h"

#ifdef USE_JEMALLOC
#include <malloc.h>
extern void* je_malloc(size_t size);
extern void* je_calloc(size_t count, size_t size);
extern void* je_realloc(void* p, size_t newsize);
extern void je_free(void* p);
#ifdef USE_JEMALLOC_DFX_INTF
extern void je_malloc_disable();
extern void je_malloc_enable();
extern int je_iterate(uintptr_t base, size_t size,
	void (*callback)(uintptr_t ptr, size_t size, void* arg), void* arg);
extern int je_mallopt(int param, int value);
#endif
#endif

#if defined(__GNUC__) && defined(__PIC__)
#define inline inline __attribute__((always_inline))
#endif

#ifdef HOOK_ENABLE
void *__libc_malloc(size_t);
void __libc_free(void *p);
#endif

static struct {
	volatile uint64_t binmap;
	struct bin bins[BINS_COUNT];
	volatile int free_lock[2];
#ifdef MUSL_ITERATE_AND_STATS_API
	occupied_bin_t occupied_bins[OCCUPIED_BIN_COUNT];
#endif
#ifdef MALLOC_FREELIST_QUARANTINE
	struct bin quarantine[QUARANTINE_NUM];
	size_t quarantined_count[QUARANTINE_NUM];
	size_t quarantined_size[QUARANTINE_NUM];
#ifdef MALLOC_RED_ZONE
	char poison[BINS_COUNT];
	volatile int poison_lock[2];
	int poison_count_down;
#endif
#endif
} mal;

int __malloc_replaced;

#ifdef MUSL_ITERATE_AND_STATS_API

/* Usable memory only, excluding overhead for chunks */
size_t total_heap_space = 0;
volatile int total_heap_space_inc_lock[2];


size_t __get_total_heap_space(void) {
	return total_heap_space;
}

static void occupied_bin_destructor(void *occupied_bin)
{
	internal_free(occupied_bin);
}

occupied_bin_t *__get_occupied_bin_by_idx(size_t bin_index)
{
	return &mal.occupied_bins[bin_index];
}

static inline size_t get_occupied_bin_index(int thread_id)
{
	return (size_t) ((size_t)thread_id % OCCUPIED_BIN_COUNT);
}

occupied_bin_t *__get_occupied_bin(struct chunk *c)
{
	size_t bin_index = get_occupied_bin_index(c->thread_id);
	return __get_occupied_bin_by_idx(bin_index);
}

occupied_bin_t *__get_current_occupied_bin()
{
	size_t bin_index = get_occupied_bin_index(__pthread_self()->tid);
	return &mal.occupied_bins[bin_index];
}
#endif

/* Synchronization tools */

static inline struct chunk *encode_chunk(struct chunk *ptr, void *key)
{
#ifndef MALLOC_FREELIST_HARDENED
	(void)key;
	return ptr;
#else
	return (struct chunk *)encode_ptr(ptr, key);
#endif
}

static inline void lock(volatile int *lk)
{
	if (libc.threads_minus_1)
		while(a_swap(lk, 1)) __wait(lk, lk+1, 1, 1);
}

static inline void unlock(volatile int *lk)
{
	if (lk[0]) {
		a_store(lk, 0);
		if (lk[1]) __wake(lk, 1, 1);
	}
}

#ifdef MUSL_ITERATE_AND_STATS_API
void __push_chunk(struct chunk *c)
{
	c->prev_occupied = c->next_occupied = NULL;
	c->thread_id = 0;

	occupied_bin_t *occupied_bin = __get_current_occupied_bin();
	lock(occupied_bin->lock);

	if (occupied_bin->head != NULL) {
		occupied_bin->head->prev_occupied = c;
		c->next_occupied = occupied_bin->head;
	} else {
		occupied_bin->tail = c;
	}
	occupied_bin->head = c;
	c->thread_id = __pthread_self()->tid;

	unlock(occupied_bin->lock);
}

void __pop_chunk(struct chunk *c)
{
	if (!c->thread_id) {
		return;
	}
	occupied_bin_t *occupied_bin = __get_occupied_bin(c);
	lock(occupied_bin->lock);

	if (c == occupied_bin->head) {
		occupied_bin->head = c->next_occupied;
	} else {
		c->prev_occupied->next_occupied = c->next_occupied;
	}
	if (c == occupied_bin->tail) {
		occupied_bin->tail = c->prev_occupied;
	} else {
		c->next_occupied->prev_occupied = c->prev_occupied;
	}
	unlock(occupied_bin->lock);
}
#endif

void malloc_disable(void)
{
#ifdef USE_JEMALLOC_DFX_INTF
	je_malloc_disable();
#elif defined(MUSL_ITERATE_AND_STATS_API)
	lock(mal.free_lock);
	lock(total_heap_space_inc_lock);
	for (size_t i = 0; i < BINS_COUNT; ++i) {
		lock(mal.bins[i].lock);
	}
	for (size_t i = 0; i < OCCUPIED_BIN_COUNT; ++i) {
		lock(mal.occupied_bins[i].lock);
	}
#endif
}

void malloc_enable(void)
{
#ifdef USE_JEMALLOC_DFX_INTF
	je_malloc_enable();
#elif defined(MUSL_ITERATE_AND_STATS_API)
	for (size_t i = 0; i < OCCUPIED_BIN_COUNT; ++i) {
		unlock(mal.occupied_bins[i].lock);
	}
	for (size_t i = 0; i < BINS_COUNT; ++i) {
		unlock(mal.bins[i].lock);
	}
	unlock(total_heap_space_inc_lock);
	unlock(mal.free_lock);
#endif
}

#ifdef MUSL_ITERATE_AND_STATS_API
typedef struct iterate_info_s {
	uintptr_t start_ptr;
	uintptr_t end_ptr;
	malloc_iterate_callback callback;
	void *arg;
} iterate_info_t;

static void malloc_iterate_visitor(void *block, size_t block_size, void *arg)
{
	iterate_info_t *iterate_info = (iterate_info_t *)arg;
	if ((uintptr_t)block >= iterate_info->start_ptr && (uintptr_t)block < iterate_info->end_ptr) {
		iterate_info->callback(block, block_size, iterate_info->arg);
	}
}

static void malloc_iterate_occupied_bin(occupied_bin_t *occupied_bin, iterate_info_t *iterate_info)
{
	for (struct chunk *c = occupied_bin->head; c != NULL; c = c->next_occupied) {
		malloc_iterate_visitor(CHUNK_TO_MEM(c), CHUNK_SIZE(c) - OVERHEAD, iterate_info);
	}
}
#endif

int malloc_iterate(void* base, size_t size, void (*callback)(void* base, size_t size, void* arg), void* arg)
{
#ifdef USE_JEMALLOC_DFX_INTF
	return je_iterate(base, size, callback, arg);
#elif defined(MUSL_ITERATE_AND_STATS_API)
	uintptr_t ptr = (uintptr_t)base;
	uintptr_t end_ptr = ptr + size;
	iterate_info_t iterate_info = {ptr, end_ptr, callback, arg};

	for (size_t i = 0; i < OCCUPIED_BIN_COUNT; ++i) {
		occupied_bin_t *occupied_bin = &mal.occupied_bins[i];
		malloc_iterate_occupied_bin(occupied_bin, &iterate_info);
	}

#endif
	return 0;
}

static inline void lock_bin(int i)
{
	lock(mal.bins[i].lock);
	if (!mal.bins[i].head) {
#ifdef MALLOC_FREELIST_HARDENED
		mal.bins[i].key = next_key();
		mal.bins[i].head = mal.bins[i].tail = encode_chunk(BIN_TO_CHUNK(i), mal.bins[i].key);
#else
		mal.bins[i].head = mal.bins[i].tail = BIN_TO_CHUNK(i);
#endif
	}
}

static inline void unlock_bin(int i)
{
	unlock(mal.bins[i].lock);
}

#ifdef MALLOC_FREELIST_QUARANTINE
static inline void lock_quarantine(int i)
{
	lock(mal.quarantine[i].lock);
	if (!mal.quarantine[i].head) {
		mal.quarantine[i].key = next_key();
		mal.quarantine[i].head = mal.quarantine[i].tail = encode_chunk(QUARANTINE_TO_CHUNK(i), mal.quarantine[i].key);
	}
}

static inline void unlock_quarantine(int i)
{
	unlock(mal.quarantine[i].lock);
}
#endif

static int first_set(uint64_t x)
{
#if 1
	return a_ctz_64(x);
#else
	static const char debruijn64[64] = {
		0, 1, 2, 53, 3, 7, 54, 27, 4, 38, 41, 8, 34, 55, 48, 28,
		62, 5, 39, 46, 44, 42, 22, 9, 24, 35, 59, 56, 49, 18, 29, 11,
		63, 52, 6, 26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
		51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
	};
	static const char debruijn32[32] = {
		0, 1, 23, 2, 29, 24, 19, 3, 30, 27, 25, 11, 20, 8, 4, 13,
		31, 22, 28, 18, 26, 10, 7, 12, 21, 17, 9, 6, 16, 5, 15, 14
	};
	if (sizeof(long) < 8) {
		uint32_t y = x;
		if (!y) {
			y = x>>32;
			return 32 + debruijn32[(y&-y)*0x076be629 >> 27];
		}
		return debruijn32[(y&-y)*0x076be629 >> 27];
	}
	return debruijn64[(x&-x)*0x022fdd63cc95386dull >> 58];
#endif
}

static const unsigned char bin_tab[60] = {
	            32,33,34,35,36,36,37,37,38,38,39,39,
	40,40,40,40,41,41,41,41,42,42,42,42,43,43,43,43,
	44,44,44,44,44,44,44,44,45,45,45,45,45,45,45,45,
	46,46,46,46,46,46,46,46,47,47,47,47,47,47,47,47,
};

static int bin_index(size_t x)
{
	x = x / SIZE_ALIGN - 1;
	if (x <= 32) return x;
	if (x < 512) return bin_tab[x/8-4];
	if (x > 0x1c00) return 63;
	return bin_tab[x/128-4] + 16;
}

static int bin_index_up(size_t x)
{
	x = x / SIZE_ALIGN - 1;
	if (x <= 32) return x;
	x--;
	if (x < 512) return bin_tab[x/8-4] + 1;
	return bin_tab[x/128-4] + 17;
}

#ifdef MALLOC_RED_ZONE
static inline size_t chunk_checksum_calculate(struct chunk *c)
{
	return (((size_t)c) ^ c->csize ^ c->usize ^ (c->state & M_STATE_MASK)) << M_CHECKSUM_SHIFT;
}

void chunk_checksum_set(struct chunk *c)
{
	c->state = (c->state & M_STATE_MASK) | chunk_checksum_calculate(c);
}

int chunk_checksum_check(struct chunk *c)
{
	return (c->state & ~M_STATE_MASK) ^ chunk_checksum_calculate(c);
}

static inline char get_poison(int i)
{
	char poison = 0;
	lock(mal.poison_lock);
	if (!mal.poison[i]) {
		mal.poison[i] = (char)(uintptr_t)next_key();
	}
	poison = mal.poison[i];
	unlock(mal.poison_lock);
	return poison;
}

static inline int need_poison(void)
{
	int ret = 0;
	lock(mal.poison_lock);
	if (mal.poison_count_down == 0) {
		/* Make sure the period is POISON_COUNT_DOWN_BASE */
		mal.poison_count_down = POISON_PERIOD - 1;
		ret = 1;
	} else {
		--mal.poison_count_down;
	}
	unlock(mal.poison_lock);
	return ret;
}

static inline void chunk_poison_set(struct chunk *c)
{
	char * start = ((char *)CHUNK_TO_MEM(c)) + c->usize;
	size_t size = CHUNK_SIZE(c) - OVERHEAD - c->usize;
	char val = get_poison(bin_index(CHUNK_SIZE(c)));
	memset(start, val, size);
	c->state |= M_RZ_POISON;
}

void chunk_poison_check(struct chunk *c)
{
	size_t csize = CHUNK_SIZE(c);
	char poison = get_poison(bin_index(csize));
	size_t padding_size = csize - OVERHEAD - c->usize;
	char *start = (char *)c + OVERHEAD + c->usize;
	for (size_t i = 0; i < padding_size; ++i) {
		/* Poison not right, crash */
		if (start[i] != poison) {
			a_crash();
		}
	}
}
#endif

#if 0
void __dump_heap(int x)
{
	struct chunk *c;
	int i;
	for (c = (void *)mal.heap; CHUNK_SIZE(c); c = NEXT_CHUNK(c))
		fprintf(stderr, "base %p size %zu (%d) flags %d/%d\n",
			c, CHUNK_SIZE(c), bin_index(CHUNK_SIZE(c)),
			c->csize & 15,
			NEXT_CHUNK(c)->psize & 15);
	for (i=0; i<BINS_COUNT; i++) {
		if (mal.bins[i].head != BIN_TO_CHUNK(i) && mal.bins[i].head) {
			fprintf(stderr, "bin %d: %p\n", i, mal.bins[i].head);
			if (!(mal.binmap & 1ULL<<i))
				fprintf(stderr, "missing from binmap!\n");
		} else if (mal.binmap & 1ULL<<i)
			fprintf(stderr, "binmap wrongly contains %d!\n", i);
	}
}
#endif

static struct chunk *expand_heap(size_t n)
{
	static int heap_lock[2];
	static void *end;
	void *p;
	struct chunk *w;

	/* The argument n already accounts for the caller's chunk
	 * overhead needs, but if the heap can't be extended in-place,
	 * we need room for an extra zero-sized sentinel chunk. */
	n += SIZE_ALIGN;

	lock(heap_lock);

#ifdef MUSL_ITERATE_AND_STATS_API
	lock(total_heap_space_inc_lock);
#endif

	p = __expand_heap(&n);
	if (!p) {
#ifdef MUSL_ITERATE_AND_STATS_API
		unlock(total_heap_space_inc_lock);
#endif
		unlock(heap_lock);
		return 0;
	}

	/* If not just expanding existing space, we need to make a
	 * new sentinel chunk below the allocated space. */
	if (p != end) {
		/* Valid/safe because of the prologue increment. */
		n -= SIZE_ALIGN;
		p = (char *)p + SIZE_ALIGN;
		w = MEM_TO_CHUNK(p);
		w->psize = 0 | C_INUSE;
	}

	/* Record new heap end and fill in footer. */
	end = (char *)p + n;
	w = MEM_TO_CHUNK(end);
	w->psize = n | C_INUSE;
	w->csize = 0 | C_INUSE;

	/* Fill in header, which may be new or may be replacing a
	 * zero-size sentinel header at the old end-of-heap. */
	w = MEM_TO_CHUNK(p);
	w->csize = n | C_INUSE;
#ifdef MALLOC_RED_ZONE
	w->state = M_STATE_BRK;
	w->usize = POINTER_USAGE;
	chunk_checksum_set(w);
#endif

#ifdef MUSL_ITERATE_AND_STATS_API
	total_heap_space += n - OVERHEAD;
	unlock(total_heap_space_inc_lock);
#endif

	unlock(heap_lock);

	return w;
}

static int adjust_size(size_t *n)
{
	/* Result of pointer difference must fit in ptrdiff_t. */
	if (*n-1 > PTRDIFF_MAX - SIZE_ALIGN - PAGE_SIZE) {
		if (*n) {
			errno = ENOMEM;
			return -1;
		} else {
			*n = SIZE_ALIGN;
			return 0;
		}
	}
#ifdef MALLOC_RED_ZONE
	/*
	 * *n + OVERHEAD + SIZE_ALIGN + 1 - 1
	 * to make sure a least 1 byte for red zone
	 */
	*n = (*n + OVERHEAD + SIZE_ALIGN) & SIZE_MASK;
#else
	*n = (*n + OVERHEAD + SIZE_ALIGN - 1) & SIZE_MASK;
#endif
	return 0;
}

static void unbin(struct chunk *c, int i)
{
#ifdef MALLOC_FREELIST_HARDENED
	void *key = mal.bins[i].key;
#else
	void *key = NULL;
#endif
	struct chunk *next = encode_chunk(c->next, key);
	struct chunk *prev = encode_chunk(c->prev, key);
	if (prev->next != encode_chunk(c, key) ||
		next->prev != encode_chunk(c, key)) {
		/* crash when the double-link list is corrupt */
		a_crash();
	}
	if (c->prev == c->next)
		a_and_64(&mal.binmap, ~(1ULL<<i));
	prev->next = c->next;
	next->prev = c->prev;
	c->csize |= C_INUSE;
	NEXT_CHUNK(c)->psize |= C_INUSE;
}

static int alloc_fwd(struct chunk *c)
{
	int i;
	size_t k;
	while (!((k=c->csize) & C_INUSE)) {
		i = bin_index(k);
		lock_bin(i);
		if (c->csize == k) {
			unbin(c, i);
			unlock_bin(i);
			return 1;
		}
		unlock_bin(i);
	}
	return 0;
}

static int alloc_rev(struct chunk *c)
{
	int i;
	size_t k;
	while (!((k=c->psize) & C_INUSE)) {
		i = bin_index(k);
		lock_bin(i);
		if (c->psize == k) {
			unbin(PREV_CHUNK(c), i);
			unlock_bin(i);
			return 1;
		}
		unlock_bin(i);
	}
	return 0;
}


/* pretrim - trims a chunk _prior_ to removing it from its bin.
 * Must be called with i as the ideal bin for size n, j the bin
 * for the _free_ chunk self, and bin j locked. */
static int pretrim(struct chunk *self, size_t n, int i, int j)
{
	size_t n1;
	struct chunk *next, *split;

	/* We cannot pretrim if it would require re-binning. */
	if (j < 40) return 0;
	if (j < i+3) {
		if (j != 63) return 0;
		n1 = CHUNK_SIZE(self);
		if (n1-n <= MMAP_THRESHOLD) return 0;
	} else {
		n1 = CHUNK_SIZE(self);
	}
	if (bin_index(n1-n) != j) return 0;

	next = NEXT_CHUNK(self);
	split = (void *)((char *)self + n);

#ifdef MALLOC_FREELIST_HARDENED
	void *key = mal.bins[j].key;
#else
	void *key = NULL;
#endif
	struct chunk *realprev = encode_chunk(self->prev, key);
	struct chunk *realnext = encode_chunk(self->next, key);
	split->prev = self->prev;
	split->next = self->next;
	realprev->next = encode_chunk(split, key);
	realnext->prev = encode_chunk(split, key);
	split->psize = n | C_INUSE;
	split->csize = n1-n;
	next->psize = n1-n;
	self->csize = n | C_INUSE;
#ifdef MALLOC_RED_ZONE
	/* split poison state keep up to self for less poison operations */
	self->state &= ~M_RZ_POISON;
	split->state = M_STATE_BRK;
	split->usize = POINTER_USAGE;
	chunk_checksum_set(split);
#endif
	return 1;
}

static void trim(struct chunk *self, size_t n)
{
	size_t n1 = CHUNK_SIZE(self);
	struct chunk *next, *split;

	if (n >= n1 - DONTCARE) return;

	next = NEXT_CHUNK(self);
	split = (void *)((char *)self + n);

	split->psize = n | C_INUSE;
	split->csize = n1-n | C_INUSE;
	next->psize = n1-n | C_INUSE;
	self->csize = n | C_INUSE;
#ifdef MALLOC_RED_ZONE
	/* Remove the poison tag, because of triming chunk */
	split->state = M_STATE_BRK;
	split->usize = POINTER_USAGE;
	chunk_checksum_set(split);
#endif

	__bin_chunk(split);
}

#ifdef HOOK_ENABLE
void *__libc_malloc(size_t n)
#else
void *malloc(size_t n)
#endif
{
#ifdef USE_JEMALLOC
	return je_malloc(n);
#endif
	return internal_malloc(n);
}

void *internal_malloc(size_t n)
{
	struct chunk *c;
	int i, j;
#ifdef MALLOC_RED_ZONE
	size_t user_size = n;
	char poison;
#endif

	if (adjust_size(&n) < 0) return 0;

	if (n > MMAP_THRESHOLD) {
		size_t len = n + OVERHEAD + PAGE_SIZE - 1 & -PAGE_SIZE;
		char *base = __mmap(0, len, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		if (base == (void *)-1) return 0;

		prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, base, len, "native_heap:musl");

		c = (void *)(base + SIZE_ALIGN - OVERHEAD);
		c->csize = len - (SIZE_ALIGN - OVERHEAD);
		c->psize = SIZE_ALIGN - OVERHEAD;
#ifdef MALLOC_RED_ZONE
		c->state = M_STATE_MMAP | M_STATE_USED;
		c->usize = user_size;
		if (need_poison()) {
			chunk_poison_set(c);
		}
		chunk_checksum_set(c);
#endif
#ifdef MUSL_ITERATE_AND_STATS_API
		__push_chunk(c);
#endif
		return CHUNK_TO_MEM(c);
	}

	i = bin_index_up(n);
	for (;;) {
		uint64_t mask = mal.binmap & -(1ULL<<i);
		if (!mask) {
			c = expand_heap(n);
			if (!c) return 0;
			if (alloc_rev(c)) {
				struct chunk *x = c;
				c = PREV_CHUNK(c);
				NEXT_CHUNK(x)->psize = c->csize =
					x->csize + CHUNK_SIZE(c);
			}
			break;
		}
		j = first_set(mask);
		lock_bin(j);
#ifdef MALLOC_FREELIST_HARDENED
		void *key = mal.bins[j].key;
#else
		void *key = NULL;
#endif
		c = encode_chunk(mal.bins[j].head, key); /* Decode the head pointer */
		if (c != BIN_TO_CHUNK(j)) {
#ifdef MALLOC_RED_ZONE
			if (c->state & M_RZ_POISON) {
				chunk_poison_check(c);
			}
#endif
			if (!pretrim(c, n, i, j)) unbin(c, j);
			unlock_bin(j);
			break;
		}
		unlock_bin(j);
	}

	/* Now patch up in case we over-allocated */
	trim(c, n);

#ifdef MALLOC_RED_ZONE
	c->usize = user_size;
	c->state |= M_STATE_USED;
	if (need_poison()) {
		chunk_poison_set(c);
	} else {
		c->state &= ~M_RZ_POISON;
	}
	chunk_checksum_set(c);
#endif
#ifdef MUSL_ITERATE_AND_STATS_API
	__push_chunk(c);
#endif
	return CHUNK_TO_MEM(c);
}

static size_t mal0_clear(char *p, size_t pagesz, size_t n)
{
#ifdef __GNUC__
	typedef uint64_t __attribute__((__may_alias__)) T;
#else
	typedef unsigned char T;
#endif
	char *pp = p + n;
	size_t i = (uintptr_t)pp & (pagesz - 1);
	for (;;) {
		pp = memset(pp - i, 0, i);
		if (pp - p < pagesz) return pp - p;
		for (i = pagesz; i; i -= 2*sizeof(T), pp -= 2*sizeof(T))
		        if (((T *)pp)[-1] | ((T *)pp)[-2])
				break;
	}
}

#ifdef HOOK_ENABLE
void *__libc_calloc(size_t m, size_t n)
#else
void *calloc(size_t m, size_t n)
#endif
{
#ifdef USE_JEMALLOC
	return je_calloc(m, n);
#endif
	if(n && m > (size_t)-1/n){
		errno=ENOMEM;
		return 0;
	}
	n *= m;
	void *p=malloc(n);
	if(!p) return p;
	if(!__malloc_replaced) {
		if(IS_MMAPPED(MEM_TO_CHUNK(p)))
			return p;
		if(n >= PAGE_SIZE)
			n = mal0_clear(p, PAGE_SIZE, n);
	}
	return memset(p, 0, n);
}

void *internal_calloc(size_t m, size_t n)
{
	if (n && m > (size_t)-1/n) {
		errno = ENOMEM;
		return 0;
	}
	n *= m;
	void *p = internal_malloc(n);
	if (!p) return p;
	if (!__malloc_replaced) {
		if (IS_MMAPPED(MEM_TO_CHUNK(p)))
			return p;
		if (n >= PAGE_SIZE)
			n = mal0_clear(p, PAGE_SIZE, n);
	}
	return memset(p, 0, n);
}

#ifdef HOOK_ENABLE
void *__libc_realloc(void *p, size_t n)
#else
void *realloc(void *p, size_t n)
#endif
{
#ifdef USE_JEMALLOC
	return je_realloc(p, n);
#endif
	struct chunk *self, *next;
	size_t n0, n1;
	void *new;
#ifdef MALLOC_RED_ZONE
	size_t user_size = n;
#endif

	if (!p) return malloc(n);
	if (!n) {
		free(p);
		return NULL;
	}

	if (adjust_size(&n) < 0) return 0;

	self = MEM_TO_CHUNK(p);
	n1 = n0 = CHUNK_SIZE(self);
#ifdef MALLOC_RED_ZONE
	/* Not a valid chunk */
	if (!(self->state & M_STATE_USED)) a_crash();
	if (chunk_checksum_check(self)) a_crash();
	if (self->state & M_RZ_POISON) chunk_poison_check(self);
#endif

	if (IS_MMAPPED(self)) {
		size_t extra = self->psize;
		char *base = (char *)self - extra;
		size_t oldlen = n0 + extra;
		size_t newlen = n + extra;
		/* Crash on realloc of freed chunk */
#ifdef MALLOC_RED_ZONE
		/* Wrong malloc type */
		if (!(self->state & M_STATE_MMAP)) a_crash();
#endif
		if (extra & 1) a_crash();
		if (newlen < PAGE_SIZE && (new = malloc(n-OVERHEAD))) {
			n0 = n;
			goto copy_free_ret;
		}
		newlen = (newlen + PAGE_SIZE-1) & -PAGE_SIZE;
		if (oldlen == newlen) return p;
#ifndef MUSL_ITERATE_AND_STATS_API
		base = __mremap(base, oldlen, newlen, MREMAP_MAYMOVE);
#else
		base = __mremap(base, oldlen, newlen, 0);
#endif
		if (base == (void *)-1)
			goto copy_realloc;
#ifndef MUSL_ITERATE_AND_STATS_API
		self = (void *)(base + extra);
#endif
		self->csize = newlen - extra;
#ifdef MALLOC_RED_ZONE
		self->usize = user_size;
		if (need_poison()) {
			chunk_poison_set(self);
		} else {
			self->state &= ~M_RZ_POISON;
		}
		chunk_checksum_set(self);
#endif
		return CHUNK_TO_MEM(self);
	}

	next = NEXT_CHUNK(self);

	/* Crash on corrupted footer (likely from buffer overflow) */
	if (next->psize != self->csize) a_crash();

	/* Merge adjacent chunks if we need more space. This is not
	 * a waste of time even if we fail to get enough space, because our
	 * subsequent call to free would otherwise have to do the merge. */
	if (n > n1 && alloc_fwd(next)) {
		n1 += CHUNK_SIZE(next);
		next = NEXT_CHUNK(next);
#ifdef MALLOC_RED_ZONE
		/* alloc forward arises, remove the poison tag */
		self->state &= ~M_RZ_POISON;
#endif
	}
	/* FIXME: find what's wrong here and reenable it..? */
	if (0 && n > n1 && alloc_rev(self)) {
		self = PREV_CHUNK(self);
		n1 += CHUNK_SIZE(self);
	}
	self->csize = n1 | C_INUSE;
	next->psize = n1 | C_INUSE;

	/* If we got enough space, split off the excess and return */
	if (n <= n1) {
		//memmove(CHUNK_TO_MEM(self), p, n0-OVERHEAD);
		trim(self, n);
#ifdef MALLOC_RED_ZONE
		self->usize = user_size;
		if (need_poison()) {
			chunk_poison_set(self);
		} else {
			self->state &= ~M_RZ_POISON;
		}
		chunk_checksum_set(self);
#endif
		return CHUNK_TO_MEM(self);
	}

copy_realloc:
	/* As a last resort, allocate a new chunk and copy to it. */
	new = malloc(n-OVERHEAD);
	if (!new) return 0;
copy_free_ret:
#ifndef MALLOC_RED_ZONE
	memcpy(new, p, n0-OVERHEAD);
#else
	memcpy(new, p, self->usize < user_size ? self->usize : user_size);
	chunk_checksum_set(self);
#endif
	free(CHUNK_TO_MEM(self));
	return new;
}

void *internal_realloc(void *p, size_t n)
{
	struct chunk *self, *next;
	size_t n0, n1;
	void *new;
#ifdef MALLOC_RED_ZONE
	size_t user_size = n;
#endif

	if (!p) return internal_malloc(n);
	if (!n) {
		internal_free(p);
		return NULL;
	}

	if (adjust_size(&n) < 0) return 0;

	self = MEM_TO_CHUNK(p);
	n1 = n0 = CHUNK_SIZE(self);
#ifdef MALLOC_RED_ZONE
	/* Not a valid chunk */
	if (!(self->state & M_STATE_USED)) a_crash();
	if (chunk_checksum_check(self)) a_crash();
	if (self->state & M_RZ_POISON) chunk_poison_check(self);
#endif

	if (IS_MMAPPED(self)) {
		size_t extra = self->psize;
		char *base = (char *)self - extra;
		size_t oldlen = n0 + extra;
		size_t newlen = n + extra;
		/* Crash on realloc of freed chunk */
#ifdef MALLOC_RED_ZONE
		/* Wrong malloc type */
		if (!(self->state & M_STATE_MMAP)) a_crash();
#endif
		if (extra & 1) a_crash();
		if (newlen < PAGE_SIZE && (new = internal_malloc(n-OVERHEAD))) {
			n0 = n;
			goto copy_free_ret;
		}
		newlen = (newlen + PAGE_SIZE-1) & -PAGE_SIZE;
		if (oldlen == newlen) return p;
		base = __mremap(base, oldlen, newlen, MREMAP_MAYMOVE);
		if (base == (void *)-1)
			goto copy_realloc;
		self = (void *)(base + extra);
		self->csize = newlen - extra;
#ifdef MALLOC_RED_ZONE
		self->usize = user_size;
		if (need_poison()) {
			chunk_poison_set(self);
		} else {
			self->state &= ~M_RZ_POISON;
		}
		chunk_checksum_set(self);
#endif
		return CHUNK_TO_MEM(self);
	}

	next = NEXT_CHUNK(self);

	/* Crash on corrupted footer (likely from buffer overflow) */
	if (next->psize != self->csize) a_crash();

	/* Merge adjacent chunks if we need more space. This is not
	 * a waste of time even if we fail to get enough space, because our
	 * subsequent call to free would otherwise have to do the merge. */
	if (n > n1 && alloc_fwd(next)) {
		n1 += CHUNK_SIZE(next);
		next = NEXT_CHUNK(next);
#ifdef MALLOC_RED_ZONE
		/* alloc forward arises, remove the poison tag */
		self->state &= ~M_RZ_POISON;
#endif
	}
	/* FIXME: find what's wrong here and reenable it..? */
	if (0 && n > n1 && alloc_rev(self)) {
		self = PREV_CHUNK(self);
		n1 += CHUNK_SIZE(self);
	}
	self->csize = n1 | C_INUSE;
	next->psize = n1 | C_INUSE;

	/* If we got enough space, split off the excess and return */
	if (n <= n1) {
		//memmove(CHUNK_TO_MEM(self), p, n0-OVERHEAD);
		trim(self, n);
#ifdef MALLOC_RED_ZONE
		self->usize = user_size;
		if (need_poison()) {
			chunk_poison_set(self);
		} else {
			self->state &= ~M_RZ_POISON;
		}
		chunk_checksum_set(self);
#endif
		return CHUNK_TO_MEM(self);
	}

copy_realloc:
	/* As a last resort, allocate a new chunk and copy to it. */
	new = internal_malloc(n-OVERHEAD);
	if (!new) return 0;
copy_free_ret:
#ifndef MALLOC_RED_ZONE
	memcpy(new, p, n0-OVERHEAD);
#else
	memcpy(new, p, self->usize < user_size ? self->usize : user_size);
	chunk_checksum_set(self);
#endif
	internal_free(CHUNK_TO_MEM(self));
	return new;
}

void __bin_chunk(struct chunk *self)
{
	struct chunk *next = NEXT_CHUNK(self);
	size_t final_size, new_size, size;
	int reclaim=0;
	int i;

	final_size = new_size = CHUNK_SIZE(self);

	/* Crash on corrupted footer (likely from buffer overflow) */
	if (next->psize != self->csize) a_crash();

	for (;;) {
		if (self->psize & next->csize & C_INUSE) {
			self->csize = final_size | C_INUSE;
			next->psize = final_size | C_INUSE;
			i = bin_index(final_size);
			lock_bin(i);
			lock(mal.free_lock);
			if (self->psize & next->csize & C_INUSE)
				break;
			unlock(mal.free_lock);
			unlock_bin(i);
		}

		if (alloc_rev(self)) {
			self = PREV_CHUNK(self);
			size = CHUNK_SIZE(self);
			final_size += size;
			if (new_size+size > RECLAIM && (new_size+size^size) > size)
				reclaim = 1;
		}

		if (alloc_fwd(next)) {
			size = CHUNK_SIZE(next);
			final_size += size;
			if (new_size+size > RECLAIM && (new_size+size^size) > size)
				reclaim = 1;
			next = NEXT_CHUNK(next);
		}
#ifdef MALLOC_RED_ZONE
		/* if poisoned chunk is combined, we should remove the poisoned tag */
		self->state &= ~M_RZ_POISON;
#endif
	}

	if (!(mal.binmap & 1ULL<<i))
		a_or_64(&mal.binmap, 1ULL<<i);

	self->csize = final_size;
	next->psize = final_size;
	unlock(mal.free_lock);

#ifdef MALLOC_FREELIST_HARDENED
	void *key = mal.bins[i].key;
#else
	void *key = NULL;
#endif
	self->next = encode_chunk(BIN_TO_CHUNK(i), key);
	self->prev = mal.bins[i].tail;
	struct chunk *xorptr = encode_ptr(self, key);
	encode_chunk(mal.bins[i].tail, key)->next = xorptr;
	mal.bins[i].tail = xorptr;

	/* Replace middle of large chunks with fresh zero pages */
	if (reclaim) {
		uintptr_t a = (uintptr_t)self + SIZE_ALIGN+PAGE_SIZE-1 & -PAGE_SIZE;
		uintptr_t b = (uintptr_t)next - SIZE_ALIGN & -PAGE_SIZE;
#if 1
		__madvise((void *)a, b-a, MADV_DONTNEED);
#else
		__mmap((void *)a, b-a, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0);
#endif
#ifdef MALLOC_RED_ZONE
		self->state &= ~M_RZ_POISON;
#endif
	}

#ifdef MALLOC_RED_ZONE
	chunk_checksum_set(self);
#endif
	unlock_bin(i);
}

static void unmap_chunk(struct chunk *self)
{
	size_t extra = self->psize;
	char *base = (char *)self - extra;
	size_t len = CHUNK_SIZE(self) + extra;
#ifdef MALLOC_RED_ZONE
	/* Wrong chunk type */
	if (!(self->state & M_STATE_MMAP)) a_crash();
#endif
	/* Crash on double free */
	if (extra & 1) a_crash();
	__munmap(base, len);
}

#ifdef MALLOC_FREELIST_QUARANTINE
static inline int quarantine_index(size_t size)
{
	return (size / SIZE_ALIGN) & (QUARANTINE_NUM - 1);
}

static void quarantine_contained(struct chunk *self)
{
	size_t size = CHUNK_SIZE(self);
	int i = quarantine_index(size);
	struct chunk *cur;
	struct chunk *next;
	struct chunk *prev;
	void *key;

#ifdef MALLOC_RED_ZONE
	/* Wrong chunk type */
	if (!(self->state & M_STATE_BRK)) a_crash();
#endif

	lock_quarantine(i);
	key = mal.quarantine[i].key;
	cur = encode_chunk(mal.quarantine[i].head, key);
	prev = QUARANTINE_TO_CHUNK(i);
	while (cur != QUARANTINE_TO_CHUNK(i)) {
		/* "Safe-unlink" check */
		next = encode_chunk(cur->next, key);
		if (prev->next != encode_chunk(cur, key) ||
			next->prev != encode_chunk(cur, key))
		a_crash();
		/* Find that "self" is in the freelist, crash */
		if (cur == self) a_crash();
		prev = cur;
		cur = next;
	}
	unlock_quarantine(i);
}

static void quarantine_bin(struct chunk *self)
{
	size_t size = CHUNK_SIZE(self);
	struct chunk *cur;
	struct chunk *next;
	void *key;
	int i;

#ifdef MALLOC_RED_ZONE
	self->state &= ~M_STATE_USED;
#endif
	/* Avoid quarantining large memory */
	if (size > QUARANTINE_THRESHOLD) {
#ifdef MALLOC_RED_ZONE
		self->usize = POINTER_USAGE;
		if ((self->psize & self->csize & NEXT_CHUNK(self)->csize & C_INUSE) &&
			need_poison()) {
			/* Self will not be combined mostly */
			chunk_poison_set(self);
		} else {
			self->state &= ~M_RZ_POISON;
		}
		chunk_checksum_set(self);
#endif
		__bin_chunk(self);
		return;
	}

	i = quarantine_index(size);
	lock_quarantine(i);
	key = mal.quarantine[i].key;
	if (mal.quarantined_size[i] > QUARANTINE_THRESHOLD || mal.quarantined_count[i] > QUARANTINE_N_THRESHOLD) {
		/* cherry-pick an independent list from quarantine */
		cur = encode_chunk(mal.quarantine[i].head, key);
		/* now clear the quarantine[i] */
		mal.quarantine[i].head = mal.quarantine[i].tail = encode_chunk(QUARANTINE_TO_CHUNK(i), key);
		mal.quarantined_size[i] = mal.quarantined_count[i] = 0;
		unlock_quarantine(i);

		/* Bin the quarantined chunk */
		do {
			next = encode_chunk(cur->next, key);
#ifdef MALLOC_RED_ZONE
			if (cur->state & M_RZ_POISON) chunk_poison_check(cur);
#endif
			__bin_chunk(cur);
			cur = next;
		} while (cur != QUARANTINE_TO_CHUNK(i));
	} else {
		unlock_quarantine(i);
	}

	lock_quarantine(i);
	self->next = encode_chunk(QUARANTINE_TO_CHUNK(i), key);
	self->prev = mal.quarantine[i].tail;
	encode_chunk(mal.quarantine[i].tail, key)->next = encode_chunk(self, key);
	mal.quarantine[i].tail = encode_chunk(self, key);

	mal.quarantined_size[i] += size;
	mal.quarantined_count[i] += 1;

#ifdef MALLOC_RED_ZONE
	if (need_poison()) {
		self->usize = POINTER_USAGE;
		chunk_poison_set(self);
	} else {
		self->state &= ~M_RZ_POISON;
	}
	chunk_checksum_set(self);
#endif
	unlock_quarantine(i);
}
#endif

#ifdef HOOK_ENABLE
void __libc_free(void *p)
#else
void free(void *p)
#endif
{
#ifdef USE_JEMALLOC
	return je_free(p);
#endif
	return internal_free(p);
}

void internal_free(void *p)
{
	if (!p) return;

	struct chunk *self = MEM_TO_CHUNK(p);
#ifdef MUSL_ITERATE_AND_STATS_API
	__pop_chunk(self);
#endif

#ifdef MALLOC_RED_ZONE
	/* This is not a valid chunk for freeing */
	if (chunk_checksum_check(self)) a_crash();
	if (!(self->state & M_STATE_USED)) a_crash();
	if (self->state & M_RZ_POISON) chunk_poison_check(self);
#endif
	if (IS_MMAPPED(self))
		unmap_chunk(self);
	else {
#ifdef MALLOC_FREELIST_QUARANTINE
		quarantine_contained(self);
		quarantine_bin(self);
#else
		__bin_chunk(self);
#endif
	}
}

void __malloc_donate(char *start, char *end)
{
#ifndef USE_JEMALLOC
	size_t align_start_up = (SIZE_ALIGN-1) & (-(uintptr_t)start - OVERHEAD);
	size_t align_end_down = (SIZE_ALIGN-1) & (uintptr_t)end;

	/* Getting past this condition ensures that the padding for alignment
	 * and header overhead will not overflow and will leave a nonzero
	 * multiple of SIZE_ALIGN bytes between start and end. */
	if (end - start <= OVERHEAD + align_start_up + align_end_down)
		return;
	start += align_start_up + OVERHEAD;
	end   -= align_end_down;

	struct chunk *c = MEM_TO_CHUNK(start), *n = MEM_TO_CHUNK(end);
	c->psize = n->csize = C_INUSE;
	c->csize = n->psize = C_INUSE | (end-start);
#ifdef MALLOC_RED_ZONE
	c->usize = POINTER_USAGE;
	c->state = M_STATE_BRK;
	chunk_checksum_set(c);
#endif
#ifdef MUSL_ITERATE_AND_STATS_API
	lock(total_heap_space_inc_lock);
	total_heap_space += CHUNK_SIZE(c) - OVERHEAD;
#endif
	__bin_chunk(c);
#ifdef MUSL_ITERATE_AND_STATS_API
	unlock(total_heap_space_inc_lock);
#endif
#endif
}

#ifdef HOOK_ENABLE
int __libc_mallopt(int param, int value)
#else
int mallopt(int param, int value)
#endif
{
#ifdef USE_JEMALLOC_DFX_INTF
	return je_mallopt(param, value);
#endif
	return 0;
}

ssize_t malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count)
{
	return 0;
}
