#include <unistd.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "memory_trace.h"
#include "musl_fdtrack_hook.h"
#endif

int dup(int fd)
{
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	int fd_new = syscall(SYS_dup, fd);
	int res = FDTRACK_START_HOOK(fd_new);
	restrace(RES_FD_DUP, res, FD_SIZE, TAG_RES_FD_DUP, true);
	return res;
#endif
	return syscall(SYS_dup, fd);
}
