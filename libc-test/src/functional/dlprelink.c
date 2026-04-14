/*
* Copyright (C) 2026 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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

#define PATH_PREFIX "/data/local/tmp/"
#define SOLIST_PATH "/data/local/tmp/prelink_so_list"
#define SOLIST_PATH_WRG "/data/local/tmp/prelink_so_list_empty"

#define SO_FOR_DLOPEN_DSO "libdlopen_dso.so"
#define SO_FOR_NO_DELETE "lib_for_no_delete.so"
#define SO_FOR_DLOPEN "lib_for_dlopen.so"
#define SO_LOAD_BY_LOCAL "libdlopen_for_load_by_local_dso.so"
#define SO_LOAD_BY_GLOBAL "libdlopen_for_load_by_global_dso.so"
#define SO_CLOSE_RECURSIVE_OPEN_SO "libdlclose_recursive_dlopen_so.so"
#define DLOPEN_WEAK "libdlopen_weak.so"
#define DLOPEN_GLOBAL "libdlopen_global.so"
#define DLOPEN_LOCAL "libdlopen_local.so"
#define DLCLOSE_EXIT_DEAD_LOCK "libdl_multithread_test_dso.so"
#define DLCLOSE_WITH_TLS "libdlclose_tls.so"

static char *so_list[] = {
	PATH_PREFIX "libfoo.so\n"
	PATH_PREFIX SO_FOR_NO_DELETE "\n",
};

static int prelink_memfd = -1;
static int closed_memfd = -1;

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

static void destroy_memfd(int fd)
{
	if (fd > 0) {
		close(fd);
	}
	fd = -1;
}

static int dlprelink_wrong_case_prepare(void)
{
	int err = 0;
	int fd = memfd_create("tmp", MFD_ALLOW_SEALING);
	if (fd < 0) {
		err = fd;
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	} else {
		closed_memfd = fd;
	}
	if (err == 0) {
		err = close(fd);
		if (err < 0) {
			err = fd;
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		}
	}

	if (err == 0) {
		fd = open(SOLIST_PATH_WRG, O_RDWR | O_CREAT);
		if (fd < 0) {
			err = fd;
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		} else {
			close(fd);
		}
	}
	return err;
}

static int dlprelink_record_wrong_case1(void)
{
	int err = 0;
	err = fcntl(closed_memfd, F_GETFD);
	if (err == -1 && errno == EBADF) {
		err = 0;
	} else {
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		err = -1;
	}
	if (err == 0) {
		err = dlprelink_record(closed_memfd, SOLIST_PATH);
		if (err != -1) {
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			err = -1;
		} else {
			err = 0;
		}
	}
	return err;
}

static int dlprelink_record_wrong_case2(void)
{
	int err = 0;
	err = dlprelink_record(prelink_memfd, SOLIST_PATH_WRG);
	if (err != -1) {
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		err = -1;
	} else {
		err = 0;
	}
	return err;
}

static int dlprelink_record_correct(void)
{
	int err = 0;
	err = dlprelink_record(prelink_memfd, SOLIST_PATH);
	if (err != 0) {
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	}
	return err;
}

static int dlprelink_register_wrong_case1(void)
{
	int err = 0;
	err = fcntl(closed_memfd, F_GETFD);
	if (err == -1 && errno == EBADF) {
		err = 0;
	} else {
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		err = -1;
	}
	if (err == 0) {
		err = dlprelink_register(closed_memfd);
		if (err != -1) {
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			err = -1;
		} else {
			err = 0;
		}
	}
	return err;
}

static int dlprelink_register_correct(void)
{
	int err = 0;
	err = dlprelink_register(prelink_memfd);
	if (err != 0) {
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	}
	return err;
}

typedef int (*test_case)(void);
static int dlprelink_fork(test_case testcase, bool need_setfd, int fd, int exp_ret)
{
	int err = 0;
	int ws = 0;
	fflush(stdout);
	pid_t prelinker_pid = fork();
	if (prelinker_pid < 0) {
		err = prelinker_pid;
		t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	} else if (prelinker_pid == 0) {
		if (err == 0 && need_setfd) {
			err = fcntl(fd, F_SETFD, 0);
			if (err != 0) {
				t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			}
		}
		err = testcase();
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

static int dlprelink_for_proc_appspawn(void)
{
	int err = 0;
	err = prepare_memfd(&prelink_memfd);
	if (err == 0) {
		err = dlprelink_wrong_case_prepare();
	}
	printf("==== BEG: dlprelink for APPSPAWN ====!\n");

	test_case record_case[] = {
		dlprelink_record_wrong_case1,
		dlprelink_record_wrong_case2,
		dlprelink_record_correct,
	};

	for (unsigned int i = 0; i < ARRAY_LENGTH(record_case) && err == 0; i++) {
		err = dlprelink_fork(record_case[i], true, prelink_memfd, 0);
		if (err != 0) {
			t_error("[%s:%d]: err: %d, record_case %d\n", __FUNCTION__, __LINE__, err, i + 1);
		}
	}

	if (err == 0) {
		err = fcntl(prelink_memfd, F_ADD_SEALS, F_SEAL_SEAL | F_SEAL_GROW | F_SEAL_SHRINK | F_SEAL_FUTURE_WRITE);
		if (err != 0) {
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		}
	}

	test_case register_case[] = {
		dlprelink_register_wrong_case1,
	};

	for (unsigned int i = 0; i < ARRAY_LENGTH(register_case) && err == 0; i++) {
		err = register_case[i]();
	}

	printf("==== END: dlprelink for APPSPAWN ====!\n");
	destroy_memfd(prelink_memfd);
	return err;
}

static int dlprelink_execl(void)
{
#if defined(__aarch64__)
	int err = execl("/system/lib/ld-musl-aarch64.so.1", "prelinker", SOLIST_PATH, (char *)NULL);
#elif defined(__arm__)
	int err = execl("/system/lib/ld-musl-arm.so.1", "prelinker", SOLIST_PATH, (char *)NULL);
#else
	int err = -1;
#endif
	/* memfd is not opened, so it should fail. */
	err = err == 0 ? -1 : 0;
	return err;
}

static int dlprelink_for_proc_init(void)
{
	int err = 0;
	printf("==== BEG: dlprelink for INIT ====!\n");

	if (err == 0) {
		err = dlprelink_fork(dlprelink_execl, false, -1, 1);
		if (err != 0) {
			t_error("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		}
	}

	printf("==== END: dlprelink for INIT ====!\n");
	return err;
}

int main(int argc, char *argv[])
{
	int err = 0;

	err = prepare_file_and_reserve_mem();

	if (err == 0) {
		err = dlprelink_for_proc_appspawn();
	}
	if (err == 0) {
		err = dlprelink_for_proc_init();
	}

	return t_status;
}
