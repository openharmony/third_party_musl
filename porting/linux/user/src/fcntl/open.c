#include <fcntl.h>
#include <stdarg.h>
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack_hook.h"
#endif
#include "syscall.h"

int open(const char *filename, int flags, ...)
{
	mode_t mode = 0;

	if ((flags & O_CREAT) || (flags & O_TMPFILE) == O_TMPFILE) {
		va_list ap;
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	int fd = __sys_open_cp(filename, flags, mode);
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	return FDTRACK_START_HOOK(__syscall_ret(fd));
#else
	return __syscall_ret(fd);
#endif
}

weak_alias(open, open64);
