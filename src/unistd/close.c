#include <unistd.h>
#include <errno.h>
#include "aio_impl.h"
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "memory_trace.h"
#endif

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int __close(int fd)
{
	// Hook fd close
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	restrace(RES_FD_MASK, fd, FD_SIZE, TAG_RES_FD_ALL, false);
#endif
	fd = __aio_close(fd);
	int r = __syscall_cp(SYS_close, fd);
	if (r == -EINTR) r = 0;
	return __syscall_ret(r);
}

weak_alias(__close, close);
