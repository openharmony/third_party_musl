/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "test.h"

#define UNIT            16
#define OFF_OFFSET      2
#define FIRST_OFFSET    (-4)
#define FIRST_OFF_OFFSET 8
#define MALLOC_SIZE_S   50
#define MALLOC_SIZE_L   100
#define TEST_NUM        512

struct meta_in {
	struct meta_in *prev, *next;
	uintptr_t *mem;
};

struct group_in {
	struct meta_in *meta;
};

static struct group_in *get_group(const uint8_t *p)
{
	int offset = *(const uint16_t *)(p - OFF_OFFSET);

	if (p[FIRST_OFFSET]) {
		offset = *(uint32_t *)(p - FIRST_OFF_OFFSET);
	}
	struct group_in *base = (void *)(p - UNIT*offset - UNIT);
	return base;
}

static void handler(int s)
{
}

volatile void *tmp;

int set_devide_chunk(size_t size)
{
	if (!(tmp = malloc(size))) {
		t_error("Malloc failed: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

static int child(void)
{
	uint8_t *c1;
	uint8_t *c2;

	uint8_t *temp;
	struct group_in *g1 = NULL;
	struct group_in *g2 = NULL;

	for (int i = 0; i < TEST_NUM; ++i) {
		c1 = (uint8_t *)malloc(MALLOC_SIZE_S);
		if (!c1) {
			t_error("Malloc failed: %s\n", strerror(errno));
			return -1;
		}
		g1 = get_group(c1);
		c2 = (uint8_t *)malloc(MALLOC_SIZE_L);
		if (!c2) {
			t_error("Malloc failed: %s\n", strerror(errno));
			return -1;
		}
		g2 = get_group(c2);
		g2->meta = g1->meta;
		free(c2);
		free(c1);
	}

	return 0;
}

static pid_t start_child(void)
{
	pid_t pid;
	int ret;
	pid = fork();
	if (pid == 0) {
		ret = child();
		t_error("child process normally out with %d\n", ret);
		return ret;
	}
	return pid;
}

int main(int argc, char *argv[])
{
	sigset_t set;
	int status;
	pid_t pid;
	int flag = 0;

	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	sigprocmask(SIG_BLOCK, &set, 0);
	signal(SIGCHLD, handler);
	signal(SIGILL, SIG_DFL);

	pid = start_child();
	if (pid == -1) {
		t_error("%s fork failed: %s\n", argv[0], strerror(errno));
		return -1;
	}
	if (sigtimedwait(&set, 0, &(struct timespec){5, 0}) == -1) { /* Wait for 5 seconds */
		if (errno == EAGAIN)
			flag = 1;
		else
			t_error("%s sigtimedwait failed: %s\n", argv[0], strerror(errno));
		if (kill(pid, SIGKILL) == -1)
			t_error("%s kill failed: %s\n", argv[0], strerror(errno));
	}

	if (waitpid(pid, &status, 0) != pid) {
		t_error("%s waitpid failed: %s\n", argv[0], strerror(errno));
		return -1;
	}

	if (flag) {
		t_error("Child process time out\n");
	}

	if (WIFSIGNALED(status)) {
		if (WTERMSIG(status) != SIGSEGV && WTERMSIG(status) != SIGILL) {
			t_error("%s child process out with %s\n", argv[0], strsignal(WTERMSIG(status)));
			return -1;
		}
	} else {
		t_error("%s child process finished normally\n", argv[0]);
	}
	return 0;
}
