/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
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
extern "C" {
    #include "../../common/test.h"
}
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <string>
#include <unistd.h>

constexpr int MAX_BUFFER_SIZE = 128;
constexpr const char *FAULTLOG_DIR = "/data/log/faultlog/faultlogger/";
constexpr const char *LOG_TAG = "ohos_dfx_log";

extern "C" __attribute__((weak)) int ohos_dfx_log(const char *s);

static void ClearDfxLogs()
{
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(FAULTLOG_DIR);
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, LOG_TAG) != NULL) {
            char tmp[MAX_BUFFER_SIZE];
            snprintf(tmp, MAX_BUFFER_SIZE, "%s/%s", FAULTLOG_DIR, ptr->d_name);
            remove(tmp);
        }
    }
    closedir(dir);
}

static void CheckLogContent(char *file, const char *content)
{
    FILE *fp = fopen(file, "r");
    if (!fp) {
        return;
    }
    if (fseek(fp, 0, SEEK_END) == -1) {
        return;
    }
    int size = ftell(fp);
    if (size <= 0) {
        fclose(fp);
        t_error("FAIL %s size is <=0!\n", file);
    }
    
    std::string buffer;
    buffer.resize(size);

    if (fseek(fp, 0, SEEK_SET) == -1) {
        fclose(fp);
        return;
    }
    int rsize = fread(&buffer[0], 1, size, fp);
    if (rsize == 0) {
        fclose(fp);
        return;
    }
    
    if (buffer.find(content) != std::string::npos) {
        printf("[ohos_dfx_log] check pass:\n%s\n", content);
    } else {
        t_error("[ohos_dfx_log] failed:\n%s\n", content);
    }

    fclose(fp);
}

static void CheckLog(const char *content)
{
    sleep(1);
    DIR *faultlogDir = opendir(FAULTLOG_DIR);
    struct dirent *ptr;
    while ((ptr = readdir(faultlogDir)) != NULL) {
        if (strstr(ptr->d_name, LOG_TAG) != NULL) {
            char tmp[MAX_BUFFER_SIZE];
            snprintf(tmp, MAX_BUFFER_SIZE, "%s/%s", FAULTLOG_DIR, ptr->d_name);
            CheckLogContent(tmp, content);
            break;
        }
    }
    closedir(faultlogDir);
}

void TestAsanLog()
{
    ClearDfxLogs();
    if (&ohos_dfx_log) {
        ohos_dfx_log("[ohos_dfx_log] output something to the log path.\n");
        ohos_dfx_log("End Asan report");
        CheckLog("[ohos_dfx_log] output something to the log path.\nEnd Asan report");
    } else {
        t_error("[ohos_dfx_log] cannot find ohos_dfx_log");
    }
}

void TestHWAsanLog()
{
    ClearDfxLogs();
    if (&ohos_dfx_log) {
        ohos_dfx_log("[ohos_dfx_log] output something to the log path.\n");
        ohos_dfx_log("End Hwasan report");
        CheckLog("[ohos_dfx_log] output something to the log path.\nEnd Hwasan report");
    } else {
        t_error("[ohos_dfx_log] cannot find ohos_dfx_log");
    }
}

void TestTsanLog()
{
    ClearDfxLogs();
    if (&ohos_dfx_log) {
        ohos_dfx_log("[ohos_dfx_log] output something to the log path.\n");
        ohos_dfx_log("End Tsan report");
        CheckLog("[ohos_dfx_log] output something to the log path.\nEnd Tsan report");
    } else {
        t_error("[ohos_dfx_log] cannot find ohos_dfx_log");
    }
}

void TestUbsanLog()
{
    ClearDfxLogs();
    if (&ohos_dfx_log) {
        ohos_dfx_log("[ohos_dfx_log] output something to the log path.\n");
        ohos_dfx_log("End Ubsan report");
        CheckLog("[ohos_dfx_log] output something to the log path.\nEnd Ubsan report");
    } else {
        t_error("[ohos_dfx_log] cannot find ohos_dfx_log");
    }
}

void TestCfiLog()
{
    ClearDfxLogs();
    if (&ohos_dfx_log) {
        ohos_dfx_log("[ohos_dfx_log] output something to the log path.\n");
        ohos_dfx_log("End CFI report");
        CheckLog("[ohos_dfx_log] output something to the log path.\nEnd CFI report");
    } else {
        t_error("[ohos_dfx_log] cannot find ohos_dfx_log");
    }
}

void TestBufferExpand()
{
    ClearDfxLogs();
    if (&ohos_dfx_log) {
        for (int i = 0; i < 5000; ++i) {
            ohos_dfx_log("ohos_dfx_log output something to the log path ohos_dfx_log output something to the log path.\n");
        }
        ohos_dfx_log("End Asan report");
        CheckLog("End Asan report");
    } else {
        t_error("[ohos_dfx_log] cannot find ohos_dfx_log");
    }
}

int main()
{
    TestAsanLog();
    TestHWAsanLog();
    TestTsanLog();
    TestUbsanLog();
    TestCfiLog();
    TestBufferExpand();
    return t_status;
}