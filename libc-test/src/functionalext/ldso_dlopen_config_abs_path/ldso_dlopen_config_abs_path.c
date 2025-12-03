/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include "ldso_dlopen_config_abs_path.h"

#define ABS_HELLO_SO "/data/local/tmp/libc-test-lib/libhello.so"
#define ABS_HELLO2_SO "/data/local/tmp/libc-test-lib/libhello2.so"
#define HELLO_SO "libhello.so"
#define HELLO2_SO "libhello2.so"
/**
通过排列组合进行测试
*/

FuncType *dlopen_config_abs_path_policy_func = NULL;
Dl_namespace ns1, ns2;

bool InitDlopenConfigAbsPathPolicy()
{
    dlopen_config_abs_path_policy_func = dlsym(RTLD_DEFAULT, "dlopen_config_abs_path_policy");
    if (dlopen_config_abs_path_policy_func == NULL) {
        printf("dlsym dlopen_config_abs_path_policy failed\n");
        return false;
    }
    return true;
}

#define OTHER_FLAG (0)

// TODO: 将所有的dlopen换成dlopen_ns，通过自定义的Ans继承自定义的Bns进行测试，并且测试时使用新生成的so文件，不使用系统内部的so
bool CheckPrerequisites()
{
    // init function
    bool result = InitDlopenConfigAbsPathPolicy();
    if (!result) {
        return false;
    }
    // check so file exists
    int fileExists = access(ABS_HELLO_SO, F_OK);
    if (fileExists != 0) {
        printf("ABS_HELLO_SO not exists\n");
        return false;
    }
    (void)system("cp /data/local/tmp/libc-test-lib/libhello.so /data/local/tmp/libc-test-lib/libhello2.so");
    fileExists = access(ABS_HELLO2_SO, F_OK);
    if (fileExists != 0) {
        printf("ABS_HELLO2_SO not exists\n");
        return false;
    }
    // init namespace
    dlns_init(&ns1, "ns1");
    dlns_init(&ns2, "ns2");
    int res = dlns_create2(&ns1, "/data/test/invalid", OTHER_FLAG);
    if (res) {
        printf("dlns_create2 ns1 failed res=%d errno=%d\n", res, errno);
        return false;
    }
    res = dlns_create2(&ns2, "/data/local/tmp/libc-test-lib", OTHER_FLAG);
    if (res) {
        printf("dlns_create2 ns2 failed res=%d errno=%d\n", res, errno);
        return false;
    }
    res = dlns_inherit(&ns1, &ns2, HELLO_SO);
    if (res) {
        printf("dlns_inherit ns1 from ns2 failed res=%d errno=%d\n", res, errno);
        return false;
    }
    res = dlns_set_namespace_separated("ns1", true);
    if (res) {
        printf("ns1 dlns_set_namespace_separated false failed res=%d errno=%d\n", res, errno);
        return false;
    }
    res = dlns_set_namespace_separated("ns2", true);
    if (res) {
        printf("ns2 dlns_set_namespace_separated false failed res=%d errno=%d\n", res, errno);
        return false;
    }
    return true;
}

bool OpenSo(bool absolute, bool inWhiteList)
{
    void *ptr = NULL;
    if (absolute) {
        if (inWhiteList) {
            ptr = dlopen_ns(&ns1, ABS_HELLO_SO, RTLD_NOW);
        } else {
            ptr = dlopen_ns(&ns1, ABS_HELLO2_SO, RTLD_NOW);
        }
    } else {
        if (inWhiteList) {
            ptr = dlopen_ns(&ns1, HELLO_SO, RTLD_NOW);
        } else {
            ptr = dlopen_ns(&ns1, HELLO2_SO, RTLD_NOW);
        }
    }
    return ptr ? true : false;
}
