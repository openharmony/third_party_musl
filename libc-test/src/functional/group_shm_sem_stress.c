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

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <poll.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/group_ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "test.h"
#include "group_ipc_fixture.h"

#define TEST_GID_A_DEFAULT 198401U
#define TEST_GID_B_DEFAULT 198402U
/* 默认使用短时稳定性回归负载，长时间压力测试通过环境变量提高轮数和进程数。 */
#define DEFAULT_STRESS_ROUNDS 100
#define DEFAULT_COUNTER_WORKERS 4
#define DEFAULT_COUNTER_LOOPS 100
#define DEFAULT_RACE_WORKERS 4
#define DEFAULT_RACE_ROUNDS 10
#define DEFAULT_CHURN_WORKERS 4
#define DEFAULT_CHURN_ROUNDS 100
#define WAIT_TIMEOUT_MS 10000

/* Preserve errno for assertions and flush before a child can call _exit. */
#define TEST_LOG(...) do { \
	int log_errno = errno; \
	(void)printf("%s:%d: %s: pid=%d errno=%d (%s): ", \
		__FILE__, __LINE__, __func__, (int)getpid(), log_errno, strerror(log_errno)); \
	errno = log_errno; \
	(void)printf(__VA_ARGS__); \
	(void)fflush(stdout); \
	errno = log_errno; \
} while (0)

/*
 * This binary is excluded from the default short libc-test run. Release
 * stability runs can tune the duration with LIBC_TEST_GROUP_STRESS_ROUNDS,
 * LIBC_TEST_GROUP_COUNTER_LOOPS, LIBC_TEST_GROUP_RACE_ROUNDS, and
 * LIBC_TEST_GROUP_CHURN_ROUNDS.
 */

struct stress_record {
	uint32_t sequence;
	uint32_t value;
	uint32_t result;
	uint32_t state;
	uint32_t checksum;
};

struct race_result {
	int success;
	int error;
	dev_t dev;
	ino_t ino;
	int value;
};

static gid_t gid_a;
static gid_t gid_b;

static void close_test_fd(int fd)
{
	if (fd >= 0) {
		(void)close(fd);
	}
}

static void unmap_test_memory(void *map, size_t size)
{
	if (map != MAP_FAILED) {
		(void)munmap(map, size);
	}
}

static void close_test_sem(sem_t *sem)
{
	if (sem != SEM_FAILED) {
		(void)sem_close(sem);
	}
}

static gid_t read_gid(const char *name, unsigned long fallback)
{
	const char *value = getenv(name);
	char *end;
	unsigned long parsed;

	if (!value || !*value) {
		return (gid_t)fallback;
	}
	errno = 0;
	parsed = strtoul(value, &end, 10);
	if (errno || *end || (gid_t)parsed != parsed) {
		TEST_LOG("invalid %s=%s; using fallback gid %lu\n", name, value, fallback);
		return (gid_t)fallback;
	}
	return (gid_t)parsed;
}

static int read_count(const char *name, int fallback, int maximum)
{
	const char *value = getenv(name);
	char *end;
	long parsed;

	if (!value || !*value) {
		return fallback;
	}
	errno = 0;
	parsed = strtol(value, &end, 10);
	if (errno || *end || parsed <= 0 || parsed > maximum) {
		(void)t_error("invalid %s=%s\n", name, value);
		return fallback;
	}
	return (int)parsed;
}

static int init_directory(const char *path, mode_t mode)
{
	int fd;
	int result = 0;
	struct stat st = {0};

	if (mkdir(path, mode) && errno != EEXIST) {
		(void)t_error("cannot create public directory %s: %s\n", path, strerror(errno));
		return -1;
	}
	fd = open(path, O_RDONLY | O_CLOEXEC);
	if (fd < 0) {
		(void)t_error("cannot open public directory %s: %s\n", path, strerror(errno));
		return -1;
	}
	if (fstat(fd, &st) || !S_ISDIR(st.st_mode)) {
		(void)t_error("public path %s is not a directory\n", path);
		(void)close(fd);
		return -1;
	}
	if (fchown(fd, 0, 0) || fchmod(fd, mode)) {
		(void)t_error("cannot initialize public directory %s: %s\n", path, strerror(errno));
		result = -1;
	}
	if (close(fd)) {
		(void)t_error("cannot close public directory %s: %s\n", path, strerror(errno));
		result = -1;
	}
	return result;
}

static int init_test_environment(void)
{
	if (geteuid()) {
		(void)t_error("root is required to initialize /dev/group/shm\n");
		return -1;
	}
	if (gid_a == gid_b) {
		(void)t_error("group stress test gids must be distinct: %u\n", (unsigned)gid_a);
		return -1;
	}
	return init_directory("/dev/group", 0755) || init_directory("/dev/group/shm", 01777);
}

static void make_name(char *buf, size_t size, const char *tag)
{
	(void)snprintf(buf, size, "/libctest-stress-%s-%d", tag, (int)getpid());
}

static void cleanup_object(const char *name, gid_t gid)
{
	int saved = errno;
	(void)group_shm_unlink(name, gid);
	errno = saved;
}

static int write_all(int fd, const void *data, size_t size)
{
	const unsigned char *p = data;
	while (size) {
		ssize_t n = write(fd, p, size);
		if (n < 0 && errno == EINTR) {
			continue;
		}
		if (n <= 0) {
			TEST_LOG("write failed: fd=%d remaining=%zu result=%zd\n", fd, size, n);
			return -1;
		}
		p += n;
		size -= n;
	}
	return 0;
}

static int read_all_timeout(int fd, void *data, size_t size)
{
	unsigned char *p = data;
	while (size) {
		struct pollfd pfd = { .fd = fd, .events = POLLIN };
		int ret;
		do {
			ret = poll(&pfd, 1, WAIT_TIMEOUT_MS);
		} while (ret < 0 && errno == EINTR);
		if (ret <= 0 || !(pfd.revents & (POLLIN | POLLHUP))) {
			TEST_LOG("poll failed, timed out, or returned unexpected events: "
				"fd=%d timeout=%d result=%d revents=%#x\n",
				fd, WAIT_TIMEOUT_MS, ret, (unsigned)pfd.revents);
			return -1;
		}
		ssize_t n = read(fd, p, size);
		if (n < 0 && errno == EINTR) {
			continue;
		}
		if (n <= 0) {
			TEST_LOG("read failed or reached EOF: fd=%d remaining=%zu result=%zd\n", fd, size, n);
			return -1;
		}
		p += n;
		size -= n;
	}
	return 0;
}

static int wait_process(pid_t pid)
{
	int status;
	pid_t ret;
	do {
		ret = waitpid(pid, &status, 0);
	} while (ret < 0 && errno == EINTR);
	if (ret != pid) {
		TEST_LOG("waitpid failed: child=%d result=%d\n", (int)pid, (int)ret);
		return -1;
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status)) {
		if (WIFSIGNALED(status)) {
			TEST_LOG("child %d terminated by signal %d\n", (int)pid, WTERMSIG(status));
		} else {
			TEST_LOG("child %d exited abnormally: status=%#x\n", (int)pid, status);
		}
		return -1;
	}
	return 0;
}

/* 只创建子进程；调用方在 pid == 0 分支执行子任务并用 _exit 退出。 */
static pid_t spawn_child(void)
{
	pid_t pid = fork();
	if (pid < 0) {
		(void)t_error("fork failed: %s\n", strerror(errno));
	}
	return pid;
}

static int sem_wait_seconds(sem_t *sem, int seconds)
{
	struct timespec deadline;
	(void)clock_gettime(CLOCK_REALTIME, &deadline);
	deadline.tv_sec += seconds;
	while (sem_timedwait(sem, &deadline)) {
		if (errno == EINTR) {
			continue;
		}
		/* The caller decides whether a timeout is expected. */
		TEST_LOG("sem_timedwait returned failure: timeout=%d seconds\n", seconds);
		return -1;
	}
	return 0;
}

static uint32_t checksum(const struct stress_record *record)
{
	return record->sequence ^ record->value ^ record->result ^ record->state;
}

static int count_fds(void)
{
	DIR *dir = opendir("/proc/self/fd");
	struct dirent *entry;
	int count = 0;
	if (!dir) {
		TEST_LOG("cannot count file descriptors: opendir /proc/self/fd failed\n");
		return -1;
	}
	while ((entry = readdir(dir))) {
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
			count++;
		}
	}
	(void)closedir(dir);
	return count;
}

static int count_group_maps(void)
{
	char line[512];
	FILE *file = fopen("/proc/self/maps", "r");
	int count = 0;
	if (!file) {
		TEST_LOG("cannot count group mappings: fopen /proc/self/maps failed\n");
		return -1;
	}
	while (fgets(line, sizeof line, file)) {
		if (strstr(line, "/dev/group/shm/")) {
			count++;
		}
	}
	(void)fclose(file);
	return count;
}

static long rss_pages(void)
{
	long total, resident;
	FILE *file = fopen("/proc/self/statm", "r");
	if (!file) {
		TEST_LOG("cannot sample RSS: fopen /proc/self/statm failed\n");
		return -1;
	}
	if (fscanf(file, "%ld %ld", &total, &resident) != 2) {
		TEST_LOG("cannot parse total and resident pages from /proc/self/statm\n");
		resident = -1;
	}
	(void)fclose(file);
	return resident;
}

static int count_tmp_files(gid_t gid)
{
	char path[128];
	DIR *dir;
	struct dirent *entry;
	int count = 0;

	(void)snprintf(path, sizeof path, "/dev/group/shm/%u", (unsigned)gid);
	dir = opendir(path);
	if (!dir) {
		if (errno == ENOENT) {
			return 0;
		}
		TEST_LOG("cannot scan temporary files: path=%s\n", path);
		return -1;
	}
	while ((entry = readdir(dir))) {
		if (!strncmp(entry->d_name, "tmp-", 4)) {
			count++;
		}
	}
	(void)closedir(dir);
	return count;
}

static int child_producer_consumer(struct stress_record *record, sem_t *ready, sem_t *done, int rounds)
{
	int i;

	for (i = 1; i <= rounds; i++) {
		if (sem_wait_seconds(ready, 30)) {
			TEST_LOG("producer-consumer ready wait failed: round=%d (exit 103)\n", i);
			return 103;
		}
		if (record->sequence != (uint32_t)i || record->state != 1 ||
		    record->checksum != checksum(record)) {
			TEST_LOG("producer-consumer sequence, state or checksum mismatch: round=%d (exit 104)\n", i);
			return 104;
		}
		record->result = record->value * 2;
		record->state = 2;
		record->checksum = checksum(record);
		if (sem_post(done)) {
			TEST_LOG("producer-consumer done post failed: round=%d (exit 105)\n", i);
			return 105;
		}
	}
	(void)munmap(record, sizeof *record);
	(void)sem_close(ready);
	(void)sem_close(done);
	return 0;
}

static int child_counter(uint64_t *counter, sem_t *mutex, int loops)
{
	int i;

	for (i = 0; i < loops; i++) {
		if (sem_wait_seconds(mutex, 30)) {
			TEST_LOG("counter mutex wait failed: round=%d (exit 113)\n", i);
			return 113;
		}
		(*counter)++;
		if (sem_post(mutex)) {
			TEST_LOG("counter mutex post failed: round=%d (exit 114)\n", i);
			return 114;
		}
	}
	(void)munmap(counter, sizeof *counter);
	(void)sem_close(mutex);
	return 0;
}

static int child_race(gid_t gid, const char *name, int start_fd, int result_fd, int semaphore)
{
	struct race_result result = {0};
	sem_t *sem;
	char token;
	int fd;

	if (read_all_timeout(start_fd, &token, 1)) {
		TEST_LOG("exclusive race worker did not receive start notification (exit 121)\n");
		return 121;
	}
	errno = 0;
	if (semaphore) {
		sem = group_sem_open(name, O_CREAT | O_EXCL, gid, 0600, 1);
		result.success = sem != SEM_FAILED;
		result.error = errno;
		if (result.success) {
			(void)sem_close(sem);
		}
	} else {
		fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid);
		result.success = fd >= 0;
		result.error = errno;
		close_test_fd(fd);
	}
	if (!result.success && result.error != EEXIST) {
		TEST_LOG("exclusive %s create failed: gid=%u name=%s result errno=%d, expected EEXIST\n",
			semaphore ? "SEM" : "SHM", (unsigned)gid, name, result.error);
	}
	if (write_all(result_fd, &result, sizeof result)) {
		TEST_LOG("exclusive race worker could not send result (exit 122)\n");
		return 122;
	}
	return 0;
}

static int child_nonexclusive(gid_t gid, const char *name, int start_fd, int result_fd, int semaphore, int initial)
{
	struct race_result result = {0};
	struct stat st;
	sem_t *sem;
	char token;
	int fd;

	if (read_all_timeout(start_fd, &token, 1)) {
		TEST_LOG("nonexclusive race worker did not receive start notification (exit 126)\n");
		return 126;
	}
	errno = 0;
	if (semaphore) {
		sem = group_sem_open(name, O_CREAT, gid, 0600, (unsigned)initial);
		result.success = sem != SEM_FAILED;
		result.error = errno;
		if (result.success) {
			if (sem_getvalue(sem, &result.value)) {
				TEST_LOG("nonexclusive SEM getvalue failed: gid=%u name=%s\n", (unsigned)gid, name);
				result.success = 0;
				result.error = errno;
			}
			(void)sem_close(sem);
		}
	} else {
		fd = group_shm_open(name, O_CREAT | O_RDWR, 0600, gid);
		result.success = fd >= 0;
		result.error = errno;
		if (fd >= 0) {
			if (fstat(fd, &st)) {
				TEST_LOG("nonexclusive SHM fstat failed: gid=%u name=%s fd=%d\n", (unsigned)gid, name, fd);
				result.success = 0;
				result.error = errno;
			} else {
				result.dev = st.st_dev;
				result.ino = st.st_ino;
			}
			(void)close(fd);
		}
	}
	if (!result.success) {
		TEST_LOG("nonexclusive %s open or inspection failed: gid=%u name=%s result errno=%d\n",
			semaphore ? "SEM" : "SHM", (unsigned)gid, name, result.error);
	}
	if (write_all(result_fd, &result, sizeof result)) {
		TEST_LOG("nonexclusive race worker could not send result (exit 127)\n");
		return 127;
	}
	return 0;
}

static int child_churn(gid_t gid, const char *shm_name, const char *sem_name, int rounds, int worker)
{
	sem_t *sem;
	int i, fd;

	for (i = 0; i < rounds; i++) {
		fd = group_shm_open(shm_name, O_CREAT | O_RDWR, 0600, gid);
		if (fd < 0) {
			TEST_LOG("churn SHM open failed: round=%d (exit 131)\n", i);
			return 131;
		}
		(void)close(fd);
		if ((i + worker) % 3 == 0 && group_shm_unlink(shm_name, gid) && errno != ENOENT) {
			TEST_LOG("churn SHM unlink failed: round=%d (exit 132)\n", i);
			return 132;
		}
		sem = group_sem_open(sem_name, O_CREAT, gid, 0600, 0);
		if (sem == SEM_FAILED) {
			TEST_LOG("churn SEM open failed: round=%d (exit 133)\n", i);
			return 133;
		}
		if (sem_close(sem)) {
			TEST_LOG("churn SEM close failed: round=%d (exit 134)\n", i);
			return 134;
		}
		if ((i + worker) % 4 == 0 && group_sem_unlink(sem_name, gid) && errno != ENOENT) {
			TEST_LOG("churn SEM unlink failed: round=%d (exit 135)\n", i);
			return 135;
		}
	}
	return 0;
}

static int sequential_shm_round(const char *shm_name, int i)
{
	uint64_t *value;
	int fd;

	fd = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (fd < 0 || ftruncate(fd, sizeof *value)) {
		(void)t_error("stress sequential SHM failed at %d: %s\n", i, strerror(errno));
		close_test_fd(fd);
		return -1;
	}
	value = mmap(0, sizeof *value, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (value == MAP_FAILED) {
		(void)t_error("stress sequential mmap failed at %d\n", i);
		(void)close(fd);
		return -1;
	}
	*value = (uint64_t)i;
	if (*value != (uint64_t)i) {
		(void)t_error("stress sequential SHM data mismatch at %d\n", i);
	}
	(void)munmap(value, sizeof *value);
	(void)close(fd);
	if (group_shm_unlink(shm_name, gid_a)) {
		TEST_LOG("stress sequential SHM unlink failed at round %d\n", i);
		return -1;
	}
	return 0;
}

static int sequential_sem_round(const char *sem_name, int i)
{
	sem_t *sem, *second;

	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, 1);
	second = group_sem_open(sem_name, 0, gid_a);
	if (sem == SEM_FAILED || second == SEM_FAILED || sem_wait_seconds(sem, 30) || sem_post(second)) {
		(void)t_error("stress sequential SEM failed at %d: %s\n", i, strerror(errno));
		close_test_sem(sem);
		close_test_sem(second);
		return -1;
	}
	(void)sem_close(sem);
	(void)sem_close(second);
	if (group_sem_unlink(sem_name, gid_a)) {
		TEST_LOG("stress sequential SEM unlink failed at round %d\n", i);
		return -1;
	}
	return 0;
}

static void check_resource_counts(int before_fds, int before_maps, long before_rss)
{
	int after_fds, after_maps;
	long after_rss;

	after_fds = count_fds();
	after_maps = count_group_maps();
	after_rss = rss_pages();
	if (before_fds >= 0 && after_fds != before_fds) {
		(void)t_error("stress fd count grew from %d to %d\n", before_fds, after_fds);
	}
	if (before_maps >= 0 && after_maps != before_maps) {
		(void)t_error("stress group map count grew from %d to %d\n", before_maps, after_maps);
	}
	if (before_rss >= 0 && after_rss > before_rss + 256) {
		(void)t_error("stress RSS grew from %ld to %ld pages\n", before_rss, after_rss);
	}
}

/**
 * @tc.name      : group_shm_sem_stress_0100
 * @tc.desc      : Test repeated sequential create, open, map, close, and unlink operations.
 * @tc.level     : Level 1
 */
static void sequential_churn(int rounds)
{
	char shm_name[96], sem_name[96];
	sem_t *sem;
	int before_fds, before_maps, i;
	long before_rss;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(shm_name, sizeof shm_name, "sequential-shm");
	make_name(sem_name, sizeof sem_name, "sequential-sem");
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	/* Warm up the process-local named semaphore table before sampling. */
	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, 1);
	if (sem != SEM_FAILED) {
		(void)sem_close(sem);
	} else {
		TEST_LOG("stress sequential SEM warmup failed: name=%s\n", sem_name);
	}
	cleanup_object(sem_name, gid_a);
	before_fds = count_fds();
	before_maps = count_group_maps();
	before_rss = rss_pages();
	for (i = 0; i < rounds; i++) {
		if (sequential_shm_round(shm_name, i) || sequential_sem_round(sem_name, i)) {
			break;
		}
	}
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	check_resource_counts(before_fds, before_maps, before_rss);
}

static void run_producer_consumer(struct stress_record *record, sem_t *ready, sem_t *done, int fd, int rounds)
{
	pid_t pid;
	int i;

	(void)memset(record, 0, sizeof *record);
	pid = spawn_child();
	if (!pid) {
		/* 直接使用 fork 继承的共享记录和信号量。 */
		(void)close(fd);
		_exit(child_producer_consumer(record, ready, done, rounds));
	}
	for (i = 1; pid > 0 && i <= rounds; i++) {
		record->sequence = (uint32_t)i;
		record->value = (uint32_t)(i * 7);
		record->result = 0;
		record->state = 1;
		record->checksum = checksum(record);
		if (sem_post(ready) || sem_wait_seconds(done, 30)) {
			(void)t_error("stress producer-consumer sync failed at %d\n", i);
			break;
		}
		if (record->result != record->value * 2 || record->state != 2 ||
		    record->checksum != checksum(record)) {
			(void)t_error("stress producer-consumer data mismatch at %d\n", i);
			break;
		}
	}
	if (pid > 0 && wait_process(pid)) {
		(void)t_error("stress producer-consumer child failed\n");
	}
	(void)munmap(record, sizeof *record);
}

/**
 * @tc.name      : group_shm_sem_stress_0200
 * @tc.desc      : Test long-running cross-process producer-consumer synchronization.
 * @tc.level     : Level 1
 */
static void producer_consumer(int rounds)
{
	struct stress_record *record;
	char shm_name[96], ready_name[96], done_name[96];
	sem_t *ready = SEM_FAILED, *done = SEM_FAILED;
	int fd = -1;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(shm_name, sizeof shm_name, "pc-shm");
	make_name(ready_name, sizeof ready_name, "pc-ready");
	make_name(done_name, sizeof done_name, "pc-done");
	cleanup_object(shm_name, gid_a);
	cleanup_object(ready_name, gid_a);
	cleanup_object(done_name, gid_a);
	fd = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	ready = group_sem_open(ready_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	done = group_sem_open(done_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	if (fd < 0 || ready == SEM_FAILED || done == SEM_FAILED || ftruncate(fd, sizeof *record)) {
		(void)t_error("stress producer-consumer setup failed: %s\n", strerror(errno));
		goto out;
	}
	record = mmap(0, sizeof *record, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (record == MAP_FAILED) {
		TEST_LOG("stress producer-consumer mmap failed: fd=%d\n", fd);
		goto out;
	}
	run_producer_consumer(record, ready, done, fd, rounds);
out:
	close_test_fd(fd);
	close_test_sem(ready);
	close_test_sem(done);
	cleanup_object(shm_name, gid_a);
	cleanup_object(ready_name, gid_a);
	cleanup_object(done_name, gid_a);
}

static void run_counter_workers(uint64_t *counters[2], sem_t *mutexes[2], int fds[2],
	pid_t *children, int workers, int loops)
{
	int group, i, total = workers * 2;

	for (group = 0; group < 2; group++) {
		for (i = 0; i < workers; i++) {
			children[group * workers + i] = spawn_child();
			if (!children[group * workers + i]) {
				/* 每个子进程只操作所分配 gid 的计数器和互斥信号量。 */
				(void)close(fds[0]);
				(void)close(fds[1]);
				_exit(child_counter(counters[group], mutexes[group], loops));
			}
		}
	}
	for (i = 0; i < total; i++) {
		if (children[i] <= 0 || wait_process(children[i])) {
			(void)t_error("stress counter worker %d failed\n", i);
		}
	}
}

static void check_counter_results(uint64_t *counters[2], gid_t gids[2], int workers, int loops)
{
	uint64_t expected = (uint64_t)workers * (uint64_t)loops;
	int group;

	for (group = 0; group < 2; group++) {
		if (*counters[group] != expected) {
			(void)t_error("stress gid %u counter is %llu, wanted %llu\n", (unsigned)gids[group],
				(unsigned long long)*counters[group], (unsigned long long)expected);
		}
	}
}

/**
 * @tc.name      : group_shm_sem_stress_0300
 * @tc.desc      : Test concurrent shared counters and isolation across two gids.
 * @tc.level     : Level 1
 */
static void counter_isolation(int workers, int loops)
{
	char shm_name[96], sem_name[96];
	uint64_t *counters[2] = {MAP_FAILED, MAP_FAILED};
	sem_t *mutexes[2] = {SEM_FAILED, SEM_FAILED};
	int fds[2] = {-1, -1}, group, total = workers * 2;
	pid_t *children;
	gid_t gids[2] = {gid_a, gid_b};

	if (prepare_case_directory(gid_a) || prepare_case_directory(gid_b)) {
		return;
	}
	children = calloc((size_t)total, sizeof *children);
	if (!children) {
		(void)t_error("stress counter children allocation failed\n");
		return;
	}
	make_name(shm_name, sizeof shm_name, "counter-shm");
	make_name(sem_name, sizeof sem_name, "counter-sem");
	for (group = 0; group < 2; group++) {
		cleanup_object(shm_name, gids[group]);
		cleanup_object(sem_name, gids[group]);
		fds[group] = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gids[group]);
		mutexes[group] = group_sem_open(sem_name, O_CREAT | O_EXCL, gids[group], 0600, 1);
		if (fds[group] < 0 || mutexes[group] == SEM_FAILED || ftruncate(fds[group], sizeof(uint64_t))) {
			(void)t_error("stress counter gid %u setup failed\n", (unsigned)gids[group]);
			goto out;
		}
		counters[group] = mmap(0, sizeof(uint64_t), PROT_READ | PROT_WRITE, MAP_SHARED, fds[group], 0);
		if (counters[group] == MAP_FAILED) {
			TEST_LOG("stress counter mmap failed: gid=%u fd=%d\n", (unsigned)gids[group], fds[group]);
			goto out;
		}
		*counters[group] = 0;
	}
	run_counter_workers(counters, mutexes, fds, children, workers, loops);
	check_counter_results(counters, gids, workers, loops);
out:
	for (group = 0; group < 2; group++) {
		unmap_test_memory(counters[group], sizeof(uint64_t));
		close_test_fd(fds[group]);
		close_test_sem(mutexes[group]);
		cleanup_object(shm_name, gids[group]);
		cleanup_object(sem_name, gids[group]);
	}
	free(children);
}

static void launch_exclusive_workers(pid_t *children, int workers, const char *round_name,
	int start_pipe[2], int result_pipe[2], int type)
{
	int i;

	for (i = 0; i < workers; i++) {
		children[i] = spawn_child();
		if (!children[i]) {
			(void)close(start_pipe[1]);
			(void)close(result_pipe[0]);
			_exit(child_race(gid_a, round_name, start_pipe[0], result_pipe[1], type));
		}
	}
	for (i = 0; i < workers; i++) {
		(void)write_all(start_pipe[1], "G", 1);
	}
	(void)close(start_pipe[0]);
	(void)close(start_pipe[1]);
	(void)close(result_pipe[1]);
}

static int collect_exclusive_results(int result_fd, int workers, int type, int round)
{
	struct race_result result;
	int i, successes;

	successes = 0;
	for (i = 0; i < workers; i++) {
		if (read_all_timeout(result_fd, &result, sizeof result)) {
			(void)t_error("stress race result timeout at type %d round %d\n", type, round);
			break;
		}
		if (result.success) {
			successes++;
		} else if (result.error != EEXIST) {
			(void)t_error("stress race loser errno %d at type %d round %d\n", result.error, type, round);
		}
	}
	return successes;
}

static int run_exclusive_round(pid_t *children, int workers, const char *name, int type, int round)
{
	char round_name[120];
	int i, start_pipe[2], result_pipe[2], tmp_before;
	int successes;

	(void)snprintf(round_name, sizeof round_name, "%s-%d-%d", name, type, round);
	cleanup_object(round_name, gid_a);
	tmp_before = count_tmp_files(gid_a);
	start_pipe[0] = start_pipe[1] = -1;
	result_pipe[0] = result_pipe[1] = -1;
	if (pipe(start_pipe) || pipe(result_pipe)) {
		(void)t_error("stress race pipe failed\n");
		close_test_fd(start_pipe[0]);
		close_test_fd(start_pipe[1]);
		close_test_fd(result_pipe[0]);
		close_test_fd(result_pipe[1]);
		return -1;
	}
	launch_exclusive_workers(children, workers, round_name, start_pipe, result_pipe, type);
	successes = collect_exclusive_results(result_pipe[0], workers, type, round);
	(void)close(result_pipe[0]);
	for (i = 0; i < workers; i++) {
		if (children[i] <= 0 || wait_process(children[i])) {
			(void)t_error("stress race worker failed at type %d round %d\n", type, round);
		}
	}
	if (successes != 1) {
		(void)t_error("stress race successes %d at type %d round %d\n", successes, type, round);
	}
	if (count_tmp_files(gid_a) != tmp_before) {
		(void)t_error("stress race left tmp files at type %d round %d\n", type, round);
	}
	cleanup_object(round_name, gid_a);
	return 0;
}

/**
 * @tc.name      : group_shm_sem_stress_0400
 * @tc.desc      : Test repeated concurrent O_CREAT and O_EXCL races.
 * @tc.level     : Level 1
 */
static void exclusive_races(int workers, int rounds)
{
	char name[96];
	pid_t *children;
	int type, round;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	children = calloc((size_t)workers, sizeof *children);
	if (!children) {
		TEST_LOG("stress exclusive race children allocation failed: workers=%d\n", workers);
		return;
	}
	make_name(name, sizeof name, "race");
	for (type = 0; type < 2; type++) {
		for (round = 0; round < rounds; round++) {
			if (run_exclusive_round(children, workers, name, type, round)) {
				goto out;
			}
		}
	}
out:
	free(children);
}

static void launch_nonexclusive_workers(pid_t *children, int workers, const char *round_name,
	int start_pipe[2], int result_pipe[2], int type)
{
	int i;

	for (i = 0; i < workers; i++) {
		children[i] = spawn_child();
		if (!children[i]) {
			(void)close(start_pipe[1]);
			(void)close(result_pipe[0]);
			_exit(child_nonexclusive(gid_a, round_name, start_pipe[0], result_pipe[1], type, i + 1));
		}
	}
	for (i = 0; i < workers; i++) {
		(void)write_all(start_pipe[1], "G", 1);
	}
	(void)close(start_pipe[0]);
	(void)close(start_pipe[1]);
	(void)close(result_pipe[1]);
}

static int collect_nonexclusive_results(int result_fd, int workers, int type, int round)
{
	struct race_result result;
	int i, expected_value;
	dev_t expected_dev;
	ino_t expected_ino;

	expected_value = -1;
	expected_dev = 0;
	expected_ino = 0;
	for (i = 0; i < workers; i++) {
		if (read_all_timeout(result_fd, &result, sizeof result)) {
			(void)t_error("stress nonexclusive result timeout at type %d round %d\n", type, round);
			break;
		}
		if (!result.success) {
			(void)t_error("stress nonexclusive open failed with errno %d at type %d round %d\n",
				result.error, type, round);
			continue;
		}
		if (type) {
			if (expected_value < 0) {
				expected_value = result.value;
			} else if (result.value != expected_value) {
				(void)t_error("stress nonexclusive SEM values differ at round %d\n", round);
			}
		} else {
			if (!expected_ino) {
				expected_dev = result.dev;
				expected_ino = result.ino;
			} else if (result.dev != expected_dev || result.ino != expected_ino) {
				(void)t_error("stress nonexclusive SHM objects differ at round %d\n", round);
			}
		}
	}
	return expected_value;
}

static int run_nonexclusive_round(pid_t *children, int workers, const char *name, int type, int round)
{
	char round_name[120];
	int i, start_pipe[2], result_pipe[2], tmp_before;
	int expected_value;

	(void)snprintf(round_name, sizeof round_name, "%s-%d-%d", name, type, round);
	cleanup_object(round_name, gid_a);
	tmp_before = count_tmp_files(gid_a);
	start_pipe[0] = start_pipe[1] = -1;
	result_pipe[0] = result_pipe[1] = -1;
	if (pipe(start_pipe) || pipe(result_pipe)) {
		(void)t_error("stress nonexclusive pipe failed\n");
		close_test_fd(start_pipe[0]);
		close_test_fd(start_pipe[1]);
		close_test_fd(result_pipe[0]);
		close_test_fd(result_pipe[1]);
		return -1;
	}
	launch_nonexclusive_workers(children, workers, round_name, start_pipe, result_pipe, type);
	expected_value = collect_nonexclusive_results(result_pipe[0], workers, type, round);
	(void)close(result_pipe[0]);
	for (i = 0; i < workers; i++) {
		if (children[i] <= 0 || wait_process(children[i])) {
			(void)t_error("stress nonexclusive worker failed at type %d round %d\n", type, round);
		}
	}
	if (type && (expected_value < 1 || expected_value > workers)) {
		(void)t_error("stress nonexclusive SEM winner value %d is invalid\n", expected_value);
	}
	if (count_tmp_files(gid_a) != tmp_before) {
		(void)t_error("stress nonexclusive left tmp files at type %d round %d\n", type, round);
	}
	cleanup_object(round_name, gid_a);
	return 0;
}

/**
 * @tc.name      : group_shm_sem_stress_0500
 * @tc.desc      : Test repeated concurrent nonexclusive creation and object convergence.
 * @tc.level     : Level 1
 */
static void nonexclusive_races(int workers, int rounds)
{
	char name[96];
	pid_t *children;
	int type, round;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	children = calloc((size_t)workers, sizeof *children);
	if (!children) {
		TEST_LOG("stress nonexclusive race children allocation failed: workers=%d\n", workers);
		return;
	}
	make_name(name, sizeof name, "nonexclusive");
	for (type = 0; type < 2; type++) {
		for (round = 0; round < rounds; round++) {
			if (run_nonexclusive_round(children, workers, name, type, round)) {
				goto out;
			}
		}
	}
out:
	free(children);
}

/**
 * @tc.name      : group_shm_sem_stress_0600
 * @tc.desc      : Test concurrent mixed open, close, and unlink churn.
 * @tc.level     : Level 1
 */
static void mixed_churn(int workers, int rounds)
{
	char shm_name[96], sem_name[96];
	pid_t *children;
	int i, tmp_before;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	children = calloc((size_t)workers, sizeof *children);
	if (!children) {
		TEST_LOG("stress mixed churn children allocation failed: workers=%d\n", workers);
		return;
	}
	make_name(shm_name, sizeof shm_name, "mixed-shm");
	make_name(sem_name, sizeof sem_name, "mixed-sem");
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	tmp_before = count_tmp_files(gid_a);
	for (i = 0; i < workers; i++) {
		children[i] = spawn_child();
		if (!children[i]) {
			_exit(child_churn(gid_a, shm_name, sem_name, rounds, i));
		}
	}
	for (i = 0; i < workers; i++) {
		if (children[i] <= 0 || wait_process(children[i])) {
			(void)t_error("mixed churn worker %d failed\n", i);
		}
	}
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	if (count_tmp_files(gid_a) != tmp_before) {
		(void)t_error("mixed churn left tmp files\n");
	}
	free(children);
}

int main(void)
{
	int rounds, counter_workers, counter_loops, race_workers, race_rounds;
	int churn_workers, churn_rounds, tmp_a_before, tmp_b_before;

	(void)signal(SIGPIPE, SIG_IGN);
	gid_a = read_gid("LIBC_TEST_GROUP_GID_A", TEST_GID_A_DEFAULT);
	gid_b = read_gid("LIBC_TEST_GROUP_GID_B", TEST_GID_B_DEFAULT);
	if (init_test_environment()) {
		return t_status;
	}
	tmp_a_before = count_tmp_files(gid_a);
	tmp_b_before = count_tmp_files(gid_b);
	if (tmp_a_before < 0 || tmp_b_before < 0) {
		(void)t_error("cannot inspect group stress test directories for temporary files: "
			"tmp_a_before=%d, tmp_b_before=%d\n", tmp_a_before, tmp_b_before);
		return t_status;
	}
	rounds = read_count("LIBC_TEST_GROUP_STRESS_ROUNDS", DEFAULT_STRESS_ROUNDS, 1000000);
	counter_workers = read_count("LIBC_TEST_GROUP_COUNTER_WORKERS", DEFAULT_COUNTER_WORKERS, 64);
	counter_loops = read_count("LIBC_TEST_GROUP_COUNTER_LOOPS", DEFAULT_COUNTER_LOOPS, 1000000);
	race_workers = read_count("LIBC_TEST_GROUP_RACE_WORKERS", DEFAULT_RACE_WORKERS, 64);
	race_rounds = read_count("LIBC_TEST_GROUP_RACE_ROUNDS", DEFAULT_RACE_ROUNDS, 10000);
	churn_workers = read_count("LIBC_TEST_GROUP_CHURN_WORKERS", DEFAULT_CHURN_WORKERS, 64);
	churn_rounds = read_count("LIBC_TEST_GROUP_CHURN_ROUNDS", DEFAULT_CHURN_ROUNDS, 1000000);

	sequential_churn(rounds);
	producer_consumer(rounds);
	counter_isolation(counter_workers, counter_loops);
	exclusive_races(race_workers, race_rounds);
	nonexclusive_races(race_workers, race_rounds);
	mixed_churn(churn_workers, churn_rounds);
	if (count_tmp_files(gid_a) != tmp_a_before) {
		(void)t_error("group stress tests left tmp files in gid %u\n", (unsigned)gid_a);
	}
	if (count_tmp_files(gid_b) != tmp_b_before) {
		(void)t_error("group stress tests left tmp files in gid %u\n", (unsigned)gid_b);
	}
	return t_status;
}
