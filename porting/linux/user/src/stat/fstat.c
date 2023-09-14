#define _BSD_SOURCE
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"

int weak_fstat(int fd, struct stat *st)
{
	if (fd<0) return __syscall_ret(-EBADF);
	return fstatat(fd, "", st, AT_EMPTY_PATH);
}
weak_alias(weak_fstat, fstat);
#if !_REDIR_TIME64
weak_alias(weak_fstat, fstat64);
#endif
