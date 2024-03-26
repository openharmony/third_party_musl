#define _BSD_SOURCE
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"

#ifndef __LITEOS__
int __fstat(int fd, struct stat *st)
#else
int fstat(int fd, struct stat *st)
#endif
{
	if (fd<0) return __syscall_ret(-EBADF);
	return fstatat(fd, "", st, AT_EMPTY_PATH);
}

#ifndef __LITEOS__
weak_alias(__fstat, fstat);
#if !_REDIR_TIME64
weak_alias(__fstat, fstat64);
#endif
#else
#if !_REDIR_TIME64
weak_alias(fstat, fstat64);
#endif
#endif
