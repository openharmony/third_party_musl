#include <malloc.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "test.h"

#define THREAD_MAX_N 3
#define SIZE_ALIGN (4 * sizeof(size_t))

void *func(void *arg)
{
	int *val = (int *)arg;
	cpu_set_t mask;
	void *ptr;

	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask) < 0) {
		t_error("Set CPU affinity of thread %d failure, ERROR:%s\n", *val, strerror(errno));
		return NULL;
	}

	ptr = malloc(SIZE_ALIGN);
	if (!ptr) {
		t_error("Thread %d malloc failed for size %zu\n", *val, SIZE_ALIGN);
		*val = errno;
		return NULL;
	}
	free(ptr);

	*val = 0;
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_attr_t attr;
	pthread_t tids[THREAD_MAX_N];
	int t_result[THREAD_MAX_N] = {0};
	int created = 0;
	int flag = 0;
	int ret;
	int i;

	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		t_error("Init pthread attribute failed: %s\n", strerror(ret));
		return -1;
	}

	for (i = 0; i < THREAD_MAX_N; ++i) {
		t_result[i] = i;
		ret = pthread_create(&tids[i], &attr, func, &t_result[i]);
		if (ret != 0) {
			t_error("Create pthread %u failed: %s\n", i, strerror(ret));
			flag = -1;
			break;
		}
		created++;
	}

	for (i = 0; i < created; ++i) {
		ret = pthread_join(tids[i], NULL);
		if (ret != 0) {
			t_error("Join thread %u failed: %s\n", i, strerror(ret));
			flag = -1;
		}
		if (t_result[i] != 0) {
			flag = -1;
		}
	}

	(void)pthread_attr_destroy(&attr);

	if (!flag) {
		t_status = 0;
	}

	return t_status;
}
