/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "malloc_random.h"

#ifdef MALLOC_FREELIST_HARDENED
static bool get_random(uint64_t *x)
{
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		return false;
	}
	int ret = read(fd, x, sizeof(uint64_t));
	if (ret < 0) {
		close(fd);
		return false;
	}

	close(fd);
	return true;
}

void* next_key()
{
	uint64_t x = 0;
	struct timespec ts;
	/* Try to use urandom to get the random number first */
	if (!get_random(&x)) {
		/* Can't get random number from /dev/urandom, generate from addr based on ASLR and time */
		(void)clock_gettime(CLOCK_REALTIME, &ts);
		x = (((uint64_t)get_random) << 32) ^ (uint64_t)next_key ^ ts.tv_nsec;
	}
	/* Return an odd key, make sure that the xor pointer being odd */
	return (void *)(x | 1);
}

void* encode_ptr(void *ptr, void *key)
{
	return (void *)((uintptr_t)ptr ^ (uintptr_t)key);
}

#else // MALLOC_FREELIST_HARDENED
void* encode_ptr(void *ptr, void *key)
{
	(void)key;
	return ptr;
}
#endif // MALLOC_FREELIST_HARDENED
