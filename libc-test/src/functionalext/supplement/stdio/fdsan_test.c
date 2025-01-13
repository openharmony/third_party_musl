/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <musl_log.h>
#include <atomic.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#include "functionalext.h"

#define DEV_NULL_PATH "/dev/null"
#define DEBUG_SIGNAL (42)
#define ONE (1)
#define ZERO (0)
#define TWO (2)
#define THREE (3)
#define DIRECTORY_PATH "/data/log/faultlog/temp"
#define FDSAN_SEARCH_STRING "DEBUG SIGNAL(FDSAN)"
#define BADFD_SEARCH_STRING "EBADF:"

#define TEMP_FD_0 (0)
#define TEMP_FD_1 (1)
#define TEMP_FD_2 (2)
#define TEMP_FD_3 (3)
#define TEMP_FD_4 (4)
#define TEMP_FD_5 (5)
#define TEMP_FD_6 (6)
#define INVALID_TAG_999 (999)
#define INVALID_TAG_998 (998)
#define TEMP_TAG_99 (99)

// 子线程函数，向主线程发送信号，触发fdsan校验失败循环三次
void* thread_func(void* arg)
{
    for (int i = ZERO; i < THREE; i++) {
        fdsan_exchange_owner_tag((int)arg, INVALID_TAG_999, INVALID_TAG_998);
    }
    return NULL;
}

#define MAX_PATH_LENGTH (1024)

#define NOT_FOUND (0)
#define FOUND (1)
// 函数：检查文件中是否包含目标字符串
int contains_string(FILE *file, const char *search_string)
{
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, search_string) != NULL) {
            return FOUND;  // 找到目标字符串
        }
    }
    return NOT_FOUND;  // 没有找到
}

// 函数：遍历目录，检查文件
int count_string_in_directory(const char *dir_path, const char *search_string1, const char *search_string2)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        t_error("can not open dir");
        return -1;
    }

    struct dirent *entry;
    int count = ZERO;

    // 遍历目录中的每个文件
    while ((entry = readdir(dir)) != NULL) {
        // 跳过 "." 和 ".."
        if (strcmp(entry->d_name, ".") == ZERO || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 构造文件的完整路径
        char file_path[MAX_PATH_LENGTH];
        (void)snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

        // 判断是否为文件（可以进一步扩展检查是否为普通文件）
        FILE *file = fopen(file_path, "r");
        if (file != NULL) {
            // 如果文件中包含目标字符串，计数+1
            if (contains_string(file, search_string1) && contains_string(file, search_string2)) {
                count++;
            }
            fclose(file);
        }
    }

    closedir(dir);
    return count;
}

void wait_until_signal(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, DEBUG_SIGNAL);
    // 设置超时结构，设置超时为 0.5 秒
    struct timespec timeout;
    timeout.tv_sec = 0;          // 秒部分为 0
    timeout.tv_nsec = 500000000; // 纳秒部分为 500000000，即 0.5 秒
    (void)sigtimedwait(&set, NULL, &timeout);
}

#define TEMP_LENGTH (256)
#define RIGHT_RESULT (1)

void check_same_fd_same_time_fdsan_signal_once(void)
{
    (void)system("rm -rf /data/log/faultlog/temp/*");
    // 创建两个线程
    pthread_t thread1, thread2;
    int result = pthread_create(&thread1, NULL, thread_func, (void *)TEMP_FD_0);
    if (result != 0) {
        t_error("check_fdsan_signal_once pthread_create1 failed result=%d\n", result);
        return;
    }
    result = pthread_create(&thread2, NULL, thread_func, (void *)TEMP_FD_0);
    if (result != 0) {
        t_error("check_fdsan_signal_once pthread_create2 failed result=%d\n", result);
        return;
    }

    // 主线程等待子线程执行完
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 获取当前进程的 PID
    pid_t pid = getpid();
    
    // 用于存储拼接后的字符串
    char search_string[TEMP_LENGTH];
    
    // 使用 sprintf 将 PID 转换为字符串并与其他内容拼接
    (void)sprintf(search_string, "Pid:%d", pid);

    wait_until_signal();
    // 检查生成了几个文件
    result = count_string_in_directory(DIRECTORY_PATH, search_string, FDSAN_SEARCH_STRING);
    if (result != RIGHT_RESULT) {
        t_error("The target string '%s' appears in %d files\n", search_string, result);
    }
}

void check_same_fd_diff_time_fdsan_signal_once(void)
{
    (void)system("rm -rf /data/log/faultlog/temp/*");
    fdsan_exchange_owner_tag(TEMP_FD_1, INVALID_TAG_999, INVALID_TAG_998);
    fdsan_exchange_owner_tag(TEMP_FD_1, INVALID_TAG_999, INVALID_TAG_998);

    // 获取当前进程的 PID
    pid_t pid = getpid();
    
    // 用于存储拼接后的字符串
    char search_string[TEMP_LENGTH];
    
    // 使用 sprintf 将 PID 转换为字符串并与其他内容拼接
    (void)sprintf(search_string, "Pid:%d", pid);

    wait_until_signal();
    // 检查生成了几个文件
    int result = count_string_in_directory(DIRECTORY_PATH, search_string, FDSAN_SEARCH_STRING);
    if (result != RIGHT_RESULT) {
        t_error("The target string '%s' appears in %d files\n", search_string, result);
    }
}

void check_diff_fd_same_time_fdsan_signal_twice(void)
{
    (void)system("rm -rf /data/log/faultlog/temp/*");
    // 创建两个线程
    pthread_t thread1, thread2;
    int result = pthread_create(&thread1, NULL, thread_func, (void *)TEMP_FD_2);
    if (result != 0) {
        t_error("check_fdsan_signal_once pthread_create1 failed result=%d\n", result);
        return;
    }
    result = pthread_create(&thread2, NULL, thread_func, (void *)TEMP_FD_3);
    if (result != 0) {
        t_error("check_fdsan_signal_once pthread_create2 failed result=%d\n", result);
        return;
    }

    // 主线程等待子线程执行完
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 获取当前进程的 PID
    pid_t pid = getpid();
    
    // 用于存储拼接后的字符串
    char search_string[TEMP_LENGTH];
    
    // 使用 sprintf 将 PID 转换为字符串并与其他内容拼接
    (void)sprintf(search_string, "Pid:%d", pid);

    wait_until_signal();
    // 检查生成了几个文件
    result = count_string_in_directory(DIRECTORY_PATH, search_string, FDSAN_SEARCH_STRING);
    if (result != TWO) {
        t_error("The target string '%s' appears in %d files\n", search_string, result);
    }
}

void check_diff_fd_diff_time_fdsan_signal_twice(void)
{
    (void)system("rm -rf /data/log/faultlog/temp/*");
    fdsan_exchange_owner_tag(TEMP_FD_4, INVALID_TAG_999, INVALID_TAG_998);
    fdsan_exchange_owner_tag(TEMP_FD_5, INVALID_TAG_999, INVALID_TAG_998);

    // 获取当前进程的 PID
    pid_t pid = getpid();
    
    // 用于存储拼接后的字符串
    char search_string[TEMP_LENGTH];
    
    // 使用 sprintf 将 PID 转换为字符串并与其他内容拼接
    (void)sprintf(search_string, "Pid:%d", pid);

    wait_until_signal();
    // 检查生成了几个文件
    int result = count_string_in_directory(DIRECTORY_PATH, search_string, FDSAN_SEARCH_STRING);
    if (result != TWO) {
        t_error("The target string '%s' appears in %d files\n", search_string, result);
    }
}

void check_bad_fd(void)
{
    (void)system("rm -rf /data/log/faultlog/temp/*");
    int fd = open("/data/local/tmp/newfile.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        t_error("check_bad_fd open failed errno=%d\n", errno);
        return;
    }
    fdsan_exchange_owner_tag(fd, ZERO, TEMP_TAG_99);
    fdsan_close_with_tag(fd, TEMP_TAG_99);
    fdsan_close_with_tag(fd, TEMP_TAG_99);
    fdsan_close_with_tag(fd, TEMP_TAG_99);

    // 获取当前进程的 PID
    pid_t pid = getpid();

    // 用于存储拼接后的字符串
    char search_string[TEMP_LENGTH];
    
    // 使用 sprintf 将 PID 转换为字符串并与其他内容拼接
    (void)sprintf(search_string, "Pid:%d", pid);

    wait_until_signal();
    // 检查生成了几个文件
    int result = count_string_in_directory(DIRECTORY_PATH, search_string, BADFD_SEARCH_STRING);
    if (result == ZERO) {
        t_error("The target string '%s' appears in %d files\n", search_string, result);
    }
}

int main(void)
{
    FILE *fp = popen("uname", "r");
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("uname result %s", buffer);
        // jump over this testcase
        if (strstr(buffer, "Linux")) {
            return ZERO;
        }
    }
    // 1. 检查同fd，两线程同时触发校验，生成几次报告 预期1次
    check_same_fd_same_time_fdsan_signal_once();
    // 2. 检查同fd，前后触发校验，生成几次报告 预期1次
    check_same_fd_diff_time_fdsan_signal_once();
    // 3. 检查不同fd，同时触发校验，生成几次报告 预期2次
    check_diff_fd_same_time_fdsan_signal_twice();
    // 4. 检查不同fd，前后触发校验，生成几次报告 预期2次
    check_diff_fd_diff_time_fdsan_signal_twice();
    // 5. 检查EBADF触发42信号是否不受影响
    check_bad_fd();
    return t_status;
}
