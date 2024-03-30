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

#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <dlfcn.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "musl_log.h"

static bool g_needCheck = true;
static bool g_isCheckedBeta = false;
bool __fdtrack_hook_init_flag = false;
static char *__is_beta_version = "beta";
static char *__fdtrack_hook_shared_lib = "libfdleak_tracker.so";
static char *__key_version_type = "const.logsystem.versiontype";
long long __ohos_fdtrack_hook_shared_library;

static void* load_fdtrack_hook_shared_library()
{
	void* shared_library_handle = NULL;

	shared_library_handle = dlopen(__fdtrack_hook_shared_lib, RTLD_NOW | RTLD_LOCAL);

	if (shared_library_handle == NULL) {
		MUSL_LOGI("FdTrack, Unable to open shared library %s: %s.\n", __fdtrack_hook_shared_lib, dlerror());
		return NULL;
	}
	return shared_library_handle;
}

static void init_ohos_fdtrack_hook()
{
	void* shared_library_handle = (void *)__ohos_fdtrack_hook_shared_library;
	if (shared_library_handle != NULL && shared_library_handle != (void*)-1) {
		MUSL_LOGI("FdTrack, ohos_fdtrack_hook_shared_library has had.");
		return;
	}

	shared_library_handle = load_fdtrack_hook_shared_library();
	if (shared_library_handle == NULL) {
		MUSL_LOGI("FdTrack, load_fdtrack_hook_shared_library failed.");
		return;
	}
	MUSL_LOGI("FdTrack, load_fdtrack_hook_shared_library success.");
}

static bool is_beta_version()
{
	CachedHandle handle = CachedParameterCreate(__key_version_type, "unknown");
	const char *value = CachedParameterGet(handle);
	return (value != NULL && strncmp(value, __is_beta_version, strlen(__is_beta_version)) == 0);
}

static bool check_load_fdtrack()
{
	if (!g_needCheck) {
		return false;
	}
	if (!g_isCheckedBeta) {
		bool isBetaVersion = is_beta_version();
		g_isCheckedBeta = true;
		if (!isBetaVersion) {
			g_needCheck = false;
			return false;
		}
	}
	return true;
}

__attribute__((constructor())) static void __musl_fdtrack_initialize()
{
	if (!check_load_fdtrack()) {
		return;
	}
	MUSL_LOGI("FdTrack, %d begin musl_fdtrack_initialize, flag %d.\n", getpid(), __fdtrack_hook_init_flag);
	if (!__fdtrack_hook_init_flag) {
		__fdtrack_hook_init_flag = true;
		init_ohos_fdtrack_hook();
	}
}

#endif