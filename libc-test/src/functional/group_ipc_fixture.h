/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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
#ifndef GROUP_IPC_FIXTURE_H
#define GROUP_IPC_FIXTURE_H

/* Called by the owning process before its case uses a group interface. */
static int create_case_directory(gid_t gid)
{
	char path[128];
	uid_t owner = gid == (gid_t)-1 ? geteuid() : (uid_t)gid;
	gid_t group = gid == (gid_t)-1 ? getegid() : gid;
	int fd;
	int result = 0;

	if (geteuid() != owner || getegid() != group) {
		if (setgroups(0, NULL) || setgid(group) || setuid(owner)) {
			(void)t_error("cannot switch directory creator to uid=%u gid=%u: %s\n",
				(unsigned)owner, (unsigned)group, strerror(errno));
			return -1;
		}
	}
	(void)snprintf(path, sizeof path, "/dev/group/shm/%u", (unsigned)gid);
	(void)umask(0007);
	if (mkdir(path, 02770) && errno != EEXIST) {
		(void)t_error("cannot create case directory %s: %s\n", path, strerror(errno));
		return -1;
	}
	fd = open(path, O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		(void)t_error("cannot open case directory %s: %s\n", path, strerror(errno));
		return -1;
	}
	struct stat st = {0};
	if (fstat(fd, &st) || !S_ISDIR(st.st_mode)) {
		(void)t_error("case path %s is not a directory\n", path);
		(void)close(fd);
		return -1;
	}
	/* Apply setgid explicitly: mkdir need not preserve this bit. No root repair. */
	if (fchown(fd, owner, group) || fchmod(fd, 02770)) {
		(void)t_error("cannot prepare owned case directory %s: %s\n", path, strerror(errno));
		result = -1;
	}
	if (close(fd)) {
		(void)t_error("cannot close case directory %s: %s\n", path, strerror(errno));
		result = -1;
	}
	return result;
}

/* Keep the case runner's identity unchanged; only the directory creator drops privileges. */
static int prepare_case_directory(gid_t gid)
{
	int status;
	pid_t waited;
	pid_t pid = fork();

	if (!pid) {
		_exit(create_case_directory(gid) ? 1 : 0);
	}
	if (pid < 0) {
		(void)t_error("case directory fork failed for gid=%u: %s\n", (unsigned)gid, strerror(errno));
		return -1;
	}
	do {
		waited = waitpid(pid, &status, 0);
	} while (waited < 0 && errno == EINTR);
	if (waited != pid || !WIFEXITED(status) || WEXITSTATUS(status)) {
		(void)t_error("case directory preparation failed for gid=%u\n", (unsigned)gid);
		return -1;
	}
	return 0;
}

#endif
