#include <sys/socket.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "memory_trace.h"
#include "musl_fdtrack_hook.h"
#endif

int accept(int fd, struct sockaddr *restrict addr, socklen_t *restrict len)
{
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	int fd_new = socketcall_cp(accept, fd, addr, len, 0, 0, 0);
	restraceFd(RES_FD_SOCKET, fd_new, TAG_RES_FD_SOCKET, true);
	return FDTRACK_START_HOOK(fd_new);
#endif
	return socketcall_cp(accept, fd, addr, len, 0, 0, 0);
}
