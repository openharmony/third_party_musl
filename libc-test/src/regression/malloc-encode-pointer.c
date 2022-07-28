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
 
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "test.h"

int main(int argc, char *argv[])
{
	uintptr_t *c0 = (uintptr_t *)malloc(sizeof(uintptr_t) * 10);
	if (!c0) {
		t_error("Malloc c0 failed: %s\n", strerror(errno));
		return -1;
	}
	/* Malloc dividing chunk to avoid combination of neighbouring freed chunk */
	void *d0 = malloc(sizeof(uintptr_t) * 10);
	if (!d0) {
		t_error("Malloc d0 failed: %s\n", strerror(errno));
		return -1;
	}

	uintptr_t *c1 = (uintptr_t *)malloc(sizeof(uintptr_t) * 10);
	if (!c1) {
		t_error("Malloc c1 failed: %s\n", strerror(errno));
		return -1;
	}
	/* Malloc dividing chunk to avoid combination of neighbouring freed chunk */
	void *d1 = malloc(sizeof(uintptr_t) * 10);
	if (!d1) {
		t_error("Malloc d1 failed: %s\n", strerror(errno));
		return -1;
	}
	
	/* Free the chunk, with same size, they're put into the same bin */
	/*    --------      --------      --------
	 *    |  c0  |      |  c1  |      |  bin |
	 * -->| next |----->| next |----->| next |-----
	 * |  | prev |<-----| prev |<-----| prev |    |
	 * |  -------       -------       -------     |
	 * --------------------------------------------
	 */
	free(c0);
	free(c1);

	uintptr_t xoraddr = c0[0]; /* Get the next of c0 */
	uintptr_t realaddr = (uintptr_t)((char *)c1 - sizeof(uintptr_t) * 2);
	if (xoraddr == realaddr) {
		t_error("encoding pointer is equal to real pointer\n");
	}
	
	free(d0);
	free(d1);

	return t_status;
}
