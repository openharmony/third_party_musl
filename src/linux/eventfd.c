#include <sys/eventfd.h>
#include <unistd.h>
#include <errno.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack_hook.h"
#endif

int eventfd(unsigned int count, int flags)
{
	int r = __syscall(SYS_eventfd2, count, flags);
#ifdef SYS_eventfd
	if (r==-ENOSYS && !flags) r = __syscall(SYS_eventfd, count);
#endif

#ifdef OHOS_FDTRACK_HOOK_ENABLE
	return FDTRACK_START_HOOK(__syscall_ret(r));
#endif
	return __syscall_ret(r);
}

int eventfd_read(int fd, eventfd_t *value)
{
	return (sizeof(*value) == read(fd, value, sizeof(*value))) ? 0 : -1;
}

int eventfd_write(int fd, eventfd_t value)
{
	return (sizeof(value) == write(fd, &value, sizeof(value))) ? 0 : -1;
}
