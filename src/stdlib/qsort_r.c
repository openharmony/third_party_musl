/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <stdlib.h>

typedef int (*cmpfun)(const void *, const void *, void *);

void __qsort_r(void *base, size_t nel, size_t width, cmpfun cmp, void *arg);

void qsort_r(void *base, size_t nel, size_t width, cmpfun cmp, void *arg)
{
#ifdef MUSL_EXTERNAL_FUNCTION
	__qsort_r(base, nel, width, cmp, arg);
#else
	(void)base;
	(void)nel;
	(void)width;
	(void)cmp;
	(void)arg;
	errno = ENOSYS;
#endif
}
