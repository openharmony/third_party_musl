#include <sys/group_ipc.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <unsupported_api.h>

char *__shm_mapname(const char *name, char *buf)
{
	char *p;
	while (*name == '/') name++;
	if (*(p = __strchrnul(name, '/')) || p==name ||
	    (p-name <= 2 && name[0]=='.' && p[-1]=='.')) {
		errno = EINVAL;
		return 0;
	}
	if (p-name > NAME_MAX) {
		errno = ENAMETOOLONG;
		return 0;
	}
	memcpy(buf, "/dev/shm/", 9);
	memcpy(buf+9, name, p-name+1);
	return buf;
}

char *__group_shm_mapname(const char *name, gid_t gid, char *buf)
{
	char *p;
	int len;

	while (*name == '/') {
		name++;
	}
	if (*(p = __strchrnul(name, '/')) || p==name ||
	    (p-name <= 2 && name[0]=='.' && p[-1]=='.')) {
		errno = EINVAL;
		return 0;
	}
	if (p-name > NAME_MAX) {
		errno = ENAMETOOLONG;
		return 0;
	}
	len = snprintf(buf, __GROUP_SHM_PATH_MAX, "/dev/group/shm/%u/", (unsigned)gid);
	if (len < 0 || (size_t)len >= __GROUP_SHM_PATH_MAX) {
		errno = ENAMETOOLONG;
		return 0;
	}
	(void)memcpy(buf+len, name, p-name+1);
	return buf;
}

int shm_open(const char *name, int flag, mode_t mode)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	int cs;
	char buf[NAME_MAX+10];
	if (!(name = __shm_mapname(name, buf))) return -1;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	int fd = open(name, flag|O_NOFOLLOW|O_CLOEXEC|O_NONBLOCK, mode);
	pthread_setcancelstate(cs, 0);
	return fd;
}

int shm_unlink(const char *name)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	char buf[NAME_MAX+10];
	if (!(name = __shm_mapname(name, buf))) return -1;
	return unlink(name);
}

int group_shm_open(const char *name, int flag, mode_t mode, gid_t gid)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	int cs;
	char buf[__GROUP_SHM_PATH_MAX];
	if (!(name = __group_shm_mapname(name, gid, buf))) {
		return -1;
	}
	(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	int fd = open(name, flag|O_NOFOLLOW|O_CLOEXEC|O_NONBLOCK, mode);
	(void)pthread_setcancelstate(cs, 0);
	return fd;
}

int group_shm_unlink(const char *name, gid_t gid)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	int result;
	char buf[__GROUP_SHM_PATH_MAX];
	if (!(name = __group_shm_mapname(name, gid, buf))) {
		return -1;
	}
	result = unlink(name);
	return result;
}
