/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include <log_base.h>
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
#include <hilog_common.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <dirent.h>
#include <securec.h>

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD003F07

#undef LOG_TAG
#define LOG_TAG "HILOGBASETEST"

#define SOME_LOGS 10
#define MORE_LOGS 100
#define SHORT_LOG 16
#define LONG_LOG 1000
#define NEGATIVE_ONE (-1)
#define DEFAULT_BUF_SIZE 256

typedef enum {
    DEBUG_METHOD = 0,
    INFO_METHOD = 1,
    WARN_METHOD = 2,
    ERROR_METHOD = 3,
    FATAL_METHOD = 4,
    METHODS_NUMBER = 5
} LogInterfaceType;

typedef void (*LogMethodFunc)(const char* msg);

static const char* METHOD_NAMES[METHODS_NUMBER] = {
    "Debug", "Info", "Warn", "Error", "Fatal"
};

static void HilogBaseDebug(const char* msg)
{
    HILOG_BASE_DEBUG(LOG_CORE, "%{public}s", msg);
}

static void HilogBaseInfo(const char* msg)
{
    HILOG_BASE_INFO(LOG_CORE, "%{public}s", msg);
}

static void HilogBaseWarn(const char* msg)
{
    HILOG_BASE_WARN(LOG_CORE, "%{public}s", msg);
}

static void HilogBaseError(const char* msg)
{
    HILOG_BASE_ERROR(LOG_CORE, "%{public}s", msg);
}

static void HilogBaseFatal(const char* msg)
{
    HILOG_BASE_FATAL(LOG_CORE, "%{public}s", msg);
}

static LogMethodFunc LOG_C_METHODS[METHODS_NUMBER] = {
    HilogBaseDebug,
    HilogBaseInfo,
    HilogBaseWarn,
    HilogBaseError,
    HilogBaseFatal
};

char* PopenToString(const char *command)
{
    if (command == NULL) {
        return NULL;
    }
    FILE *fp = popen(command, "re");
    if (!fp) {
        return NULL;
    }

    char *buf = (char*)malloc(DEFAULT_BUF_SIZE);
    if (!buf) {
        pclose(fp);
        return NULL;
    }

    size_t bufSize = DEFAULT_BUF_SIZE;
    size_t totalRead = 0;
    size_t chunkSize = 0;

    while ((chunkSize = fread(buf + totalRead, 1, bufSize - totalRead -1, fp)) > 0) {
        totalRead += chunkSize;

        // 如果缓冲区满了，需要扩容
        if (totalRead >= bufSize - 1) {
            size_t newSize = bufSize * 2; // 2 : 扩容两倍
            char *newBuf = (char*)malloc(newSize);
            if (!newBuf) {
                free(buf);
                pclose(fp);
                return NULL;
            }

            if (memcpy_s(newBuf, newSize, buf, totalRead) != 0) {
                free(newBuf);
                free(buf);
                pclose(fp);
                return NULL;
            }

            free(buf);
            buf = newBuf; //更新指针和大小
            bufSize = newSize;
        }
    }

    //确保字符串以\0结尾
    if (totalRead < bufSize - 1) {
        buf[totalRead] = '\0';
    } else {
        // 如果刚好填满，也需要加终止符
        buf[bufSize - 1] = '\0';
    }
    pclose(fp);
    return buf;
}

char* RandomStringGenerator(uint32_t len)
{
    char* str = (char*)malloc(len + 1);
    if (str == NULL) {
        return NULL; // 内存分配失败
    }

    int fd = open("/dev/random", O_RDONLY);
    if (fd == NEGATIVE_ONE) {
        free(str);
        return NULL;
    }
    // 读取len个随机字节
    if (read(fd, str, len) != len) {
        close(fd);
        free(str);
        return NULL;
    }

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    // 将随机字节映射到charset的范围
    for (uint32_t i = 0; i < len; ++i) {
        str[i] = charset[str[i] % (sizeof(charset) - 1)];
    }
    
    str[len] = '\0';
    close(fd);
    return str;
}

void HiLogWriteTest(char* funcName, LogMethodFunc loggingMethod, uint32_t logCount, uint32_t logLen, bool isDebug) 
{
    char* logMsg = RandomStringGenerator(logLen);
    if (!logMsg) {
        t_error("Faild to generate random string\n");
        return;
    }
    for (uint32_t i = 0; i < logCount; ++i) {
        // 创建带序号的日志消息
        char fullMsg[MAX_LOG_LEN];
        if (snprintf_s(fullMsg, MAX_LOG_LEN, MAX_LOG_LEN - 1, "%s%u", logMsg, i) < 0) {
            t_error("logMsg exceeds MAX_LOG_LEN");
            free(logMsg);
            return;
        }
        loggingMethod(fullMsg);
    }
    
    if (strlen(logMsg) > MAX_LOG_LEN-1) {
        logMsg[MAX_LOG_LEN-1] = '\0'; // 直接截断字符串
    }
    
    usleep(1000); /* 1000: 睡眠 1 毫秒 */
    
    char* cmd = "/system/bin/hilog -x";
    char* logMsgs = PopenToString(cmd);
    if (!logMsgs) {
        t_error("Failed to execute command: %s\n", cmd);
        free(logMsg);
        return;
    }
    uint32_t realCount = 0;
    // 处理字符串分割
    char* line = strtok(logMsgs, "\n");
    while (line != NULL) {
        if (strstr(line, logMsg) != NULL) {
            ++realCount;
        }
        line = strtok(NULL, "\n");
    }
    
    uint32_t allowedLeastLogCount = logCount - logCount / 10; /* 1 / 10: 丢失率少于10% */
    if (isDebug) {
        allowedLeastLogCount = 0; /* 0: 调试模式关闭统计 */
    }

    // 释放内存
    free(logMsg);
    free(logMsgs);
    
    EXPECT_GTE(funcName, realCount, allowedLeastLogCount);
}

/**
 * @tc.name     : HiLogBasePrint_0010
 * @tc.desc     : Test HiLogBasePrint for some logs
 * @tc.level    : Level 2
 */
static void HiLogBasePrint_0010(void)
{
    for(uint32_t i = 0; i < METHODS_NUMBER; ++i) {
        printf("Starting %s test\n", METHOD_NAMES[i]);
        HiLogWriteTest("HiLogBasePrint_0010", LOG_C_METHODS[i], SOME_LOGS, SHORT_LOG, i == DEBUG_METHOD);
    }
}

/**
 * @tc.name     : HiLogBasePrint_0020
 * @tc.desc     : Test HiLogBasePrint for more logs
 * @tc.level    : Level 2
 */
static void HiLogBasePrint_0020(void)
{
    for(uint32_t i = 0; i < METHODS_NUMBER; ++i) {
        printf("Starting %s test\n", METHOD_NAMES[i]);
        HiLogWriteTest("HiLogBasePrint_0020", LOG_C_METHODS[i], MORE_LOGS, SHORT_LOG, i == DEBUG_METHOD);
    }
}

/**
 * @tc.name     : HiLogBasePrint_0030
 * @tc.desc     : Test HiLogBasePrint for long logs
 * @tc.level    : Level 2
 */
static void HiLogBasePrint_0030(void)
{
    for(uint32_t i = 0; i < METHODS_NUMBER; ++i) {
        printf("Starting %s test\n", METHOD_NAMES[i]);
        HiLogWriteTest("HiLogBasePrint_0030", LOG_C_METHODS[i], LONG_LOG, SHORT_LOG, i == DEBUG_METHOD);
    }
}

/**
 * @tc.name     : HiLogBaseIsLoggable_0010
 * @tc.desc     : Test HiLogBaseIsLoggable for long logs
 * @tc.level    : Level 2
 */
static void HiLogBaseIsLoggable_0010(void)
{
    EXPECT_TRUE("HiLogBaseIsLoggable LOG_DEBUG", HiLogBaseIsLoggable(LOG_DOMAIN, LOG_TAG, LOG_DEBUG));
    EXPECT_TRUE("HiLogBaseIsLoggable LOG_INFO", HiLogBaseIsLoggable(LOG_DOMAIN, LOG_TAG, LOG_INFO));
    EXPECT_TRUE("HiLogBaseIsLoggable LOG_WARN", HiLogBaseIsLoggable(LOG_DOMAIN, LOG_TAG, LOG_WARN));
    EXPECT_TRUE("HiLogBaseIsLoggable LOG_ERROR", HiLogBaseIsLoggable(LOG_DOMAIN, LOG_TAG, LOG_ERROR));
    EXPECT_TRUE("HiLogBaseIsLoggable LOG_FATAL", HiLogBaseIsLoggable(LOG_DOMAIN, LOG_TAG, LOG_FATAL));
    EXPECT_TRUE("HiLogBaseIsLoggable LOG_WARN", HiLogBaseIsLoggable(LOG_DOMAIN, "abc", LOG_WARN));
    EXPECT_FALSE("HiLogBaseIsLoggable LOG_LEVEL_MIN", HiLogBaseIsLoggable(LOG_DOMAIN, "abc", LOG_LEVEL_MIN));
}

int main(void)
{
    // 解除限制
    system("/bin/hilog -Q pidoff");
    system("/bin/hilog -Q domainoff");

    HiLogBasePrint_0010();
    HiLogBasePrint_0020();
    HiLogBasePrint_0030();
    HiLogBaseIsLoggable_0010();

    // 恢复限制
    system("/bin/hilog -Q pidon");
    system("/bin/hilog -Q domainon");
	return t_status;
}
