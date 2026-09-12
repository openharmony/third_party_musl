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
#include <limits.h>
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
#define TEST_GID_DENY_DEFAULT 198403U
#define TEST_GID_ABSENT_DEFAULT 198404U
#define TEST_GID_NOTDIR_DEFAULT 198405U
#define CHILD_TIMEOUT_MS 5000
#define E2E_ROUNDS 100
#define RACE_WORKERS 16
#define TEST_MAGIC 0x4752534dU

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

#define CHECK(c, ...) do { \
	if (!(c)) { \
		(void)t_error(__VA_ARGS__); \
	} \
} while (0)

struct shared_record {
	uint32_t magic;
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
static gid_t gid_deny;
static gid_t gid_absent;
static gid_t gid_notdir;
static unsigned name_sequence;

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
	char *end;
	const char *value = getenv(name);
	unsigned long parsed;

	if (!value || !*value) {
		return (gid_t)fallback;
	}
	errno = 0;
	parsed = strtoul(value, &end, 10);
	if (errno || *end || (gid_t)parsed != parsed) {
		(void)t_error("invalid %s value: %s\n", name, value);
		return (gid_t)fallback;
	}
	return (gid_t)parsed;
}

static void init_gids(void)
{
	gid_a = read_gid("LIBC_TEST_GROUP_GID_A", TEST_GID_A_DEFAULT);
	gid_b = read_gid("LIBC_TEST_GROUP_GID_B", TEST_GID_B_DEFAULT);
	gid_deny = read_gid("LIBC_TEST_GROUP_GID_DENY", TEST_GID_DENY_DEFAULT);
	gid_absent = read_gid("LIBC_TEST_GROUP_GID_ABSENT", TEST_GID_ABSENT_DEFAULT);
	gid_notdir = read_gid("LIBC_TEST_GROUP_GID_NOTDIR", TEST_GID_NOTDIR_DEFAULT);
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

static int validate_fixture_gids(void)
{
	gid_t gids[] = {gid_a, gid_b, gid_deny, gid_absent, gid_notdir};
	size_t i, j;

	/* Exceptional fixtures must not overlap normal or boundary gids. */
	for (i = 0; i < sizeof gids / sizeof gids[0]; i++) {
		if (i >= 2 && (gids[i] == 0 || gids[i] == (gid_t)-1)) {
			(void)t_error("exceptional fixture gid %u overlaps a boundary gid\n", (unsigned)gids[i]);
			return -1;
		}
		for (j = 0; j < i; j++) {
			if (gids[i] == gids[j]) {
				(void)t_error("group test fixture gids must be distinct: %u\n", (unsigned)gids[i]);
				return -1;
			}
		}
	}
	return 0;
}

static int init_notdir_fixture(void)
{
	char path[128];
	struct stat st;
	int fd;

	(void)snprintf(path, sizeof path, "/dev/group/shm/%u", (unsigned)gid_notdir);
	fd = open(path, O_CREAT | O_EXCL | O_WRONLY | O_CLOEXEC, 0600);
	if (fd >= 0) {
		if (close(fd)) {
			(void)t_error("cannot close not-directory fixture %s: %s\n", path, strerror(errno));
			return -1;
		}
	} else if (errno != EEXIST) {
		(void)t_error("cannot create not-directory fixture %s: %s\n", path, strerror(errno));
		return -1;
	}
	if (lstat(path, &st)) {
		(void)t_error("cannot check not-directory fixture %s: %s\n", path, strerror(errno));
		return -1;
	}
	if (!S_ISREG(st.st_mode)) {
		(void)t_error("not-directory fixture must be a regular file: %s\n", path);
		return -1;
	}
	return 0;
}

static int init_test_environment(void)
{
	if (geteuid()) {
		(void)t_error("root is required to initialize /dev/group/shm\n");
		return -1;
	}
	if (validate_fixture_gids()) {
		return -1;
	}
	return init_directory("/dev/group", 0755) || init_directory("/dev/group/shm", 01777);
}

static void make_name(char *buf, size_t size, const char *tag)
{
	(void)snprintf(buf, size, "/libctest-%s-%d-%u", tag, (int)getpid(), name_sequence++);
}

static void make_path(char *buf, size_t size, gid_t gid, const char *name)
{
	while (*name == '/') {
		name++;
	}
	(void)snprintf(buf, size, "/dev/group/shm/%u/%s", (unsigned)gid, name);
}

static void cleanup_object(const char *name, gid_t gid)
{
	int saved = errno;
	(void)group_shm_unlink(name, gid);
	errno = saved;
}

static int count_tmp_files(gid_t gid)
{
	char path[128];
	struct dirent *entry;
	DIR *dir;
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

static uint32_t record_checksum(const struct shared_record *record)
{
	return record->magic ^ record->sequence ^ record->value ^
		record->result ^ record->state;
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

static void close_pipe_pair(int pipefd[2])
{
	close_test_fd(pipefd[0]);
	close_test_fd(pipefd[1]);
	pipefd[0] = pipefd[1] = -1;
}

static int read_all_timeout(int fd, void *data, size_t size, int timeout_ms)
{
	unsigned char *p = data;
	while (size) {
		struct pollfd pollfd = { .fd = fd, .events = POLLIN };
		int ret;
		do {
			ret = poll(&pollfd, 1, timeout_ms);
		} while (ret < 0 && errno == EINTR);
		if (ret <= 0 || !(pollfd.revents & (POLLIN | POLLHUP))) {
			TEST_LOG("poll failed, timed out, or returned unexpected events: "
				"fd=%d timeout=%d result=%d revents=%#x\n",
				fd, timeout_ms, ret, (unsigned)pollfd.revents);
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

static int wait_child(pid_t pid, const char *description)
{
	int status;
	pid_t ret;

	do {
		ret = waitpid(pid, &status, 0);
	} while (ret < 0 && errno == EINTR);
	if (ret != pid) {
		(void)t_error("%s: waitpid failed: %s\n", description, strerror(errno));
		return -1;
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status)) {
		if (WIFSIGNALED(status)) {
			(void)t_error("%s: child terminated by signal %d\n", description, WTERMSIG(status));
		} else {
			(void)t_error("%s: child status %#x\n", description, status);
		}
		return -1;
	}
	return 0;
}

static int sem_wait_seconds(sem_t *sem, int seconds)
{
	struct timespec deadline;
	(void)clock_gettime(CLOCK_REALTIME, &deadline);
	deadline.tv_sec += seconds;
	while (sem_timedwait(sem, &deadline) < 0) {
		if (errno == EINTR) {
			continue;
		}
		/* The caller decides whether a timeout is expected. */
		TEST_LOG("sem_timedwait returned failure: timeout=%d seconds\n", seconds);
		return -1;
	}
	return 0;
}

static int child_shm_transfer(struct shared_record *record, int ready_fd, int go_fd, int done_fd)
{
	char byte;

	/* 只有子进程自己的映射建立成功，才通知父进程可以开始写入。 */
	if (write_all(ready_fd, "R", 1)) {
		TEST_LOG("SHM exchange could not send ready notification (exit 103)\n");
		return 103;
	}
	/* 等待父进程写完数据并发送 G，不能在收到通知之前读取共享记录。 */
	if (read_all_timeout(go_fd, &byte, 1, CHILD_TIMEOUT_MS) || byte != 'G') {
		TEST_LOG("SHM exchange did not receive start notification (exit 104)\n");
		return 104;
	}
	if (record->magic != TEST_MAGIC || record->sequence != 1 || record->value != 123 ||
	    record->result != 0 || record->state != 1 ||
	    record->checksum != record_checksum(record)) {
		TEST_LOG("SHM exchange magic, value or checksum mismatch (exit 105)\n");
		return 105;
	}
	/* 子进程读到父进程的新数据后，通过自己的映射写回，再通知父进程读取。 */
	record->sequence++;
	record->value = 456;
	record->state = 2;
	record->checksum = record_checksum(record);
	if (write_all(done_fd, "D", 1)) {
		TEST_LOG("SHM exchange could not send completion notification (exit 106)\n");
		return 106;
	}
	return 0;
}

static int child_shm_exchange(gid_t gid, const char *name, int ready_fd, int go_fd, int done_fd)
{
	struct shared_record *record = MAP_FAILED;
	int fd = -1;
	int result = 0;
	char byte;

	/* 父进程在 fork 后创建、扩容并映射对象，再发送 O，避免子进程打开尚不存在的文件。 */
	if (read_all_timeout(go_fd, &byte, 1, CHILD_TIMEOUT_MS) || byte != 'O') {
		TEST_LOG("SHM exchange did not receive object-ready notification (exit 100)\n");
		result = 100;
		goto out;
	}
	/* 子进程通过相同 gid/name 独立打开并映射，不使用父进程继承的 fd 或映射。 */
	fd = group_shm_open(name, O_RDWR, 0, gid);
	if (fd < 0) {
		TEST_LOG("SHM exchange group_shm_open failed: gid=%u name=%s (exit 101)\n",
			(unsigned)gid, name);
		result = 101;
		goto out;
	}
	record = mmap(0, sizeof *record, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (record == MAP_FAILED) {
		TEST_LOG("SHM exchange mmap failed: fd=%d (exit 102)\n", fd);
		result = 102;
		goto out;
	}
	result = child_shm_transfer(record, ready_fd, go_fd, done_fd);
out:
	unmap_test_memory(record, sizeof *record);
	close_test_fd(fd);
	return result;
}

static int child_sem_waiter(sem_t *sem, int ready_fd, int done_fd)
{
	if (write_all(ready_fd, "R", 1)) {
		TEST_LOG("SEM waiter could not send ready notification (exit 112)\n");
		return 112;
	}
	if (sem_wait_seconds(sem, 5)) {
		TEST_LOG("SEM waiter failed to acquire semaphore (exit 113)\n");
		return 113;
	}
	if (write_all(done_fd, "D", 1)) {
		TEST_LOG("SEM waiter could not send completion notification (exit 114)\n");
		return 114;
	}
	if (sem_close(sem)) {
		TEST_LOG("SEM waiter sem_close failed (exit 115)\n");
		return 115;
	}
	return 0;
}

static int child_sem_holder(sem_t *sem, int acquired_fd)
{
	if (sem_wait_seconds(sem, 5)) {
		TEST_LOG("SEM holder failed to acquire semaphore (exit 122)\n");
		return 122;
	}
	if (write_all(acquired_fd, "A", 1)) {
		TEST_LOG("SEM holder could not send acquired notification (exit 123)\n");
		return 123;
	}
	/* Deliberately exit without sem_post or sem_close. */
	return 0;
}

static int child_shm_creator(gid_t gid, const char *name)
{
	struct shared_record *record;
	int fd;

	fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid);
	if (fd < 0 || ftruncate(fd, sizeof *record)) {
		TEST_LOG("SHM creator open or ftruncate failed (exit 131)\n");
		return 131;
	}
	record = mmap(0, sizeof *record, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (record == MAP_FAILED) {
		TEST_LOG("SHM creator mmap failed (exit 132)\n");
		return 132;
	}
	(void)memset(record, 0, sizeof *record);
	record->magic = TEST_MAGIC;
	record->value = 789;
	record->checksum = record_checksum(record);
	/* Process exit must release the mapping and fd but leave the name. */
	return 0;
}

static int child_e2e(struct shared_record *record, sem_t *ready, sem_t *done, int open_fd, int rounds)
{
	int i;

	if (write_all(open_fd, "R", 1)) {
		TEST_LOG("E2E could not send ready notification (exit 144)\n");
		return 144;
	}
	for (i = 1; i <= rounds; i++) {
		if (sem_wait_seconds(ready, 5)) {
			TEST_LOG("E2E ready wait failed: round=%d (exit 145)\n", i);
			return 145;
		}
		if (record->magic != TEST_MAGIC || record->sequence != (uint32_t)i ||
		    record->state != 1 || record->checksum != record_checksum(record)) {
			TEST_LOG("E2E magic, sequence, state or checksum mismatch: round=%d (exit 146)\n", i);
			return 146;
		}
		record->result = record->value * 2;
		record->state = 2;
		record->checksum = record_checksum(record);
		if (sem_post(done)) {
			TEST_LOG("E2E done post failed: round=%d (exit 147)\n", i);
			return 147;
		}
	}
	(void)munmap(record, sizeof *record);
	(void)sem_close(ready);
	(void)sem_close(done);
	return 0;
}

static int child_race(gid_t gid, const char *name, int start_fd, int result_fd, int semaphore)
{
	struct race_result result = {0};
	struct stat st;
	char token;
	int fd;
	sem_t *sem;

	if (read_all_timeout(start_fd, &token, 1, CHILD_TIMEOUT_MS)) {
		TEST_LOG("exclusive race worker did not receive start notification (exit 151)\n");
		return 151;
	}
	if (semaphore) {
		errno = 0;
		sem = group_sem_open(name, O_CREAT | O_EXCL, gid, 0600, 1);
		result.error = errno;
		result.success = sem != SEM_FAILED;
		if (result.success) {
			(void)sem_close(sem);
		}
	} else {
		errno = 0;
		fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid);
		result.error = errno;
		result.success = fd >= 0;
		if (fd >= 0) {
			if (!fstat(fd, &st)) {
				result.dev = st.st_dev;
				result.ino = st.st_ino;
			} else {
				TEST_LOG("exclusive race SHM fstat failed: gid=%u name=%s fd=%d\n",
					(unsigned)gid, name, fd);
			}
			(void)close(fd);
		}
	}
	if (!result.success && result.error != EEXIST) {
		TEST_LOG("exclusive %s create failed: gid=%u name=%s result errno=%d, expected EEXIST\n",
			semaphore ? "SEM" : "SHM", (unsigned)gid, name, result.error);
	}
	if (write_all(result_fd, &result, sizeof result)) {
		TEST_LOG("exclusive race worker could not send result (exit 152)\n");
		return 152;
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

/* Functional business scenarios. */

static int check_shm_basic_attributes(int fd, const char *name, struct stat *first_st)
{
	char path[PATH_MAX];
	struct stat path_st;

	/* fstat 通过已打开的 fd 获取对象属性，包括权限、设备号和 inode。 */
	if (fstat(fd, first_st)) {
		(void)t_error("SHM basic fstat failed: %s\n", strerror(errno));
		return -1;
	}
	/* 测试辅助函数：拼出预期的 /dev/group/shm/<gid>/<name> 路径。 */
	make_path(path, sizeof path, gid_a, name);
	/* stat 按路径获取属性；设备号和 inode 均一致，说明 fd 对应的正是预期路径下的对象。 */
	if (stat(path, &path_st) || path_st.st_dev != first_st->st_dev ||
	    path_st.st_ino != first_st->st_ino) {
		(void)t_error("SHM object was not created at %s\n", path);
		return -1;
	}
	/* CHECK 是测试断言宏：核对实际权限是否符合 mode 与 umask 的组合结果。 */
	CHECK((first_st->st_mode & 0777) == 0640,
		"SHM mode got %#o, wanted 0640\n", first_st->st_mode & 0777);
	return 0;
}

static void check_shm_basic_data(struct shared_record *record, struct shared_record *second)
{
	/* 两份 MAP_SHARED 映射均建立成功后，才通过第一份映射初始化并写入测试数据。 */
	/* memset 将整条共享记录清零，再写入用于验证共享读写的测试数据。 */
	(void)memset(record, 0, sizeof *record);
	record->magic = TEST_MAGIC;
	record->sequence = 1;
	record->value = 42;
	/* 测试辅助函数：根据记录字段计算校验和，用于检查另一份映射读到的数据是否完整。 */
	record->checksum = record_checksum(record);
	/* 正向验证：第一份映射写入 42 后，从已建立的第二份映射读取并校验。 */
	CHECK(second->value == 42 && second->checksum == record_checksum(second),
		"SHM reopened mapping has wrong data\n");
	/* 反向验证：第二份映射写入 84 后，从第一份映射读取并比较。 */
	second->value = 84;
	second->checksum = record_checksum(second);
	CHECK(record->value == 84, "SHM mapping update is not shared\n");
}

static void check_shm_basic_reopen(const char *name, struct shared_record *record, const struct stat *first_st)
{
	struct shared_record *second;
	struct stat second_st;
	int fd2;

	/* 再次打开同一 gid/name；对象已存在时 O_CREAT 不会重建对象，mode 0000 也不会修改已有权限。 */
	fd2 = group_shm_open(name, O_CREAT | O_RDWR, 0000, gid_a);
	if (fd2 < 0) {
		(void)t_error("SHM reopen failed: %s\n", strerror(errno));
		goto close_fd;
	}
	/* 再用 fstat 获取第二个 fd 的对象身份，验证重复打开指向同一对象，而非比较 fd 数值。 */
	if (fstat(fd2, &second_st)) {
		(void)t_error("SHM reopen fstat failed: %s\n", strerror(errno));
		goto close_fd;
	}
	CHECK(first_st->st_dev == second_st.st_dev && first_st->st_ino == second_st.st_ino,
		"SHM repeated opens do not identify the same object\n");
	/* 用第二个 fd 建立独立的虚拟地址映射；两份 MAP_SHARED 映射应访问同一后端数据。 */
	second = mmap(0, sizeof *second, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
	if (second == MAP_FAILED) {
		(void)t_error("SHM second mmap failed: %s\n", strerror(errno));
	} else {
		check_shm_basic_data(record, second);
		/* munmap 仅解除指定的进程内映射，不关闭 fd，也不删除共享内存名称。 */
		(void)munmap(second, sizeof *second);
	}

close_fd:
	close_test_fd(fd2);
}

/**
 * @tc.name      : group_shm_sem_0100
 * @tc.desc      : Test group shared memory creation, repeated opening, mapping, mode, and unlink.
 * @tc.level     : Level 0
 */
static void test_shm_basic(void)
{
	struct shared_record *record;
	struct stat first_st;
	char name[96];
	mode_t old_umask;
	int fd = -1;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	/* 测试辅助函数：生成包含进程号和序号的对象名，清理该 gid 下的同名残留。 */
	make_name(name, sizeof name, "gshm-basic");
	cleanup_object(name, gid_a);
	/* umask 设置创建权限的屏蔽位并返回旧值；本次预期权限为 0666 & ~0027 = 0640。 */
	old_umask = umask(0027);
	/* 排他创建可读写的共享内存对象；若名称已存在则失败，成功时返回文件描述符。 */
	fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666, gid_a);
	/* 无论创建是否成功，都立即恢复进程原有的 umask，避免影响后续测试。 */
	(void)umask(old_umask);
	if (fd < 0) {
		/* strerror 将 errno 转为错误说明；t_error 打印错误并标记测试失败。 */
		(void)t_error("SHM basic create failed: %s\n", strerror(errno));
		return;
	}
	if (check_shm_basic_attributes(fd, name, &first_st)) {
		goto out;
	}
	/* 新建对象长度为 0；ftruncate 将其扩展到一条记录的大小，供后续映射和访问。 */
	if (ftruncate(fd, sizeof *record)) {
		(void)t_error("SHM basic ftruncate failed: %s\n", strerror(errno));
		goto out;
	}
	/* mmap 从对象偏移 0 建立可读写的共享映射，地址由系统选择；MAP_SHARED 使修改可被其他共享映射看到。 */
	record = mmap(0, sizeof *record, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (record == MAP_FAILED) {
		(void)t_error("SHM basic mmap failed: %s\n", strerror(errno));
		goto out;
	}
	check_shm_basic_reopen(name, record, &first_st);
	/* 解除第一份映射；第二份映射及其 fd 已由辅助函数释放。 */
	(void)munmap(record, sizeof *record);
out:
	/* close 释放创建对象的 fd；仅关闭 fd 不会删除命名对象，也不能代替 munmap。 */
	close_test_fd(fd);
	/* unlink 删除该 gid 下的对象名称；底层对象在最后的 fd 和映射引用释放后才可回收。 */
	CHECK(!group_shm_unlink(name, gid_a), "SHM basic unlink failed: %s\n", strerror(errno));
}

static void parent_shm_transfer(struct shared_record *record, int ready_fd, int go_fd, int done_fd)
{
	char byte;

	/* 通知子进程打开同一 gid/name，并等待其独立 mmap 成功。此时尚未写入测试数据。 */
	if (write_all(go_fd, "O", 1)) {
		(void)t_error("SHM object-ready pipe failed\n");
		return;
	}
	if (read_all_timeout(ready_fd, &byte, 1, CHILD_TIMEOUT_MS) || byte != 'R') {
		(void)t_error("SHM child did not become ready\n");
		return;
	}
	/* 第三步：双方映射均就绪后，父进程写入 123，再通知子进程读取并校验。 */
	(void)memset(record, 0, sizeof *record);
	record->magic = TEST_MAGIC;
	record->sequence = 1;
	record->value = 123;
	record->state = 1;
	record->checksum = record_checksum(record);
	if (write_all(go_fd, "G", 1)) {
		(void)t_error("SHM GO pipe failed\n");
		return;
	}
	/* 第四步：等待子进程写回 456 并发送 D，父进程才读取自己的映射并校验。 */
	if (read_all_timeout(done_fd, &byte, 1, CHILD_TIMEOUT_MS) || byte != 'D') {
		(void)t_error("SHM child did not complete\n");
		return;
	}
	CHECK(record->value == 456 && record->sequence == 2 && record->state == 2 &&
		record->checksum == record_checksum(record), "SHM child response is invalid\n");
}

static void parent_shm_exchange(const char *name, int ready_fd, int go_fd, int done_fd,
	int *fd, struct shared_record **record)
{
	/* 第二步：父进程通过 group 接口创建、扩容并建立自己的 MAP_SHARED 映射。 */
	*fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (*fd < 0 || ftruncate(*fd, sizeof **record)) {
		(void)t_error("SHM process setup failed: %s\n", strerror(errno));
		return;
	}
	*record = mmap(0, sizeof **record, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
	if (*record == MAP_FAILED) {
		(void)t_error("SHM process mmap failed: %s\n", strerror(errno));
		return;
	}
	parent_shm_transfer(*record, ready_fd, go_fd, done_fd);
}

/**
 * @tc.name      : group_shm_sem_0200
 * @tc.desc      : Test bidirectional group shared memory access between processes with the same gid.
 * @tc.level     : Level 0
 */
static void test_shm_cross_process(void)
{
	struct shared_record *record = MAP_FAILED;
	char name[96];
	int ready_pipe[2] = {-1, -1}, go_pipe[2] = {-1, -1};
	int done_pipe[2] = {-1, -1}, fd = -1;
	pid_t pid = -1;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(name, sizeof name, "gshm-process");
	cleanup_object(name, gid_a);
	/* fork 前只准备对象名和通知管道，不打开或映射被测共享内存。 */
	if (pipe(ready_pipe) || pipe(go_pipe) || pipe(done_pipe)) {
		(void)t_error("SHM process pipe failed: %s\n", strerror(errno));
		goto out;
	}
	/* 第一步：先 fork，父子进程之后分别获取自己的 fd 和映射。 */
	pid = spawn_child();
	if (!pid) {
		(void)close(ready_pipe[0]);
		(void)close(go_pipe[1]);
		(void)close(done_pipe[0]);
		_exit(child_shm_exchange(gid_a, name, ready_pipe[1], go_pipe[0], done_pipe[1]));
	}
	if (pid < 0) {
		goto out;
	}
	(void)close(ready_pipe[1]);
	ready_pipe[1] = -1;
	(void)close(go_pipe[0]);
	go_pipe[0] = -1;
	(void)close(done_pipe[1]);
	done_pipe[1] = -1;
	parent_shm_exchange(name, ready_pipe[0], go_pipe[1], done_pipe[0], &fd, &record);
out:
	/* 失败时也关闭通知管道，使等待中的子进程收到 EOF，然后回收子进程和各自资源。 */
	close_pipe_pair(ready_pipe);
	close_pipe_pair(go_pipe);
	close_pipe_pair(done_pipe);
	if (pid > 0) {
		(void)wait_child(pid, "SHM cross-process");
	}
	unmap_test_memory(record, sizeof *record);
	close_test_fd(fd);
	cleanup_object(name, gid_a);
}

/**
 * @tc.name      : group_shm_sem_0300
 * @tc.desc      : Test group shared memory isolation for the same name under different gids.
 * @tc.level     : Level 0
 */
static void test_shm_gid_isolation(void)
{
	uint32_t *a = MAP_FAILED, *b = MAP_FAILED;
	char name[96];
	int fd_a = -1, fd_b = -1, reopen = -1;

	if (prepare_case_directory(gid_a) || prepare_case_directory(gid_b)) {
		return;
	}
	make_name(name, sizeof name, "gshm-gid");
	cleanup_object(name, gid_a);
	cleanup_object(name, gid_b);
	fd_a = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	fd_b = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_b);
	if (fd_a < 0 || fd_b < 0 || ftruncate(fd_a, sizeof *a) || ftruncate(fd_b, sizeof *b)) {
		(void)t_error("SHM gid isolation setup failed: %s\n", strerror(errno));
		goto out;
	}
	a = mmap(0, sizeof *a, PROT_READ | PROT_WRITE, MAP_SHARED, fd_a, 0);
	b = mmap(0, sizeof *b, PROT_READ | PROT_WRITE, MAP_SHARED, fd_b, 0);
	if (a == MAP_FAILED || b == MAP_FAILED) {
		(void)t_error("SHM gid isolation mmap failed: %s\n", strerror(errno));
		goto out;
	}
	*a = 0xaaaaaaaaU;
	*b = 0xbbbbbbbbU;
	CHECK(*a == 0xaaaaaaaaU && *b == 0xbbbbbbbbU, "SHM gid data crossed boundaries\n");
	CHECK(!group_shm_unlink(name, gid_a), "SHM gid A unlink failed\n");
	reopen = group_shm_open(name, O_RDWR, 0, gid_b);
	CHECK(reopen >= 0, "SHM gid B was affected by gid A unlink: %s\n", strerror(errno));
out:
	unmap_test_memory(a, sizeof *a);
	unmap_test_memory(b, sizeof *b);
	close_test_fd(reopen);
	close_test_fd(fd_a);
	close_test_fd(fd_b);
	cleanup_object(name, gid_a);
	cleanup_object(name, gid_b);
}

static void check_unlinked_shm_access(const char *name, uint32_t *old_map)
{
	int missing;

	*old_map = 11;
	CHECK(!group_shm_unlink(name, gid_a), "SHM unlink failed: %s\n", strerror(errno));
	errno = 0;
	missing = group_shm_open(name, O_RDWR, 0, gid_a);
	CHECK(missing == -1 && errno == ENOENT, "SHM open after unlink got %d/%d\n", missing, errno);
	*old_map = 22;
	CHECK(*old_map == 22, "SHM old mapping stopped working after unlink\n");
}

/**
 * @tc.name      : group_shm_sem_0400
 * @tc.desc      : Test group shared memory lifetime and recreation after unlink.
 * @tc.level     : Level 0
 */
static void test_shm_unlink_lifetime(void)
{
	uint32_t *old_map = MAP_FAILED, *new_map = MAP_FAILED;
	struct stat old_st, new_st;
	char name[96];
	int old_fd = -1, new_fd = -1;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(name, sizeof name, "gshm-unlink");
	cleanup_object(name, gid_a);
	old_fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (old_fd < 0 || ftruncate(old_fd, sizeof *old_map) || fstat(old_fd, &old_st)) {
		(void)t_error("SHM unlink setup failed: %s\n", strerror(errno));
		goto out;
	}
	old_map = mmap(0, sizeof *old_map, PROT_READ | PROT_WRITE, MAP_SHARED, old_fd, 0);
	if (old_map == MAP_FAILED) {
		(void)t_error("SHM old-object mmap failed: %s\n", strerror(errno));
		goto out;
	}
	check_unlinked_shm_access(name, old_map);
	new_fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (new_fd < 0 || ftruncate(new_fd, sizeof *new_map) || fstat(new_fd, &new_st)) {
		(void)t_error("SHM recreate failed: %s\n", strerror(errno));
		goto out;
	}
	new_map = mmap(0, sizeof *new_map, PROT_READ | PROT_WRITE, MAP_SHARED, new_fd, 0);
	if (new_map == MAP_FAILED) {
		(void)t_error("SHM recreated-object mmap failed: %s\n", strerror(errno));
		goto out;
	}
	*new_map = 33;
	CHECK(*old_map == 22 && *new_map == 33, "SHM old and recreated objects are not isolated\n");
	CHECK(old_st.st_dev != new_st.st_dev || old_st.st_ino != new_st.st_ino,
		"SHM recreated object reused the live old object\n");
out:
	unmap_test_memory(old_map, sizeof *old_map);
	unmap_test_memory(new_map, sizeof *new_map);
	close_test_fd(old_fd);
	close_test_fd(new_fd);
	cleanup_object(name, gid_a);
}

/**
 * @tc.name      : group_shm_sem_0500
 * @tc.desc      : Test read-only opening and O_TRUNC behavior for group shared memory.
 * @tc.level     : Level 0
 */
static void test_shm_truncate_and_readonly(void)
{
	char name[96];
	struct stat st;
	uint32_t *map;
	int fd = -1, ro = -1, trunc_fd = -1;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(name, sizeof name, "gshm-flags");
	cleanup_object(name, gid_a);
	fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (fd < 0 || ftruncate(fd, sizeof *map)) {
		(void)t_error("SHM flag-test setup failed: %s\n", strerror(errno));
		goto out;
	}
	map = mmap(0, sizeof *map, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		(void)t_error("SHM flag-test mmap failed: %s\n", strerror(errno));
		goto out;
	}
	*map = 0x12345678U;
	(void)munmap(map, sizeof *map);
	(void)close(fd);
	fd = -1;
	ro = group_shm_open(name, O_RDONLY, 0, gid_a);
	if (ro < 0) {
		(void)t_error("SHM readonly open failed: %s\n", strerror(errno));
		goto out;
	}
	map = mmap(0, sizeof *map, PROT_READ, MAP_SHARED, ro, 0);
	if (map == MAP_FAILED) {
		(void)t_error("SHM readonly mmap failed: %s\n", strerror(errno));
	} else {
		CHECK(*map == 0x12345678U, "SHM readonly data mismatch\n");
		(void)munmap(map, sizeof *map);
	}
	(void)close(ro);
	ro = -1;
	trunc_fd = group_shm_open(name, O_RDWR | O_TRUNC, 0, gid_a);
	CHECK(trunc_fd >= 0 && !fstat(trunc_fd, &st) && st.st_size == 0,
		"SHM O_TRUNC did not reset size\n");
out:
	close_test_fd(fd);
	close_test_fd(ro);
	close_test_fd(trunc_fd);
	cleanup_object(name, gid_a);
}

static void check_sem_basic_operations(sem_t *sem, sem_t *second, sem_t *third)
{
	int value = -1;

	CHECK(!sem_getvalue(sem, &value) && value == 1, "SEM initial value is %d, wanted 1\n", value);
	CHECK(!sem_wait(second), "SEM wait failed: %s\n", strerror(errno));
	errno = 0;
	CHECK(sem_trywait(third) == -1 && errno == EAGAIN,
		"SEM trywait got errno %d, wanted EAGAIN\n", errno);
	CHECK(!sem_post(sem), "SEM post failed: %s\n", strerror(errno));
}

static void check_sem_basic_references(sem_t *sem, sem_t *second, sem_t *third)
{
	check_sem_basic_operations(sem, second, third);
	if (sem != SEM_FAILED) {
		CHECK(!sem_close(sem), "first SEM close failed\n");
	}
	CHECK(!sem_wait(second) && !sem_post(third),
		"remaining SEM references stopped working after first close\n");
	if (second != SEM_FAILED) {
		CHECK(!sem_close(second), "second SEM close failed\n");
	}
	CHECK(!sem_wait(third) && !sem_post(third),
		"last SEM reference stopped working after second close\n");
	if (third != SEM_FAILED) {
		CHECK(!sem_close(third), "third SEM close failed\n");
	}
}

/**
 * @tc.name      : group_shm_sem_0600
 * @tc.desc      : Test group semaphore creation, repeated opening, operations, mode, and close references.
 * @tc.level     : Level 0
 */
static void test_sem_basic(void)
{
	char name[96];
	sem_t *sem = SEM_FAILED, *second = SEM_FAILED, *third = SEM_FAILED;
	int tmp_before;
	mode_t old_umask;
	char path[PATH_MAX];
	struct stat st;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(name, sizeof name, "gsem-basic");
	cleanup_object(name, gid_a);
	tmp_before = count_tmp_files(gid_a);
	old_umask = umask(0027);
	sem = group_sem_open(name, O_CREAT | O_EXCL, gid_a, 0777, 1);
	(void)umask(old_umask);
	if (sem == SEM_FAILED) {
		(void)t_error("SEM basic create failed: %s\n", strerror(errno));
		return;
	}
	make_path(path, sizeof path, gid_a, name);
	if (stat(path, &st)) {
		(void)t_error("SEM backing path stat failed: %s\n", strerror(errno));
		goto close_all;
	}
	CHECK((st.st_mode & 0777) == 0640,
		"SEM mode got %#o, wanted 0640\n", st.st_mode & 0777);
	CHECK(count_tmp_files(gid_a) == tmp_before, "SEM basic create left tmp files\n");
	second = group_sem_open(name, 0, gid_a);
	third = group_sem_open(name, O_CREAT, gid_a, 0000, SEM_VALUE_MAX + 1U);
	CHECK(second == sem && third == sem, "SEM repeated open did not reuse the pointer\n");
	if (second == SEM_FAILED || third == SEM_FAILED) {
		goto close_all;
	}
	check_sem_basic_references(sem, second, third);
	goto unlink_name;
close_all:
	close_test_sem(sem);
	close_test_sem(second);
	close_test_sem(third);
unlink_name:
	CHECK(!group_sem_unlink(name, gid_a), "SEM basic unlink failed: %s\n", strerror(errno));
}

/**
 * @tc.name      : group_shm_sem_0700
 * @tc.desc      : Test wait and post synchronization between processes with the same gid.
 * @tc.level     : Level 0
 */
static void test_sem_cross_process(void)
{
	char name[96], byte;
	int ready_pipe[2] = {-1, -1}, done_pipe[2] = {-1, -1};
	sem_t *sem = SEM_FAILED;
	pid_t pid;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(name, sizeof name, "gsem-process");
	cleanup_object(name, gid_a);
	sem = group_sem_open(name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	if (sem == SEM_FAILED) {
		(void)t_error("SEM process setup failed: %s\n", strerror(errno));
		goto out;
	}
	if (pipe(ready_pipe) || pipe(done_pipe)) {
		(void)t_error("SEM process setup failed: %s\n", strerror(errno));
		close_pipe_pair(ready_pipe);
		close_pipe_pair(done_pipe);
		goto out;
	}
	pid = spawn_child();
	if (!pid) {
		/* 父子进程通过 fork 继承的同一个信号量完成 wait/post。 */
		(void)close(ready_pipe[0]);
		(void)close(done_pipe[0]);
		_exit(child_sem_waiter(sem, ready_pipe[1], done_pipe[1]));
	}
	(void)close(ready_pipe[1]);
	(void)close(done_pipe[1]);
	if (pid > 0) {
		CHECK(!read_all_timeout(ready_pipe[0], &byte, 1, CHILD_TIMEOUT_MS),
			"SEM waiter did not become ready\n");
		CHECK(!sem_post(sem), "SEM parent post failed: %s\n", strerror(errno));
		CHECK(!read_all_timeout(done_pipe[0], &byte, 1, CHILD_TIMEOUT_MS),
			"SEM waiter was not released\n");
		(void)wait_child(pid, "SEM cross-process");
	}
	(void)close(ready_pipe[0]);
	(void)close(done_pipe[0]);
out:
	close_test_sem(sem);
	cleanup_object(name, gid_a);
}

/**
 * @tc.name      : group_shm_sem_0800
 * @tc.desc      : Test group semaphore isolation for the same name under different gids.
 * @tc.level     : Level 0
 */
static void test_sem_gid_isolation(void)
{
	char name[96];
	sem_t *a = SEM_FAILED, *b = SEM_FAILED;

	if (prepare_case_directory(gid_a) || prepare_case_directory(gid_b)) {
		return;
	}
	make_name(name, sizeof name, "gsem-gid");
	cleanup_object(name, gid_a);
	cleanup_object(name, gid_b);
	a = group_sem_open(name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	b = group_sem_open(name, O_CREAT | O_EXCL, gid_b, 0600, 1);
	if (a == SEM_FAILED || b == SEM_FAILED) {
		(void)t_error("SEM gid isolation setup failed: %s\n", strerror(errno));
		goto out;
	}
	errno = 0;
	CHECK(sem_trywait(a) == -1 && errno == EAGAIN, "SEM gid A was unexpectedly available\n");
	CHECK(!sem_trywait(b), "SEM gid B was unexpectedly unavailable: %s\n", strerror(errno));
	CHECK(!sem_post(b), "SEM gid B post failed\n");
	errno = 0;
	CHECK(sem_trywait(a) == -1 && errno == EAGAIN, "SEM gid B post changed gid A\n");
	CHECK(!group_sem_unlink(name, gid_a), "SEM gid A unlink failed\n");
	sem_t *reopen = group_sem_open(name, 0, gid_b);
	CHECK(reopen == b, "SEM gid B was affected by gid A unlink\n");
	close_test_sem(reopen);
out:
	close_test_sem(a);
	close_test_sem(b);
	cleanup_object(name, gid_a);
	cleanup_object(name, gid_b);
}

/**
 * @tc.name      : group_shm_sem_0900
 * @tc.desc      : Test group semaphore handle lifetime and recreation after unlink.
 * @tc.level     : Level 0
 */
static void test_sem_unlink_lifetime(void)
{
	char name[96];
	sem_t *old_sem = SEM_FAILED, *new_sem = SEM_FAILED, *missing;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(name, sizeof name, "gsem-unlink");
	cleanup_object(name, gid_a);
	old_sem = group_sem_open(name, O_CREAT | O_EXCL, gid_a, 0600, 1);
	if (old_sem == SEM_FAILED) {
		(void)t_error("SEM unlink setup failed: %s\n", strerror(errno));
		goto out;
	}
	CHECK(!group_sem_unlink(name, gid_a), "SEM unlink failed: %s\n", strerror(errno));
	errno = 0;
	missing = group_sem_open(name, 0, gid_a);
	CHECK(missing == SEM_FAILED && errno == ENOENT,
		"SEM open after unlink got %p/%d\n", missing, errno);
	CHECK(!sem_wait(old_sem), "SEM old handle failed after unlink\n");
	new_sem = group_sem_open(name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	if (new_sem == SEM_FAILED) {
		(void)t_error("SEM recreate failed: %s\n", strerror(errno));
		goto out;
	}
	errno = 0;
	CHECK(sem_trywait(new_sem) == -1 && errno == EAGAIN,
		"SEM recreated object has wrong value\n");
	CHECK(!sem_post(old_sem) && !sem_trywait(old_sem),
		"SEM old and recreated objects are not independently usable\n");
out:
	close_test_sem(old_sem);
	close_test_sem(new_sem);
	cleanup_object(name, gid_a);
}

/**
 * @tc.name      : group_shm_sem_1000
 * @tc.desc      : Test valid zero and SEM_VALUE_MAX group semaphore initial values.
 * @tc.level     : Level 0
 */
static void test_sem_value_boundaries(void)
{
	unsigned values[] = {0, SEM_VALUE_MAX};
	char name[96];
	sem_t *sem;
	int current = -1, i;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	for (i = 0; i < 2; i++) {
		make_name(name, sizeof name, i ? "gsem-max" : "gsem-zero");
		cleanup_object(name, gid_a);
		sem = group_sem_open(name, O_CREAT | O_EXCL, gid_a, 0600, values[i]);
		if (sem == SEM_FAILED) {
			(void)t_error("SEM boundary value %u create failed: %s\n", values[i], strerror(errno));
			continue;
		}
		CHECK(!sem_getvalue(sem, &current) && current == (int)values[i],
			"SEM boundary value got %d, wanted %u\n", current, values[i]);
		(void)sem_close(sem);
		cleanup_object(name, gid_a);
	}
}

static void parent_e2e_rounds(struct shared_record *record, sem_t *ready, sem_t *done)
{
	int i;

	for (i = 1; i <= E2E_ROUNDS; i++) {
		record->magic = TEST_MAGIC;
		record->sequence = (uint32_t)i;
		record->value = (uint32_t)(i * 3);
		record->result = 0;
		record->state = 1;
		record->checksum = record_checksum(record);
		if (sem_post(ready) || sem_wait_seconds(done, 5)) {
			(void)t_error("E2E synchronization failed at round %d: %s\n", i, strerror(errno));
			break;
		}
		CHECK(record->sequence == (uint32_t)i && record->result == record->value * 2 &&
			record->state == 2 && record->checksum == record_checksum(record),
			"E2E response mismatch at round %d\n", i);
	}
}

static void run_e2e_exchange(struct shared_record *record, sem_t *ready, sem_t *done, int fd, int open_pipe[2])
{
	char byte;
	pid_t pid;

	(void)memset(record, 0, sizeof *record);
	pid = spawn_child();
	if (!pid) {
		/* 子进程继承共享记录和两个信号量，直接参与生产消费。 */
		(void)close(open_pipe[0]);
		(void)close(fd);
		_exit(child_e2e(record, ready, done, open_pipe[1], E2E_ROUNDS));
	}
	(void)close(open_pipe[1]);
	if (pid > 0 && !read_all_timeout(open_pipe[0], &byte, 1, CHILD_TIMEOUT_MS)) {
		parent_e2e_rounds(record, ready, done);
		(void)wait_child(pid, "group shm/sem E2E");
	} else if (pid > 0) {
		(void)t_error("E2E child did not open the objects\n");
		(void)kill(pid, SIGKILL);
		(void)waitpid(pid, 0, 0);
	}
	(void)close(open_pipe[0]);
	(void)munmap(record, sizeof *record);
}

/**
 * @tc.name      : group_shm_sem_1100
 * @tc.desc      : Test group shared memory and semaphores in a cross-process producer-consumer workflow.
 * @tc.level     : Level 0
 */
static void test_e2e(void)
{
	struct shared_record *record;
	char shm_name[96], ready_name[96], done_name[96];
	int open_pipe[2], fd = -1;
	sem_t *ready = SEM_FAILED, *done = SEM_FAILED;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	make_name(shm_name, sizeof shm_name, "gshm-e2e");
	make_name(ready_name, sizeof ready_name, "gsem-ready");
	make_name(done_name, sizeof done_name, "gsem-done");
	cleanup_object(shm_name, gid_a);
	cleanup_object(ready_name, gid_a);
	cleanup_object(done_name, gid_a);
	fd = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	ready = group_sem_open(ready_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	done = group_sem_open(done_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	if (fd < 0 || ready == SEM_FAILED || done == SEM_FAILED ||
	    ftruncate(fd, sizeof *record) || pipe(open_pipe)) {
		(void)t_error("E2E setup failed: %s\n", strerror(errno));
		goto out;
	}
	record = mmap(0, sizeof *record, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (record == MAP_FAILED) {
		(void)t_error("E2E mmap failed: %s\n", strerror(errno));
		goto out;
	}
	run_e2e_exchange(record, ready, done, fd, open_pipe);
out:
	close_test_fd(fd);
	close_test_sem(ready);
	close_test_sem(done);
	cleanup_object(shm_name, gid_a);
	cleanup_object(ready_name, gid_a);
	cleanup_object(done_name, gid_a);
}

/* Invalid input, boundary, and exceptional business scenarios. */

static void check_invalid_names(void)
{
	static const char *invalid[] = {"", "/", "////", ".", "/.", "..", "///..",
		"a/b", "/a/b", "a/", "../a"};
	sem_t *sem;
	int fd, i;

	for (i = 0; i < (int)(sizeof invalid / sizeof invalid[0]); i++) {
		errno = 0;
		fd = group_shm_open(invalid[i], O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
		CHECK(fd == -1 && errno == EINVAL, "invalid SHM name '%s' got %d/%d\n", invalid[i], fd, errno);
		close_test_fd(fd);
		errno = 0;
		CHECK(group_shm_unlink(invalid[i], gid_a) == -1 && errno == EINVAL,
			"invalid SHM unlink name '%s' got errno %d\n", invalid[i], errno);
		errno = 0;
		sem = group_sem_open(invalid[i], O_CREAT | O_EXCL, gid_a, 0600, 0);
		CHECK(sem == SEM_FAILED && errno == EINVAL,
			"invalid SEM name '%s' got %p/%d\n", invalid[i], sem, errno);
		close_test_sem(sem);
		errno = 0;
		CHECK(group_sem_unlink(invalid[i], gid_a) == -1 && errno == EINVAL,
			"invalid SEM unlink name '%s' got errno %d\n", invalid[i], errno);
	}
}

static void check_name_length_boundaries(void)
{
	char valid[NAME_MAX + 1], below_max[NAME_MAX];
	sem_t *sem;
	int fd;

	(void)memset(valid, 'n', NAME_MAX);
	valid[NAME_MAX] = 0;
	(void)memset(below_max, 'b', NAME_MAX - 1);
	below_max[NAME_MAX - 1] = 0;
	cleanup_object(below_max, gid_a);
	fd = group_shm_open(below_max, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	CHECK(fd >= 0, "NAME_MAX-1 SHM name failed: %s\n", strerror(errno));
	close_test_fd(fd);
	cleanup_object(below_max, gid_a);
	sem = group_sem_open(below_max, O_CREAT | O_EXCL, gid_a, 0600, 0);
	CHECK(sem != SEM_FAILED, "NAME_MAX-1 SEM name failed: %s\n", strerror(errno));
	close_test_sem(sem);
	cleanup_object(below_max, gid_a);
	cleanup_object(valid, gid_a);
	fd = group_shm_open(valid, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	CHECK(fd >= 0, "NAME_MAX SHM name failed: %s\n", strerror(errno));
	close_test_fd(fd);
	cleanup_object(valid, gid_a);
	sem = group_sem_open(valid, O_CREAT | O_EXCL, gid_a, 0600, 0);
	CHECK(sem != SEM_FAILED, "NAME_MAX SEM name failed: %s\n", strerror(errno));
	close_test_sem(sem);
	cleanup_object(valid, gid_a);
}

static void check_overlong_names(void)
{
	char too_long[NAME_MAX + 2];
	int tmp_before;

	(void)memset(too_long, 'x', NAME_MAX + 1);
	too_long[NAME_MAX + 1] = 0;
	tmp_before = count_tmp_files(gid_a);
	errno = 0;
	CHECK(group_shm_open(too_long, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a) == -1 && errno == ENAMETOOLONG,
		"overlong SHM name got errno %d\n", errno);
	errno = 0;
	CHECK(group_sem_open(too_long, O_CREAT | O_EXCL, gid_a, 0600, 0) == SEM_FAILED && errno == ENAMETOOLONG,
		"overlong SEM name got errno %d\n", errno);
	CHECK(count_tmp_files(gid_a) == tmp_before, "overlong names left tmp files\n");
}

static void check_leading_slash_names(void)
{
	char slash_name[96], plain_name[96], single_slash[100], multi_slash[100];
	sem_t *sem, *sem_again;
	int fd;

	make_name(slash_name, sizeof slash_name, "gshm-slash");
	while (slash_name[0] == '/') {
		(void)memmove(slash_name, slash_name + 1, strlen(slash_name));
	}
	(void)snprintf(plain_name, sizeof plain_name, "%s", slash_name);
	(void)snprintf(single_slash, sizeof single_slash, "/%s", plain_name);
	(void)snprintf(multi_slash, sizeof multi_slash, "///%s", plain_name);
	fd = group_shm_open(plain_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (fd < 0) {
		TEST_LOG("leading-slash SHM setup failed: name=%s\n", plain_name);
	}
	close_test_fd(fd);
	fd = group_shm_open(single_slash, O_RDWR, 0, gid_a);
	CHECK(fd >= 0, "single leading slash did not map to existing SHM\n");
	close_test_fd(fd);
	fd = group_shm_open(multi_slash, O_RDWR, 0, gid_a);
	CHECK(fd >= 0, "multiple leading slashes did not map to existing SHM\n");
	close_test_fd(fd);
	cleanup_object(plain_name, gid_a);
	sem = group_sem_open(plain_name, O_CREAT | O_EXCL, gid_a, 0600, 1);
	sem_again = group_sem_open(single_slash, 0, gid_a);
	CHECK(sem != SEM_FAILED && sem_again == sem,
		"single leading slash did not map to existing SEM\n");
	close_test_sem(sem_again);
	sem_again = group_sem_open(multi_slash, 0, gid_a);
	CHECK(sem != SEM_FAILED && sem_again == sem,
		"multiple leading slashes did not map to existing SEM\n");
	close_test_sem(sem_again);
	close_test_sem(sem);
	cleanup_object(plain_name, gid_a);
}

static void check_dotted_names(void)
{
	sem_t *sem;
	int fd, i;

	for (i = 0; i < 2; i++) {
		const char *dot_name = i ? "..." : ".x";
		cleanup_object(dot_name, gid_a);
		fd = group_shm_open(dot_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
		CHECK(fd >= 0, "valid dotted SHM name '%s' failed: %s\n", dot_name, strerror(errno));
		close_test_fd(fd);
		cleanup_object(dot_name, gid_a);
		sem = group_sem_open(dot_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
		CHECK(sem != SEM_FAILED, "valid dotted SEM name '%s' failed: %s\n", dot_name, strerror(errno));
		close_test_sem(sem);
		cleanup_object(dot_name, gid_a);
	}
}

/**
 * @tc.name      : group_shm_sem_1200
 * @tc.desc      : Test invalid, boundary-length, leading-slash, and dotted object names.
 * @tc.level     : Level 0
 */
static void test_name_validation(void)
{
	if (prepare_case_directory(gid_a)) {
		return;
	}
	check_invalid_names();
	check_name_length_boundaries();
	check_overlong_names();
	check_leading_slash_names();
	check_dotted_names();
}

/**
 * @tc.name      : group_shm_sem_1300
 * @tc.desc      : Test gid zero and the maximum gid_t value.
 * @tc.level     : Level 0
 */
static void test_gid_boundaries(void)
{
	gid_t gids[] = {0, (gid_t)-1};
	char name[96], path[PATH_MAX];
	struct stat st;
	sem_t *sem;
	int fd, i;

	for (i = 0; i < 2; i++) {
		make_name(name, sizeof name, i ? "gmax" : "gzero");
		if (prepare_case_directory(gids[i])) {
			continue;
		}
		cleanup_object(name, gids[i]);
		fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gids[i]);
		CHECK(fd >= 0, "boundary gid %u SHM create failed: %s\n", (unsigned)gids[i], strerror(errno));
		make_path(path, sizeof path, gids[i], name);
		CHECK(!stat(path, &st), "boundary gid %u mapped to wrong path\n", (unsigned)gids[i]);
		close_test_fd(fd);
		cleanup_object(name, gids[i]);
		sem = group_sem_open(name, O_CREAT | O_EXCL, gids[i], 0600, 0);
		CHECK(sem != SEM_FAILED, "boundary gid %u SEM create failed: %s\n", (unsigned)gids[i], strerror(errno));
		close_test_sem(sem);
		cleanup_object(name, gids[i]);
	}
}

/**
 * @tc.name      : group_shm_sem_1400
 * @tc.desc      : Test that group object operations do not change the gid directory attributes.
 * @tc.level     : Level 0
 */
static void test_directory_unchanged(void)
{
	char dir_path[128], shm_name[96], sem_name[96];
	struct stat before, after;
	sem_t *sem = SEM_FAILED;
	int fd = -1;

	if (prepare_case_directory(gid_a)) {
		return;
	}
	(void)snprintf(dir_path, sizeof dir_path, "/dev/group/shm/%u", (unsigned)gid_a);
	if (stat(dir_path, &before)) {
		(void)t_error("group directory stat failed: %s\n", strerror(errno));
		return;
	}
	make_name(shm_name, sizeof shm_name, "gshm-dirattr");
	make_name(sem_name, sizeof sem_name, "gsem-dirattr");
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	fd = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	if (fd < 0 || sem == SEM_FAILED) {
		(void)t_error("group directory attribute test setup failed: %s\n", strerror(errno));
	}
	close_test_fd(fd);
	close_test_sem(sem);
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	if (stat(dir_path, &after)) {
		(void)t_error("group directory stat after operations failed: %s\n", strerror(errno));
		return;
	}
	CHECK(before.st_dev == after.st_dev && before.st_ino == after.st_ino &&
		before.st_uid == after.st_uid && before.st_gid == after.st_gid &&
		(before.st_mode & 07777) == (after.st_mode & 07777),
		"group interfaces changed directory attributes\n");
}

static int check_error_fixture_paths(void)
{
	char absent_path[128], notdir_path[128];
	struct stat absent_st;

	(void)snprintf(absent_path, sizeof absent_path, "/dev/group/shm/%u", (unsigned)gid_absent);
	errno = 0;
	if (lstat(absent_path, &absent_st) != -1 || errno != ENOENT) {
		(void)t_error("absent gid fixture must not exist: %s, errno=%d\n", absent_path, errno);
		return -1;
	}
	(void)snprintf(notdir_path, sizeof notdir_path, "/dev/group/shm/%u", (unsigned)gid_notdir);
	if (lstat(notdir_path, &absent_st) || !S_ISREG(absent_st.st_mode)) {
		(void)t_error("not-directory gid fixture is invalid: %s\n", notdir_path);
		return -1;
	}
	return 0;
}

static void check_missing_objects(const char *shm_name, const char *sem_name)
{
	errno = 0;
	CHECK(group_shm_open(shm_name, O_RDWR, 0, gid_a) == -1 && errno == ENOENT,
		"missing SHM got errno %d\n", errno);
	errno = 0;
	CHECK(group_sem_open(sem_name, 0, gid_a) == SEM_FAILED && errno == ENOENT,
		"missing SEM got errno %d\n", errno);
	errno = 0;
	CHECK(group_shm_unlink(shm_name, gid_a) == -1 && errno == ENOENT,
		"missing SHM unlink got errno %d\n", errno);
	errno = 0;
	CHECK(group_sem_unlink(sem_name, gid_a) == -1 && errno == ENOENT,
		"missing SEM unlink got errno %d\n", errno);
}

static void check_notdir_errors(const char *shm_name, const char *sem_name)
{
	errno = 0;
	CHECK(group_shm_open(shm_name, O_CREAT | O_RDWR, 0600, gid_notdir) == -1 && errno == ENOTDIR,
		"SHM not-directory gid got errno %d\n", errno);
	errno = 0;
	CHECK(group_sem_open(sem_name, O_CREAT, gid_notdir, 0600, 0) == SEM_FAILED && errno == ENOTDIR,
		"SEM not-directory gid got errno %d\n", errno);
	errno = 0;
	CHECK(group_shm_unlink(shm_name, gid_notdir) == -1 && errno == ENOTDIR,
		"SHM unlink not-directory gid got errno %d\n", errno);
	errno = 0;
	CHECK(group_sem_unlink(sem_name, gid_notdir) == -1 && errno == ENOTDIR,
		"SEM unlink not-directory gid got errno %d\n", errno);
}

static void check_duplicate_creation(const char *shm_name, const char *sem_name)
{
	sem_t *sem;
	int fd;

	fd = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a);
	if (fd >= 0) {
		errno = 0;
		CHECK(group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600, gid_a) == -1 && errno == EEXIST,
			"duplicate SHM create got errno %d\n", errno);
		(void)close(fd);
	} else {
		TEST_LOG("duplicate SHM create test setup failed: name=%s\n", shm_name);
	}
	cleanup_object(shm_name, gid_a);
	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, 1);
	if (sem != SEM_FAILED) {
		errno = 0;
		CHECK(group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, 2) == SEM_FAILED && errno == EEXIST,
			"duplicate SEM create got errno %d\n", errno);
		(void)sem_close(sem);
	} else {
		TEST_LOG("duplicate SEM create test setup failed: name=%s\n", sem_name);
	}
	cleanup_object(sem_name, gid_a);
}

static void check_invalid_sem_value(const char *sem_name)
{
	sem_t *sem;
	int tmp_before;

	tmp_before = count_tmp_files(gid_a);
	errno = 0;
	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, SEM_VALUE_MAX + 1U);
	CHECK(sem == SEM_FAILED && errno == EINVAL, "invalid SEM value got %p/%d\n", sem, errno);
	CHECK(count_tmp_files(gid_a) == tmp_before, "invalid SEM value left tmp files\n");
}

static void check_no_path_fallback(void)
{
	char normal_name[96];
	sem_t *normal_sem;
	int normal_fd;

	make_name(normal_name, sizeof normal_name, "normal-shm");
	normal_fd = shm_open(normal_name, O_CREAT | O_EXCL | O_RDWR, 0600);
	if (normal_fd >= 0) {
		errno = 0;
		CHECK(group_shm_open(normal_name, O_RDWR, 0, gid_absent) == -1 && errno == ENOENT,
			"group SHM fell back to /dev/shm, errno %d\n", errno);
		(void)close(normal_fd);
		(void)shm_unlink(normal_name);
	} else {
		(void)t_error("normal shm setup for no-fallback test failed: %s\n", strerror(errno));
	}
	make_name(normal_name, sizeof normal_name, "normal-sem");
	normal_sem = sem_open(normal_name, O_CREAT | O_EXCL, 0600, 0);
	if (normal_sem != SEM_FAILED) {
		errno = 0;
		CHECK(group_sem_open(normal_name, 0, gid_absent) == SEM_FAILED && errno == ENOENT,
			"group SEM fell back to /dev/shm, errno %d\n", errno);
		(void)sem_close(normal_sem);
		(void)sem_unlink(normal_name);
	} else {
		(void)t_error("normal sem setup for no-fallback test failed: %s\n", strerror(errno));
	}
}

static void check_missing_parent_creation(const char *shm_name, const char *sem_name)
{
	char path[128];
	struct stat st;
	int fd;
	sem_t *sem;

	errno = 0;
	fd = group_shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0660, gid_absent);
	CHECK(fd == -1 && errno == ENOENT, "missing parent SHM create got fd=%d errno=%d\n", fd, errno);
	close_test_fd(fd);
	errno = 0;
	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_absent, (mode_t)0660, 1U);
	CHECK(sem == SEM_FAILED && errno == ENOENT, "missing parent SEM create got errno=%d\n", errno);
	close_test_sem(sem);
	(void)snprintf(path, sizeof path, "/dev/group/shm/%u", (unsigned)gid_absent);
	errno = 0;
	CHECK(lstat(path, &st) == -1 && errno == ENOENT, "interface created missing parent %s\n", path);
}

/**
 * @tc.name      : group_shm_sem_1500
 * @tc.desc      : Test missing objects/parents, invalid directories, duplicate creation, and no fallback.
 * @tc.level     : Level 0
 */
static void test_business_errors(void)
{
	char shm_name[96], sem_name[96];

	if (prepare_case_directory(gid_a) || init_notdir_fixture() || check_error_fixture_paths()) {
		return;
	}
	make_name(shm_name, sizeof shm_name, "gshm-errors");
	make_name(sem_name, sizeof sem_name, "gsem-errors");
	cleanup_object(shm_name, gid_a);
	cleanup_object(sem_name, gid_a);
	check_missing_objects(shm_name, sem_name);
	check_notdir_errors(shm_name, sem_name);
	check_duplicate_creation(shm_name, sem_name);
	check_invalid_sem_value(sem_name);
	check_no_path_fallback();
	check_missing_parent_creation(shm_name, sem_name);
}

static int collect_exclusive_results(int result_fd)
{
	int i, successes;
	struct race_result result;

	successes = 0;
	for (i = 0; i < RACE_WORKERS; i++) {
		if (read_all_timeout(result_fd, &result, sizeof result, CHILD_TIMEOUT_MS)) {
			(void)t_error("race worker result timed out\n");
			break;
		}
		if (result.success) {
			successes++;
		} else {
			CHECK(result.error == EEXIST, "race loser got errno %d, wanted EEXIST\n", result.error);
		}
	}
	return successes;
}

/**
 * @tc.name      : group_shm_sem_1700
 * @tc.desc      : Test concurrent exclusive creation of group shared memory and semaphores.
 * @tc.level     : Level 1
 */
static void test_exclusive_races(void)
{
	const char *modes[] = {"SHM", "SEM"};
	char name[96];
	int mode, i, start_pipe[2], result_pipe[2], successes, tmp_before;
	pid_t children[RACE_WORKERS];

	if (prepare_case_directory(gid_a)) {
		return;
	}
	for (mode = 0; mode < 2; mode++) {
		make_name(name, sizeof name, mode ? "gsem-race" : "gshm-race");
		cleanup_object(name, gid_a);
		tmp_before = count_tmp_files(gid_a);
		start_pipe[0] = start_pipe[1] = -1;
		result_pipe[0] = result_pipe[1] = -1;
		if (pipe(start_pipe) || pipe(result_pipe)) {
			(void)t_error("race pipes failed: %s\n", strerror(errno));
			close_pipe_pair(start_pipe);
			close_pipe_pair(result_pipe);
			return;
		}
		for (i = 0; i < RACE_WORKERS; i++) {
			children[i] = spawn_child();
			if (!children[i]) {
				(void)close(start_pipe[1]);
				(void)close(result_pipe[0]);
				_exit(child_race(gid_a, name, start_pipe[0], result_pipe[1], mode));
			}
		}
		for (i = 0; i < RACE_WORKERS; i++) {
			(void)write_all(start_pipe[1], "G", 1);
		}
		(void)close(start_pipe[0]);
		(void)close(start_pipe[1]);
		(void)close(result_pipe[1]);
		successes = collect_exclusive_results(result_pipe[0]);
		(void)close(result_pipe[0]);
		for (i = 0; i < RACE_WORKERS; i++) {
			if (children[i] > 0) {
				(void)wait_child(children[i], modes[mode]);
			}
		}
		CHECK(successes == 1, "%s race had %d successful creators, wanted 1\n", modes[mode], successes);
		CHECK(count_tmp_files(gid_a) == tmp_before, "%s race left tmp files\n", modes[mode]);
		cleanup_object(name, gid_a);
	}
}

static void check_shm_creator_exit(void)
{
	char shm_name[96];
	struct shared_record *record;
	int fd = -1;
	pid_t pid;

	make_name(shm_name, sizeof shm_name, "gshm-exit");
	cleanup_object(shm_name, gid_a);
	pid = spawn_child();
	if (!pid) {
		_exit(child_shm_creator(gid_a, shm_name));
	}
	if (pid > 0 && !wait_child(pid, "SHM creator exit")) {
		fd = group_shm_open(shm_name, O_RDWR, 0, gid_a);
		if (fd < 0) {
			(void)t_error("SHM could not be opened after creator exit: %s\n", strerror(errno));
		} else {
			record = mmap(0, sizeof *record, PROT_READ, MAP_SHARED, fd, 0);
			CHECK(record != MAP_FAILED && record->magic == TEST_MAGIC && record->value == 789 &&
				record->checksum == record_checksum(record), "SHM data invalid after creator exit\n");
			unmap_test_memory(record, sizeof *record);
			(void)close(fd);
		}
	}
	cleanup_object(shm_name, gid_a);
}

static void check_sem_waiter_exit(void)
{
	char waiter_name[96], byte;
	sem_t *sem = SEM_FAILED;
	int notify[2];
	pid_t pid;

	make_name(waiter_name, sizeof waiter_name, "gsem-waiter-kill");
	cleanup_object(waiter_name, gid_a);
	sem = group_sem_open(waiter_name, O_CREAT | O_EXCL, gid_a, 0600, 0);
	if (sem != SEM_FAILED && !pipe(notify)) {
		pid = spawn_child();
		if (!pid) {
			(void)close(notify[0]);
			_exit(child_sem_waiter(sem, notify[1], notify[1]));
		}
		(void)close(notify[1]);
		if (pid > 0) {
			CHECK(!read_all_timeout(notify[0], &byte, 1, CHILD_TIMEOUT_MS),
				"SEM waiter did not become ready before termination\n");
			(void)kill(pid, SIGKILL);
			(void)waitpid(pid, 0, 0);
			CHECK(!sem_post(sem) && !sem_wait_seconds(sem, 1),
				"SEM was corrupted after a waiting process terminated\n");
		}
		(void)close(notify[0]);
		(void)sem_close(sem);
		sem = SEM_FAILED;
	} else if (sem != SEM_FAILED) {
		(void)t_error("SEM waiter termination pipe failed: %s\n", strerror(errno));
		(void)sem_close(sem);
		sem = SEM_FAILED;
	} else {
		TEST_LOG("SEM waiter termination setup failed: name=%s\n", waiter_name);
	}
	cleanup_object(waiter_name, gid_a);
}

static void check_sem_holder_exit(void)
{
	char sem_name[96], byte;
	sem_t *sem = SEM_FAILED;
	int notify[2];
	pid_t pid;

	make_name(sem_name, sizeof sem_name, "gsem-holder-exit");
	cleanup_object(sem_name, gid_a);
	sem = group_sem_open(sem_name, O_CREAT | O_EXCL, gid_a, 0600, 1);
	if (sem == SEM_FAILED || pipe(notify)) {
		TEST_LOG("SEM holder termination open or pipe setup failed: name=%s\n", sem_name);
		goto out;
	}
	pid = spawn_child();
	if (!pid) {
		(void)close(notify[0]);
		_exit(child_sem_holder(sem, notify[1]));
	}
	(void)close(notify[1]);
	if (pid > 0) {
		CHECK(!read_all_timeout(notify[0], &byte, 1, CHILD_TIMEOUT_MS),
			"SEM holder did not acquire the semaphore\n");
		(void)wait_child(pid, "SEM holder exit");
		errno = 0;
		CHECK(sem_wait_seconds(sem, 1) == -1 && errno == ETIMEDOUT,
			"SEM was unexpectedly recovered after holder exit, errno %d\n", errno);
		CHECK(!sem_post(sem), "SEM manual recovery post failed\n");
		CHECK(!sem_wait_seconds(sem, 1), "SEM did not work after manual recovery\n");
		(void)sem_post(sem);
	}
	(void)close(notify[0]);
out:
	close_test_sem(sem);
	cleanup_object(sem_name, gid_a);
}

/**
 * @tc.name      : group_shm_sem_1800
 * @tc.desc      : Test object behavior after creator, waiter, or semaphore holder process exits.
 * @tc.level     : Level 1
 */
static void test_process_failures(void)
{
	if (prepare_case_directory(gid_a)) {
		return;
	}
	check_shm_creator_exit();
	check_sem_waiter_exit();
	check_sem_holder_exit();
}

int main(void)
{
	int tmp_a_before, tmp_b_before;

	(void)signal(SIGPIPE, SIG_IGN);
	init_gids();
	if (t_status || init_test_environment()) {
		return t_status;
	}
	tmp_a_before = count_tmp_files(gid_a);
	tmp_b_before = count_tmp_files(gid_b);
	if (tmp_a_before < 0 || tmp_b_before < 0) {
		(void)t_error("cannot inspect group test directories for temporary files: "
			"tmp_a_before=%d, tmp_b_before=%d\n", tmp_a_before, tmp_b_before);
		return t_status;
	}

	test_shm_basic();
	test_shm_cross_process();
	test_shm_gid_isolation();
	test_shm_unlink_lifetime();
	test_shm_truncate_and_readonly();
	test_sem_basic();
	test_sem_cross_process();
	test_sem_gid_isolation();
	test_sem_unlink_lifetime();
	test_sem_value_boundaries();
	test_e2e();
	test_name_validation();
	test_gid_boundaries();
	test_directory_unchanged();
	test_business_errors();
	test_exclusive_races();
	test_process_failures();
	CHECK(count_tmp_files(gid_a) == tmp_a_before,
		"group business tests left tmp files in gid %u\n", (unsigned)gid_a);
	CHECK(count_tmp_files(gid_b) == tmp_b_before,
		"group business tests left tmp files in gid %u\n", (unsigned)gid_b);
	return t_status;
}
