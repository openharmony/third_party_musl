#ifndef HOOK_ENABLE
#include <stdlib.h>
#ifdef USE_JEMALLOC
extern void* je_calloc(void* p, size_t newsize);
#endif

#ifdef USE_GWP_ASAN
	extern void* libc_gwp_asan_calloc(size_t nmemb, size_t size);
#endif

void *calloc(size_t m, size_t n)
{
#ifdef USE_GWP_ASAN
	return libc_gwp_asan_calloc(m, n);
#endif

#ifdef USE_JEMALLOC
	return je_calloc(m, n);
#endif
    return __libc_calloc(m, n);
}
#endif