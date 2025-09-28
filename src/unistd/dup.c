#include <unistd.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "memory_trace.h"
#include "musl_fdtrack_hook.h"
#endif

int dup(int fd)
{
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	restrace(RES_FD_DUP, fd, FD_SIZE, TAG_RES_FD_DUP, true);
	return FDTRACK_START_HOOK(syscall(SYS_dup, fd));
#endif
	return syscall(SYS_dup, fd);
}
