#ifdef HOOK_ENABLE
#include <unistd.h>
#include <sys/types.h>
#include "musl_malloc.h"
#include <malloc.h>
#include "musl_malloc_dispatch_table.h"
#include "common_def.h"
#include "musl_preinit_common.h"

void* malloc(size_t bytes)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		void*ret = dispatch_table->malloc(bytes);
		return ret;
	}
	void* result = MuslMalloc(malloc)(bytes);
	if (__predict_false(result == NULL)) {
		//__musl_log(__MUSL_LOG_WARN, "malloc(%zu) failed: returning null pointer\n", bytes);
	}
	return result;
}

void free(void* mem)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		dispatch_table->free(mem);
	} else {
		MuslMalloc(free)(mem);
	}
}

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->mmap(addr, length, prot, flags, fd, offset);
	} else {
		return MuslMalloc(mmap)(addr, length, prot, flags, fd, offset);
	}
}

int munmap(void* addr, size_t length)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->munmap(addr, length);
	} else {
		return MuslMalloc(munmap)(addr, length);
	}
}

void* calloc(size_t m, size_t n)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->calloc(m, n);
	} else {
		return MuslMalloc(calloc)(m, n);
	}
}

void* realloc(void *p, size_t n)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->realloc(p, n);
	} else {
		return MuslMalloc(realloc)(p, n);
	}
}

size_t malloc_usable_size(void* addr)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->malloc_usable_size(addr);
	} else {
		return MuslMalloc(malloc_usable_size)(addr);
	}
}

int mallopt(int param, int value)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->mallopt(param, value);
	} else {
		return MuslMalloc(mallopt)(param, value);
	}
}
#endif
