#include <sys/stat.h>
#include <fcntl.h>
#include "syscall.h"
#include <unsupported_api.h>

int mknod(const char *path, mode_t mode, dev_t dev)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
#ifdef SYS_mknod
	return syscall(SYS_mknod, path, mode, dev);
#else
	return syscall(SYS_mknodat, AT_FDCWD, path, mode, dev);
#endif
}
