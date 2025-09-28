#include <sys/socket.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "memory_trace.h"
#include "musl_fdtrack_hook.h"
#endif

int accept(int fd, struct sockaddr *restrict addr, socklen_t *restrict len)
{
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	restrace(RES_FD_SOCKET, fd, FD_SIZE, TAG_RES_FD_SOCKET, true);
	return FDTRACK_START_HOOK(socketcall_cp(accept, fd, addr, len, 0, 0, 0));
#endif
	return socketcall_cp(accept, fd, addr, len, 0, 0, 0);
}
