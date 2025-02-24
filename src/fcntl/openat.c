#include <fcntl.h>
#include <stdarg.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack_hook.h"
#endif

int openat(int fd, const char *filename, int flags, ...)
{
	mode_t mode = 0;

	if ((flags & O_CREAT) || (flags & O_TMPFILE) == O_TMPFILE) {
		va_list ap;
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

#ifdef OHOS_FDTRACK_HOOK_ENABLE
	return FDTRACK_START_HOOK(syscall_cp(SYS_openat, fd, filename, flags|O_LARGEFILE, mode));
#endif
	return syscall_cp(SYS_openat, fd, filename, flags|O_LARGEFILE, mode);
}

weak_alias(openat, openat64);
