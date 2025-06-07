/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <hilog_adapter.h>
#include <stdlib.h>
#include "functionalext.h"
#include "test.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/un.h>
#include <bits/alltypes.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <dirent.h>
#include <securec.h>

#define MUSL_LOG_TYPE LOG_CORE
#define MUSL_LOG_DOMAIN 0xD003F00
#define MUSL_LOG_TAG "MUSL"
#define LOG_ERROR (6)
#define CLOSE_FD_COUNT (1024)

#define ENABLE_LOG "param set musl.log.enable true"
#define LOG_LEVEL_ERROR "param set musl.log.level ERROR"

#define MUSL_LOGE(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))

#define PRINT_STRING_A "HilogAdapterPrinttest_a"
#define PRINT_STRING_B "HilogAdapterPrinttest_b"
#define PRINT_STRING_C "HilogAdapterPrinttest_c"

#define ZERO (0)
#define TWO (2)
#define THREE (3)
#define NEGATIVE_ONE (-1)
#define INVALID_SOCKET (-1)
#define THREAD_COUNT (10)
#define STR_LENGTH (20)
char str[THREAD_COUNT][STR_LENGTH];

void GenerateRandomString(char* randomString, int index)
{
    #define RANDOM_STRING_LENGTH (16)
    #define RANDOM_STRING_END (18)
    #define RANDOM_STRING_START (2)
    // 将索引转换为字符串并拼接到随机字符串前面
    (void)sprintf_s(randomString, STR_LENGTH, "%02d", index);

    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int r = 0;
    int num = 0;
    int fd = open("/dev/random", O_RDONLY);
    if (fd == NEGATIVE_ONE) {
        strcpy_s(randomString + RANDOM_STRING_START, RANDOM_STRING_LENGTH, "MUSLHILOGFIXED");
    } else {
        for (int i = RANDOM_STRING_START; i < RANDOM_STRING_LENGTH; i++) {
            read(fd, &r, sizeof(int));
            // 非安全场景，使用种子确保随机字符串每次运行都不一样
            randomString[i] = charset[r % (sizeof(charset) - 1)];
        }
    }
    close(fd);
    randomString[RANDOM_STRING_END] = '\0'; // 添加字符串结尾符
}

// 检查是否有相应的日志生成，如果有当前日志，则返回true，如果没有则返回false
bool CheckHiLogPrint(char *needToMatch)
{
    #define COMMAND_SIZE (50)
    #define READ_SIZE (49)
    #define BUFFER_SIZE (1024)
    #define FAIL_CLOSE (-1)
    bool flag = false;
    // 1. 通过system函数读取当前hilog日志信息
    char command[] = "/bin/hilog -x | grep ";
    char finalCommand[COMMAND_SIZE];
    int res = snprintf_s(finalCommand, COMMAND_SIZE, READ_SIZE, "%s%s", command, needToMatch);
    if (res == NEGATIVE_ONE) {
        printf("CheckHiLogPrint command generate snprintf_s failed\n");
        return false;
    }
    finalCommand[READ_SIZE] = '\0';
    char buffer[BUFFER_SIZE];
    FILE* pipe;
    pipe = popen(finalCommand, "r");
    if (pipe == NULL) {
        printf("CheckHiLogPrint: Failed to run command\n");
        return false;
    }

    // 读取命令输出并打印
    while (fgets(buffer, BUFFER_SIZE, pipe) != NULL) {
        printf("%s", buffer);
        flag = true;
    }

    // 关闭管道并获取返回值
    int returnValue = pclose(pipe);
    if (returnValue == FAIL_CLOSE) {
        printf("CheckHiLogPrint pclose failed returnValue=-1 errno=%d\n", errno);
    }
    return flag;
}

// 子线程打印日志执行的函数
void* FunctionPrintLog(void* arg)
{
    int index = (int)arg;
    int ret = HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG,
        "FunctionPrintLog %{public}s", str[index]);
    return NULL;
}
/**
 * @tc.name     : HiLogAdapterPrint_0010
 * @tc.desc     : Test HiLogAdapterPrint after musl_log_reset
 * @tc.level    : Level 2
 */
static void HiLogAdapterPrint_0010(void)
{
    musl_log_reset();
    int ret = HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, PRINT_STRING_A);
    EXPECT_NE("HiLogAdapterPrint_0010", ret, 0);
    // 同时也要查看hilog
    EXPECT_EQ("HiLogAdapterPrint_0010", CheckHiLogPrint(PRINT_STRING_A), true);
    ret = HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_INFO, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, PRINT_STRING_B);
    EXPECT_EQ("HiLogAdapterPrint_0010", ret, -1);
}

/**
 * @tc.name     : HilogAdapterPrint_0020
 * @tc.desc     : Test single thread HilogAdapterPrint after hilog fd close
 * @tc.level    : Level 2
 */
static void HilogAdapterPrint_0020(void)
{
    musl_log_reset();
    // We need to refresh the hilog socket to close previous socket
    RefreshHiLogSocketFd();
    // 前置条件：校验g_socketFd无效
    EXPECT_EQ("HilogAdapterPrint_0020 CheckHilogInvalid", CheckHilogValid(), ZERO);
    int ret = HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, PRINT_STRING_C);
    // 检查g_socketFd有效
    EXPECT_NE("HilogAdapterPrint_0020 CheckHilogValid", CheckHilogValid(), ZERO);
    // 检查write正常
    EXPECT_GT("HiLogAdapterPrint_0020", ret, 0);
    bool result = CheckHiLogPrint(PRINT_STRING_C);
    // 检查有日志生成
    EXPECT_EQ("HilogAdapterPrint_0020_CheckHiLogPrint", result, true);
}

/**
 * @tc.name     : HilogAdapterPrint_0030
 * @tc.desc     : dls3阶段初始化异常时，线程自己初始化成功，
 *     单线程：预期是能够替换g_socketFd，并能够正常打印日志
 *     多线程：预期是能够替换g_socketFd，并能够正常打印日志，且fd未发生泄漏
 * @tc.level    : Level 2
 */
static void HilogAdapterPrint_0030(void)
{
    musl_log_reset();
    // 关闭socketFd
    RefreshHiLogSocketFd();
    // 前置条件：校验g_socketFd无效
    EXPECT_EQ("HilogAdapterPrint_0030 CheckHilogInvalid", CheckHilogValid(), ZERO);
    pthread_t threads[THREAD_COUNT];
    // 创建线程并发打印，每个线程打印的日志都不能丢
    for (int i = ZERO; i < THREAD_COUNT; i++) {
        // 生成一个随机字符串，然后将下标通过值传递给子线程，让子线程拿到全局的数据
        GenerateRandomString(str[i], i);
        if (pthread_create(&threads[i], NULL, FunctionPrintLog, (void *)i) != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i);
        }
    }
    // 检查g_socketFd有效
    EXPECT_NE("HilogAdapterPrint_0030 CheckHilogValid", CheckHilogValid(), ZERO);
    // 等待线程执行完毕
    for (int i = ZERO; i < THREAD_COUNT; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread %d\n", i);
        } else {
            // 在这里判断当前日志是否打印成功
            bool result = CheckHiLogPrint(str[i]);
            EXPECT_EQ("HilogAdapterPrint_0030_CheckHiLogPrint", result, true);
        }
    }
}

int main(void)
{
    // 解除限制
    system("/bin/hilog -Q pidoff");
    system("/bin/hilog -Q domainoff");

    system(ENABLE_LOG);
    system(LOG_LEVEL_ERROR);
    HiLogAdapterPrint_0010();
    HilogAdapterPrint_0020();
    HilogAdapterPrint_0030();

    // 恢复限制
    system("/bin/hilog -Q pidon");
    system("/bin/hilog -Q domainon");
	return t_status;
}
