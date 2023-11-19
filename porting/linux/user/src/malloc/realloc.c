#ifndef HOOK_ENABLE
#include <stdlib.h>

#ifdef USE_JEMALLOC
extern void* je_realloc(void* p, size_t newsize);
#endif

#ifdef USE_GWP_ASAN
	extern size_t libc_gwp_asan_realloc(void *ptr, size_t size);
#endif

void *realloc(void *p, size_t n)
{
#ifdef USE_GWP_ASAN
	return libc_gwp_asan_realloc(p, n);
#endif
#ifdef USE_JEMALLOC
	return je_realloc(p, n);
#endif
	return __libc_realloc(p, n);
}
#endif
