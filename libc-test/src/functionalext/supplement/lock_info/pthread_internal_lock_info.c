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

#define NEGATIVE_ONE (-1)
#define ZERO (0)

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
        t_error("CheckHiLogPrint command generate snprintf_s failed\n");
        return false;
    }
    finalCommand[READ_SIZE] = '\0';
    char buffer[BUFFER_SIZE];
    FILE* pipe;
    pipe = popen(finalCommand, "r");
    if (pipe == NULL) {
        t_error("CheckHiLogPrint: Failed to run command\n");
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
        t_error("CheckHiLogPrint pclose failed returnValue=-1 errno=%d\n", errno);
    }
    return flag;
}

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// 不执行任何操作的信号处理函数
void ignore_signal(int signum)
{
    // 什么都不做
}

#define FREEZE_SIGNAL_35 (35)
#define INVALID_SIGNAL_40 (40)

void PthreadInternalLockInfoTest(void)
{
    // 发送信号35, 38和40到自己
    sighandler_t sigResult = signal(FREEZE_SIGNAL_35, ignore_signal);
    if (sigResult == SIG_ERR) {
        t_error("signal 35 failed errno=%d", errno);
        return;
    }
    int result = raise(FREEZE_SIGNAL_35);
    if (result != ZERO) {
        t_error("raise 35 failed errno=%d", errno);
        return;
    }
    bool hilogResult = CheckHiLogPrint("FREEZE_signo_35");
    EXPECT_EQ("signal35_check_output", hilogResult, true);

    // 在发送信号40之前，先将它的处理函数设置为不做任何操作
    sigResult = signal(INVALID_SIGNAL_40, ignore_signal);
    if (sigResult == SIG_ERR) {
        t_error("signal 40 failed errno=%d", errno);
    }
    result = raise(INVALID_SIGNAL_40);
    if (result != ZERO) {
        t_error("raise 40 failed errno=%d", errno);
        return;
    }
    hilogResult = CheckHiLogPrint("FREEZE_signo_40");
    EXPECT_EQ("signal38_check_output", hilogResult, false);
}

int main(void)
{
    // 解除限制
    system("/bin/hilog -Q pidoff");
    system("/bin/hilog -Q domainoff");
    PthreadInternalLockInfoTest();
    // 恢复限制
    system("/bin/hilog -Q pidon");
    system("/bin/hilog -Q domainon");
    return t_status;
}