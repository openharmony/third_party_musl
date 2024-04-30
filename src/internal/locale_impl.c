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

#ifdef FEATURE_ICU_LOCALE
#include <dlfcn.h>
#include <musl_log.h>
#include <string.h>
#include "locale_impl.h"

#define ICU_UC_SO "libhmicuuc.z.so"
#define ICU_I18N_SO "libhmicui18n.z.so"

static void *g_icuuc_handle = NULL;
static void *g_icui18n_handle = NULL;
hidden struct icu_opt_func g_icu_opt_func = { NULL };

#define VALID_ICU_NAME_LEN 5
static char g_valid_icu_locale_name[VALID_ICU_NAME_LEN + 1];

void *get_icu_handle(ICU_SO_TYPE type, const char *symbol_name)
{
	void *cur_handle;
	char *cur_so;
	if (type == ICU_UC) {
		cur_handle = g_icuuc_handle;
		cur_so = ICU_UC_SO;
	} else {
		cur_handle = g_icui18n_handle;
		cur_so = ICU_I18N_SO;
	}

	if (!cur_handle) {
		cur_handle = dlopen(cur_so, RTLD_LOCAL);
	}
	if (!cur_handle) {
		MUSL_LOGE("dlopen icu so for musl locale fail %{public}s", dlerror());
		return NULL;
	}
	return dlsym(cur_handle, symbol_name);
}

void get_icu_symbol(ICU_SO_TYPE type, void **icu_symbol_handle, const char *symbol_name)
{
	if (!(*icu_symbol_handle)) {
		*icu_symbol_handle = get_icu_handle(type, symbol_name);
	}
}

/* ICU methods don't need charset for locale, process the given locale name */
char *get_valid_icu_locale_name(const char *name)
{
	strncpy(g_valid_icu_locale_name, name, VALID_ICU_NAME_LEN);
	g_valid_icu_locale_name[VALID_ICU_NAME_LEN] = '\0';
	return g_valid_icu_locale_name;
}
#endif
