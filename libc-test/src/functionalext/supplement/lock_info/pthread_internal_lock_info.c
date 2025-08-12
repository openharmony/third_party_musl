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
#include <signal.h>

#define NEGATIVE_ONE (-1)
#define ZERO (0)

void WaitUntilSignal(int signum)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, signum);
    // 设置超时结构，设置超时为 0.5 秒
    struct timespec timeout;
    timeout.tv_sec = 0;          // 秒部分为 0
    timeout.tv_nsec = 500000000; // 纳秒部分为 500000000，即 0.5 秒
    (void)sigtimedwait(&set, NULL, &timeout);
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

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define FREEZE_SIGNAL_35 (35)

void PthreadInternalLockInfoTest(void)
{
    // 发送信号35和40到自己
    int result = raise(FREEZE_SIGNAL_35);
    if (result != ZERO) {
        printf("raise 35 failed errno=%d\n", errno);
        return;
    }
    WaitUntilSignal(FREEZE_SIGNAL_35);
    bool hilogResult = CheckHiLogPrint("FREEZE_signo_35");
    printf("hilogResult=%d\n", hilogResult);
    hilogResult = CheckHiLogPrint("tl_lock_count");
    printf("hilogResult=%d\n", hilogResult);
    hilogResult = CheckHiLogPrint("tl_lock_waiters");
    printf("hilogResult=%d\n", hilogResult);
    hilogResult = CheckHiLogPrint("tl_lock_tid_fail");
    printf("hilogResult=%d\n", hilogResult);
    hilogResult = CheckHiLogPrint("tl_lock_count_tid");
    printf("hilogResult=%d\n", hilogResult);
}

int main(void)
{
    // 解除限制
    system("/bin/hilog -Q pidoff");
    system("/bin/hilog -Q domainoff");
    system("param set musl.log.level WARN");
    PthreadInternalLockInfoTest();
    // 恢复限制
    system("/bin/hilog -Q pidon");
    system("/bin/hilog -Q domainon");
    return t_status;
}