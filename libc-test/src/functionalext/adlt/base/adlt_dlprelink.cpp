/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
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
#include "adlt_dlprelink.h"

#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <link.h>
#include <errno.h>

#define PATH_PREFIX "./"
#define SOLIST_PATH "/data/local/tmp/prelink_so_list"

#define SO_FOR_DLOPEN_C "libadlt_base_dlopen_c_lib.so"
#define SO_FOR_DLOPEN_C_TEST "libadlt_base_dlopen_c_test_dlopen.so"
#define SO_FOR_DLOPEN_CPP "libadlt_base_dlopen_cpp_lib.so"
#define SO_FOR_DLOPEN_CPP_TEST "libadlt_base_dlopen_cpp_test_dlopen.so"

static const char *so_list[] = {
	PATH_PREFIX SO_FOR_DLOPEN_C "\n",
	PATH_PREFIX SO_FOR_DLOPEN_C_TEST "\n",
	PATH_PREFIX SO_FOR_DLOPEN_CPP "\n",
	PATH_PREFIX SO_FOR_DLOPEN_CPP_TEST "\n",
};

static int prelink_memfd = -1;

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

static int prepare_file_and_reserve_mem(void)
{
	int err = 0;
	remove(SOLIST_PATH);
	int fd = open(SOLIST_PATH, O_RDWR | O_CREAT);
	if (fd < 0) {
		printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		err = -1;
	}

	for (unsigned int i = 0; i < ARRAY_LENGTH(so_list) && err == 0; i++) {
		ssize_t sz = write(fd, so_list[i], strlen(so_list[i]));
		if (sz <= 0) {
			printf("[%s:%d]: err: %zd\n", __FUNCTION__, __LINE__, sz);
			err = -1;
		}
	}

	if (err == 0) {
		err = close(fd);
		if (err != 0) {
			printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
		}
	}

	if (err == 0) {
		err = dlprelink_reserve_mem();
		if (err != 0) {
			printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
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
		printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
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
		printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
	} else if (prelinker_pid == 0) {
		if (need_setfd) {
			err = fcntl(fd, F_SETFD, 0);
			if (err != 0) {
				printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			}
		}
		if (err == 0) {
			err = dlprelink_record(prelink_memfd, SOLIST_PATH);
			if (err != 0) {
				printf("[%s:%d]: err: %d\n", __FUNCTION__, __LINE__, err);
			}
		}
		if (err != 0) {
			printf("[%s:%d]: ret: %d\n", __FUNCTION__, __LINE__, err);
		}
		exit(err);
	}
	if (err == 0) {
		err = waitpid(prelinker_pid, &ws, 0);
		if (err != prelinker_pid || WIFEXITED(ws) == 0 || WEXITSTATUS(ws) != exp_ret) {
			printf("[%s:%d]: ret: %d, ws: %d, expect_pid: %d, WIFEXITED: %d, WEXITSTATUS: %d\n",
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
