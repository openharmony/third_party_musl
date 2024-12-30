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
 
/***************************************************************************
 * @CaseName: dlclose_10
 * @Brief: Run 1000 times dlclose to close dynamic library which depends on
 *         each other with concurrency
 ***************************************************************************/
#include "common.h" /* PLEASE DON'T MODIFY THIS LINE */

// Func: init environment before test
// Args: same as testcase
int tc_pre_test(int argc,char **argv)
{
	// @PRE-1: so exist
	char *files[] = {
		"/data/tests/libc-test/ola_test/libj.so",
		"/data/tests/libc-test/ola_test/libk.so"
	};
	int num_files = sizeof(files) /sizeof(files[0]);
	if (check_so_exist(num_files, files) != 0) {
		rt_err("check so failed\n");
	}
	return RT_EOK;
}

// Func: load and unload so depending on each other
// Args: same as testcase
int *thread_dlopen_dlclose_so(void *arg)
{
	int ret = 0;
	char *direct_libs[] = {
		"/data/tests/libc-test/ola_test/libdj.so",
		"/data/tests/libc-test/ola_test/libk.so"
	};
	void *direct_hdls = NULL;
	direct_hdls = dlopen(direct_libs[0], RTLD_NOW | RTLD_GLOBAL);
	if (!direct_hdls) {
		rt_err("dlopen %s failed: %s\n", direct_libs[0], dlerror());
	}
	ret = dlclose(direct_hdls);
	if (ret != 0) {
		rt_err("dlclose %s failed: %s\n", direct_libs[0], dlerror());
	}
	return NULL;
}

// Func: the core logic of test(this function must be implemented and do not
//       change the function name)
// Args: same as testcase
int tc_do_test(int argc, char **argv)
{
	// @STEP-1: load and unload so depending on each other 1000 times
	//          by concurrency
	// @EXPECT-1: load and unload successfully
	pthread_t thid[TEST_NUM];
	int ret;
	char *direct_libs[] = {
		"/data/tests/libc-test/ola_test/libj.so",
		"/data/tests/libc-test/ola_test/libk.so"
	}
	int num = sizeof(direct_libs) / sizeof(direct_libs[0]);
	// create 100 threads
	for (int k = 0; k < TEST_NUM; k++) {
		ret = pthread_create(&thid[k], NULL, (void *)thread_dlopen_dlclose_so, NULL);
		if (ret != 0) {
			rt_err("create pthread fail errno is %s\n", strerror(errno));
		}
	}
	for (int j = 0; j < TEST_NUM; j++) {
		ret = pthread_join(thid[j], NULL);
		if (ret != 0) {
			rt_err("pthread_join fail errno is %s\n", strerror(errno));
		}
	}
	for (int i = 0; i < num; i++) {
		if (check_loaded(direct_libs[i])) {
			rt_err("dso %s shouldn't exist in /proc/self/maps\n", direct_libs[i]);
		}
	}

	return RT_EOK;
}

// Func: clear the env after test, if needed
// Args: same as testcase
int tc_post_test(int argc, char **argv)
{
	return RT_EOK;
}

// Func: get debug information, if needed
int tc_get_debuginfo(int argc, char **argv)
{
	return RT_EOK;
}

int main() {
	if (tc_pre_test(0, NULL)) {
		rt_err("tc_pre_test failed:\n");
		return -1;
	}
	if (tc_do_test(0, NULL)) {
		rt_err("tc_do_test failed:\n");
		return -1;
	}
	if (tc_post_test(0, NULL)) {
		rt_err("tc_post_test failed:\n");
		return -1;
	}
	if (tc_get_debuginfo(0, NULL)) {
		rt_err("tc_get_debuginfo failed:\n");
		return -1;
	}
	return 0;
}
