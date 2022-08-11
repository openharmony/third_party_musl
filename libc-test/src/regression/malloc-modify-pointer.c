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

volatile uintptr_t *p0;
volatile uintptr_t *p1;
volatile void *tmp;

static int child(void)
{
	p0 = (uintptr_t *)malloc(10 * sizeof(uintptr_t));
	if (!p0) {
		t_error("Malloc failed:%s\n", strerror(errno));
		return -1;
	}
	/* Malloc a dividing chunk to avoid combination of neighbouring freed chunk */
	tmp = malloc(10 * sizeof(uintptr_t));
	/* Malloc another chunk to get a key */
	p1 = (uintptr_t *)malloc(10 * sizeof(uintptr_t));
	if (!p1) {
		t_error("Malloc failed:%s\n", strerror(errno));
		return -1;
	}
	/* Malloc a dividing chunk to avoid combination of neighbouring freed chunk */
	tmp = malloc(10 * sizeof(uintptr_t));

	free((void *)p0);
	free((void *)p1);

	uintptr_t *fake_ptr = (uintptr_t *)((uintptr_t)((char *)p1 - sizeof(size_t) * 2) ^ (uintptr_t)p0[0]);
	p0[0] = (uintptr_t)fake_ptr;
	p1[0] = (uintptr_t)fake_ptr;

	/*
	 * The init procedure makes the freelist unpredictable. To make sure to trigger the ivalid ptr
	 * acess, here we create as many chunks as possible to make sure there are enough chunks in
	 * bin[j] of size "10 * sizeof(uintptr_t)". Basically this is heap spray.
	 */
	for (int i = 0; i < 512; ++i) {
		tmp = malloc(10 *sizeof(uintptr_t));
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
