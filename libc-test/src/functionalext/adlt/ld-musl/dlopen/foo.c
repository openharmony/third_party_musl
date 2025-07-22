/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <securec.h>
#include "constant.h"
#include "foo.h"

static void *buf=0;
static size_t buflen=0;

int get_constant() { return CONSTANT; }

void free_buf_maybe()
{
    if (buf)
	{
	    free(buf);
		buf = NULL;
	    buflen = 0;
	}
}

void set_buf(void *p, size_t len)
{
    free_buf_maybe();
	buf = malloc(len);
    if (buf)
	{
	    buflen = len;
	    memcpy_s(buf, len, p, len);
	}
}

size_t get_buflen()
{
    return buflen;
}

void *get_bufptr()
{
    return buf;
}

int cmp_buf(void *caller_buf)
{
    return memcmp(buf, caller_buf, buflen);
}