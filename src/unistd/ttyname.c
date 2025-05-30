#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <unsupported_api.h>

char *ttyname(int fd)
{
	static char buf[TTY_NAME_MAX];
	int result;
	UNSUPPORTED_API_VOID(LITEOS_A);
	if ((result = ttyname_r(fd, buf, sizeof buf))) {
		errno = result;
		return NULL;
	}
	return buf;
}
