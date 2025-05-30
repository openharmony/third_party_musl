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
#include <stdio.h>
#include <dlfcn.h>
#include <musl_log.h>
#include <string.h>
#include "locale_impl.h"

#define ICU_UC_SO "libhmicuuc.z.so"
#define ICU_I18N_SO "libhmicui18n.z.so"

static void *g_icuuc_handle = NULL;
static void *g_icui18n_handle = NULL;
hidden struct icu_opt_func g_icu_opt_func = { NULL };
static int dlopen_fail_flag = 0;
static int icuuc_handle_init_succeed = 0;
static int icuuc_wctype_handle_init_succeed = 0;
bool icu_locale_wctype_enable = false;
pthread_mutex_t icu_wctype_init_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *get_icu_handle(icu_so_type type, const char *symbol_name)
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

	if (!cur_handle && !dlopen_fail_flag) {
		cur_handle = dlopen(cur_so, RTLD_LOCAL);
        if (type == ICU_UC) {
            g_icuuc_handle = cur_handle;
        } else {
            g_icui18n_handle = cur_handle;
        }
	}
	if (!cur_handle) {
		dlopen_fail_flag = 1;
		MUSL_LOGE("dlopen icu so for musl locale fail %{public}s", dlerror());
		return NULL;
	}
	return dlsym(cur_handle, symbol_name);
}

static char *get_icu_version_num()
{
	if (!(g_icu_opt_func.get_icu_version)) {
		g_icu_opt_func.get_icu_version = get_icu_handle(ICU_UC, ICU_GET_VERSION_NUM_SYMBOL);
	}
	if (g_icu_opt_func.get_icu_version) {
		return g_icu_opt_func.get_icu_version();
	} else {
		return "";
	}
}

void get_icu_symbol(icu_so_type type, void **icu_symbol_handle, const char *symbol_name)
{
	if (!(*icu_symbol_handle)) {
		char *icu_version = get_icu_version_num();
		char *valid_icu_symbol = malloc(strlen(symbol_name) + strlen(icu_version) + 2);
		sprintf(valid_icu_symbol, "%s_%s", symbol_name, icu_version);
		*icu_symbol_handle = get_icu_handle(type, valid_icu_symbol);
		free(valid_icu_symbol);
	}
}

void set_icu_directory()
{
	if (!(g_icu_opt_func.set_data_directory)) {
		g_icu_opt_func.set_data_directory = get_icu_handle(ICU_UC, ICU_SET_DATA_DIRECTORY_SYMBOL);
		if (g_icu_opt_func.set_data_directory) {
			g_icu_opt_func.set_data_directory();
		}
	}
}

/* ICU methods don't need charset for locale, process the given locale name */
void get_valid_icu_locale_name(const char *name, const char *icu_name, int icu_name_len)
{
	char *pos = memchr(name, '.', strlen(name));
	int valid_len;
	if (pos) {
		valid_len = pos - name;
	} else {
		valid_len = strlen(name);
	}
	if (icu_name_len > valid_len) {
		strncpy((char *)icu_name, name, valid_len);
	}
}

int set_wctype_icu_enable()
{
    pthread_mutex_lock(&icu_wctype_init_mutex);
    if (!icuuc_wctype_handle_init()){
        pthread_mutex_unlock(&icu_wctype_init_mutex);
        return ICU_SYMBOL_LOAD_ERROR;
    }

    icu_locale_wctype_enable = true;
    pthread_mutex_unlock(&icu_wctype_init_mutex);
    return ICU_ZERO_ERROR;
}

bool icuuc_wctype_handle_init()
{
    if (icuuc_wctype_handle_init_succeed) {
        return true;
    }
    if (!g_icu_opt_func.u_isalnum) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isalnum), ICU_UCHAR_ISALNUM_SYMBOL);
        if (!g_icu_opt_func.u_isalnum) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isalpha) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isalpha), ICU_UCHAR_ISALPHA_SYMBOL);
        if (!g_icu_opt_func.u_isalpha) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isblank) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isblank), ICU_UCHAR_ISBLANK_SYMBOL);
        if (!g_icu_opt_func.u_isblank) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_iscntrl) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_iscntrl), ICU_UCHAR_ISCNTRL_SYMBOL);
        if (!g_icu_opt_func.u_iscntrl) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isdigit) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isdigit), ICU_UCHAR_ISDIGIT_SYMBOL);
        if (!g_icu_opt_func.u_isdigit) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isgraph) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isgraph), ICU_UCHAR_ISGRAPH_SYMBOL);
        if (!g_icu_opt_func.u_isgraph) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_islower) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_islower), ICU_UCHAR_ISLOWER_SYMBOL);
        if (!g_icu_opt_func.u_islower) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isprint) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isprint), ICU_UCHAR_ISPRINT_SYMBOL);
        if (!g_icu_opt_func.u_isprint) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_ispunct) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_ispunct), ICU_UCHAR_ISPUNCT_SYMBOL);
        if (!g_icu_opt_func.u_ispunct) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isspace) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isspace), ICU_UCHAR_ISSPACE_SYMBOL);
        if (!g_icu_opt_func.u_isspace) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isupper) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isupper), ICU_UCHAR_ISUPPER_SYMBOL);
        if (!g_icu_opt_func.u_isupper) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_isxdigit) {
        get_icu_symbol(ICU_I18N, (void **)&(g_icu_opt_func.u_isxdigit), ICU_UCHAR_ISXDIGIT_SYMBOL);
        if (!g_icu_opt_func.u_isxdigit) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_tolower) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.u_tolower), ICU_UCHAR_TOLOWER_SYMBOL);
        if (!g_icu_opt_func.u_tolower) {
            return false;
        }
    }
    if (!g_icu_opt_func.u_toupper) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.u_toupper), ICU_UCHAR_TOUPPER_SYMBOL);
        if (!g_icu_opt_func.u_toupper) {
            return false;
        }
    }
    icuuc_wctype_handle_init_succeed = true;
    return true;
}

bool icuuc_handle_init()
{
    if (icuuc_handle_init_succeed) {
        return true;
    }

    if (!g_icu_opt_func.set_data_directory) {
        g_icu_opt_func.set_data_directory = get_icu_handle(ICU_UC, ICU_SET_DATA_DIRECTORY_SYMBOL);
        if (g_icu_opt_func.set_data_directory) {
            g_icu_opt_func.set_data_directory();
        } else {
            return false;
        }
    }
    if (!g_icu_opt_func.ucnv_open) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.ucnv_open), ICU_UCNV_OPEN_SYMBOL);
        if (!g_icu_opt_func.ucnv_open) {
            return false;
        }
    }
    if (!g_icu_opt_func.ucnv_setToUCallBack) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.ucnv_setToUCallBack), ICU_UCNV_SETTOUCALLBACK_SYMBOL);
        if (!g_icu_opt_func.ucnv_setToUCallBack) {
            return false;
        }
    }
    if (!g_icu_opt_func.ucnv_setFromUCallBack) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.ucnv_setFromUCallBack), ICU_UCNV_SETFROMUCALLBACK_SYMBOL);
        if (!g_icu_opt_func.ucnv_setFromUCallBack) {
            return false;
        }
    }
    if (!g_icu_opt_func.ucnv_convertEx) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.ucnv_convertEx), ICU_UCNV_CONVERTEX_SYMBOL);
        if (!g_icu_opt_func.ucnv_convertEx) {
            return false;
        }
    }
    if (!g_icu_opt_func.ucnv_close) {
        get_icu_symbol(ICU_UC, (void **)&(g_icu_opt_func.ucnv_close), ICU_UCNV_CLOSE_SYMBOL);
        if (!g_icu_opt_func.ucnv_close) {
            return false;
        }
    }
    icuuc_handle_init_succeed = 1;
    errno = 0;
    return true;
}
#endif
