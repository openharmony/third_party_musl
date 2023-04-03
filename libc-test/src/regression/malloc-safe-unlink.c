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

static void handler(int s)
{
}

volatile void *tmp;

#define MALLOC_SIZE 40

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
	uintptr_t *c;
	uintptr_t *d;
	uintptr_t *temp;

	/* Set first dividing chunk */
	if (set_devide_chunk(MALLOC_SIZE))
		return -1;

	/*
	 * The init procedure makes the freelist unpredictable. To make sure trigger the safe-unlink
	 * check, Here we create as many chunks as possible to make sure there are enough chunks in
	 * bin[0] and malloc again. Basically this is heap spray.
	 */
	for (int i = 0; i < 512; ++i) {
		c = (uintptr_t *)malloc(MALLOC_SIZE);
		if (!c) {
			t_error("Malloc failed: %s\n", strerror(errno));
			return -1;
		}

		if (set_devide_chunk(MALLOC_SIZE)) {
			free((void *)c);
			return -1;
		}

		d = (uintptr_t *)malloc(MALLOC_SIZE);
		if (!d) {
			t_error("Malloc failed: %s\n", strerror(errno));
			free((void *)c);
			return -1;
		}

		if (set_devide_chunk(MALLOC_SIZE)) {
			free((void *)d);
			free((void *)c);
			return -1;
		}

		free((void *)d);
		free((void *)c);

		/* exchange the prev and next pointer */
		uintptr_t temp = c[0];
		c[0] = c[1];
		c[1] = temp;
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
