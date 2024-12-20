/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <string>
#include <unistd.h>

extern "C" {
    #include "test.h"
}

#define FAULTLOG_DIR "/data/log/faultlog/faultlogger/"
#define UBSAN_LOG_TAG "ubsan"
#define DEBUG 0
#define BUFFER_SIZE 4096

static void ClearCfiLog(const char *log_tag, const char *log_dir);
static void FindAndCheck(const char *pattern, const char *log_tag, const char *log_dir);

static void ShowCfiLogFile()
{
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(FAULTLOG_DIR);
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, UBSAN_LOG_TAG) != NULL) {
            printf("%s: %s\n", FAULTLOG_DIR, ptr->d_name);
        }
    }
    closedir(dir);
}

static void ClearCfiLog()
{
    ClearCfiLog(UBSAN_LOG_TAG, FAULTLOG_DIR);
}

static void ClearCfiLog(const char *log_tag, const char *log_dir)
{
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(log_dir);
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, log_tag) != NULL) {
            char tmp[BUFFER_SIZE];
            snprintf(tmp, BUFFER_SIZE, "%s/%s", log_dir, ptr->d_name);
            remove(tmp);
        }
    }
    closedir(dir);
}

static void CheckCfiLog(char *file, const char *needle)
{
    if (DEBUG) {
        printf("[cfi checking]:%s\n", file);
    }

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
    
    if (buffer.find(needle) != std::string::npos) {
        printf("[cfi checking] %s is ok.\n", needle);
    } else {
        t_error("FAIL %s is failed!\n", needle);
    }

    fclose(fp);
}

template<typename CallbackT>
static void FindDirAndCheck(DIR *dir, CallbackT &&callback)
{
    FindDirAndCheck(dir, UBSAN_LOG_TAG, FAULTLOG_DIR, callback);
}

template<typename CallbackT>
static void FindDirAndCheck(DIR *dir, const char *log_tag, const char *log_dir, CallbackT &&callback)
{
    sleep(1);
    struct dirent *ptr;
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, log_tag) != NULL) {
            char tmp[BUFFER_SIZE];
            snprintf(tmp, BUFFER_SIZE, "%s/%s", log_dir, ptr->d_name);
            callback(tmp);
        }
    }
}

static void FindAndCheck(const char *pattern)
{
    DIR *faultlogDir = opendir(FAULTLOG_DIR);
    auto callback = [=](char *file) { CheckCfiLog(file, pattern); };
    FindDirAndCheck(faultlogDir, callback);
    closedir(faultlogDir);
}

static void FindAndCheck(const char *pattern, const char *log_tag, const char *log_dir)
{
    DIR *faultlogDir = opendir(log_dir);
    auto callback = [=](char *file) { CheckCfiLog(file, pattern); };
    FindDirAndCheck(faultlogDir, log_tag, log_dir, callback);
    closedir(faultlogDir);
}

static void ExpectCfiOk()
{
    DIR *faultlogDir = opendir(FAULTLOG_DIR);
    auto callback = [](char *file) { t_error("FAIL CFI check failed!\n"); };
    FindDirAndCheck(faultlogDir, callback);
    closedir(faultlogDir);
}
