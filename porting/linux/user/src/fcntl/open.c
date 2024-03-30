#include <fcntl.h>
#include <stdarg.h>
#include "musl_fdtrack_hook.h"
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
	return FDTRACK_START_HOOK(__syscall_ret(fd));
}

weak_alias(open, open64);
