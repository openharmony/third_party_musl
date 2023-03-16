#ifdef HOOK_ENABLE
#include "musl_preinit_common.h"
#include "musl_malloc.h"
#include <stdatomic.h>
#include <malloc.h>
#include <stdlib.h>

struct musl_libc_globals __musl_libc_globals;

struct MallocDispatchType __libc_malloc_default_dispatch = {
	.malloc = MuslFunc(malloc),
	.free = MuslFunc(free),
	.mmap = MuslMalloc(mmap),
	.munmap = MuslMalloc(munmap),
	.calloc = MuslFunc(calloc),
	.realloc = MuslFunc(realloc),
	.malloc_usable_size = MuslMalloc(malloc_usable_size),
	.prctl = MuslMalloc(prctl),
};

volatile atomic_bool __hook_enable_hook_flag;
volatile atomic_bool __memleak_hook_flag;

#endif
