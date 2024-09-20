/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _MUSL_FDTRACK_H
#define _MUSL_FDTRACK_H

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fdtrack_type {
	FDTRACK_EVENT_TYPE_CREATE,
};

struct fdtrack_event {
	uint32_t type;
	int fd;
};

typedef void (*fdtrack_hook)(struct fdtrack_event*);
void set_fdtrack_enabled(bool newValue);
bool fdtrack_cas_hook(fdtrack_hook* expected, fdtrack_hook value);
bool normal_flow_control(struct timeval prevTime, int interval);
bool check_open_func(const char* expected, const char* key);
bool check_beta_develop_before();
bool check_before_memory_allocate(struct timeval prevTime, int interval);

#ifdef __cplusplus
}
#endif
#endif