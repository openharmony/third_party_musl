#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <link.h>
#include <sys/mman.h>
#include <errno.h>

#include "test.h"
#include "dlprelink.h"


#define PATH_PREFIX "/data/local/tmp/"
#define SOLIST_PATH "/data/local/tmp/prelink_so_list"

#define SO_FOR_NO_DELETE "lib_for_no_delete.so"

static char *so_list[] = {
	PATH_PREFIX "libfoo.so\n"
	PATH_PREFIX SO_FOR_NO_DELETE "\n",
};

static int prelink_memfd = -1;

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

static int prepare_file_and_reserve_mem(void)
{
	int err = 0;
	remove(SOLIST_PATH);
	int fd = open(SOLIST_PATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		err = -1;
	}

	for (unsigned int i = 0; i < ARRAY_LENGTH(so_list) && err == 0; i++) {
		size_t len = strlen(so_list[i]);
		ssize_t sz = write(fd, so_list[i], len);
		if ((size_t)sz != len) {
			t_error("[%s:%d]: err: %zd\n", __FUNCTION__, __LINE__, sz);
			err = -1;
		}
	}

	if (err == 0) {
		err = close(fd);
		if (err != 0) {
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		}
	}

	if (err == 0) {
		err = dlprelink_reserve_mem();
		if (err != 0) {
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		}
	}
	return err;
}

static int prepare_memfd(int *rfd)
{
	int err = 0;
	int fd = memfd_create("relro_cache", MFD_ALLOW_SEALING);
	if (fd < 0) {
		err = fd;
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	} else {
		*rfd = fd;
	}
	return err;
}

static int dlprelink_fork(bool need_setfd, int fd, int exp_ret)
{
	int err = 0;
	int ws = 0;
	fflush(stdout);
	pid_t prelinker_pid = fork();
	if (prelinker_pid < 0) {
		err = prelinker_pid;
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	} else if (prelinker_pid == 0) {
		if (need_setfd) {
			err = fcntl(fd, F_SETFD, 0);
			if (err != 0) {
				t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			}
		}
		if (err == 0) {
			err = dlprelink_record(prelink_memfd, SOLIST_PATH);
			if (err != 0) {
				t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			}
		}
		if (err != 0) {
			t_error("[%s:%d]: ret: %d\n", __FUNCTION__, __LINE__, err);
		}
		exit(err);
	}
	if (err == 0) {
		err = waitpid(prelinker_pid, &ws, 0);
		if (err != prelinker_pid || WIFEXITED(ws) == 0 || WEXITSTATUS(ws) != exp_ret) {
			t_error("[%s:%d]: ret: %d, ws: %d, expect_pid: %d, WIFEXITED: %d, WEXITSTATUS: %d\n",
				__FUNCTION__, __LINE__, err, ws, prelinker_pid, WIFEXITED(ws), WEXITSTATUS(ws));
		} else {
			err = 0;
		}
	}
	return err;
}

int dlprelink(void)
{
	int err = 0;

	err = prepare_file_and_reserve_mem();

	if (err == 0) {
		err = prepare_memfd(&prelink_memfd);
	}

	if (err == 0) {
		err = dlprelink_fork(true, prelink_memfd, 0);
	}

	if (err == 0) {
		err = fcntl(prelink_memfd, F_ADD_SEALS, F_SEAL_SEAL | F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_FUTURE_WRITE);
	}

	if (err == 0) {
		err = dlprelink_register(prelink_memfd);
	}

	return err;
}
