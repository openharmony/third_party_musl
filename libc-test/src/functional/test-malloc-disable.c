#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <memory.h>
#include <unistd.h>

#include "test-malloc-api-common.h"

pthread_barrier_t routine_disabled;
pthread_barrier_t routine_allocated;

const size_t SLEEP_TIME_SECONDS = 1;

void *disable_routine(void *vargp)
{
	malloc_disable();
	pthread_barrier_wait(&routine_disabled);
	sleep(SLEEP_TIME_SECONDS);
	malloc_enable();
	pthread_barrier_wait(&routine_allocated);
	return NULL;
}

int test_malloc_while_disabled(void)
{
	int ret = 0;
	pthread_barrier_init(&routine_disabled, NULL, 2);
	pthread_barrier_init(&routine_allocated, NULL, 2);
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, disable_routine, NULL);
	pthread_barrier_wait(&routine_disabled);
	time_t start = time(0);
	int *x = malloc(sizeof(int));
	pthread_barrier_wait(&routine_allocated);
	time_t end = time(0);
	size_t seconds = end - start;
	if (seconds < SLEEP_TIME_SECONDS)
	{
		ret = -1;
	}
	free(x);
	pthread_join(thread_id, NULL);

	return ret;
}

int main()
{
	int ret = 0;

	ret = check_and_report("Testing malloc while disabled", test_malloc_while_disabled);

	return ret;
}
