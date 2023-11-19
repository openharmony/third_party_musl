#ifndef HOOK_ENABLE
#include <stdlib.h>

#ifdef USE_JEMALLOC
extern void je_free(void* p);
#endif

#ifdef USE_GWP_ASAN
	extern void libc_gwp_asan_free(void *mem);
#endif

void free(void *p)
{
#ifdef USE_GWP_ASAN
	return libc_gwp_asan_free(p);
#endif

#ifdef USE_JEMALLOC
	return je_free(p);
#endif
	__libc_free(p);
}
#endif