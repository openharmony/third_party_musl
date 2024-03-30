/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef _MUSL_FDTRACK_HOOK_H
#define _MUSL_FDTRACK_HOOK_H

#include <stdatomic.h>
#include <sys/cdefs.h>
#include "common_def.h"
#include "musl_fdtrack.h"

extern _Atomic(fdtrack_hook) __fdtrack_hook;
extern bool __fdtrack_enabled;

#ifdef __cplusplus
extern "C" {
#endif

int FDTRACK_START_HOOK(int fd_value)
{
	int fd = fd_value;
	if (fd != -1 && __predict_false(__fdtrack_enabled) && __predict_false(__fdtrack_hook)) {
		struct fdtrack_event event;
		event.fd = fd;
		event.type = FDTRACK_EVENT_TYPE_CREATE;
		atomic_load(&__fdtrack_hook)(&event);
	}
	return fd;
}

#ifdef __cplusplus
}
#endif

#endif