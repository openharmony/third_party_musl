#include "memory_trace.h"
#ifdef HOOK_ENABLE
#include "common_def.h"
#include "musl_preinit_common.h"
#endif

void memtrace(void* addr, size_t size, const char* tag, bool is_using)
{
#ifdef HOOK_ENABLE
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		dispatch_table->memtrace(addr, size, tag, is_using);
	}
#endif
    return;
}
