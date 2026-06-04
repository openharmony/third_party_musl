/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
/*
 * For riscv32 lite ndk link function
 */
#include <stddef.h>
#include <features.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int *__errno_location(void)
{
    return NULL;
}
weak_alias(__errno_location, ___errno_location);

void __tl_lock(void)
{
}

void __tl_unlock(void)
{
}

int __membarrier(int cmd, int flags)
{
    return 0;
}

void __getopt_msg(const char *a, const char *b, const char *c, size_t l)
{
}

int __lockfile(FILE *f)
{
    return 0;
}

void __unlockfile(FILE *f)
{
}

char *__randname(char *template)
{
    return NULL;
}

char *__asctime_r(const struct tm *restrict tm, char *restrict buf)
{
    return NULL;
}

void __dl_seterr(const char *fmt, ...)
{
}

int __dl_invalid_handle(void *h)
{
    return 0;
}

void __dl_vseterr(const char *fmt, va_list ap)
{
}

void __vdsosym(const char *vername, const char *name)
{
}