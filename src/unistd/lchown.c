#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"
#include <unsupported_api.h>

int lchown(const char *path, uid_t uid, gid_t gid)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
#ifdef SYS_lchown
	return syscall(SYS_lchown, path, uid, gid);
#else
	return syscall(SYS_fchownat, AT_FDCWD, path, uid, gid, AT_SYMLINK_NOFOLLOW);
#endif
}
