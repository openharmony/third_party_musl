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
#include <iostream>
#include <dlfcn.h>

#include "functionalext.h"
#include "header.h"

#define STRIFY(x) #x
#define DL_STRIFY(x) STRIFY(x)

#ifdef SO_TYPE_TAG
static const char *add_lib = "libadlt_bugfix_match_sofilename_" DL_STRIFY(SO_TYPE_TAG) "_add.so";
static const char *sub_lib = "libadlt_bugfix_match_sofilename_" DL_STRIFY(SO_TYPE_TAG) "_sub.so";
#else
static const char *add_lib = "libadlt_bugfix_match_sofilename_add.so";
static const char *sub_lib = "libadlt_bugfix_match_sofilename_sub.so";
#endif

typedef int (*Add)(int);
typedef int (*Sub)(int, int);

static void test_bugfix_match_sofilename_dlopen() {
	void *handleAdd = dlopen(add_lib, RTLD_LAZY);
	if (!handleAdd) {
		t_error("%s: Failed to open add.so Err: %s\n", __func__, dlerror());
		return;
	}

	Add add = (Add) dlsym(handleAdd, "add");
	if (!add) {
		t_error("%s: Find Sym add Err: %s\n", __func__, dlerror());
		dlclose(handleAdd);
		return;
	}

	int resultAdd = add(10);
	dlclose(handleAdd);

	void *handleSub = dlopen(sub_lib, RTLD_LAZY);
	if (!handleSub) {
		t_error("%s: Failed to open sub.so Err: %s\n", __func__, dlerror());
		return;
	}

	Sub sub = (Sub) dlsym(handleSub, "sub");
	if (!sub) {
		t_error("%s: Find Sym sub Err: %s\n", __func__, dlerror());
		dlclose(handleSub);
		return;
	}

	int resultSub = sub(100, 20);
	dlclose(handleSub);

	EXPECT_EQ(__func__, resultAdd, 110);
	EXPECT_EQ(__func__, resultSub, 80);
}

int main(int argc, char **argv) {
	test_bugfix_match_sofilename_dlopen();
	return t_status;
}