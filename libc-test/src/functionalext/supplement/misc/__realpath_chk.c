/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include "functionalext.h"

static void *__realpath_chk_thread_func(void *arg)
{
    (void)arg;
    char buffer[PATH_MAX];
    
    for (int i = 0; i < 100; i++) {
        errno = 0;
        char *ret = __realpath_chk(".", buffer, sizeof(buffer));
        if (errno) {
            printf("Thread error: errno = %d\n", errno);
        }
        
        if (ret == NULL || errno != 0) {
            return (void*)(long)1;
        }
        
        if (access(ret, F_OK) != 0) {
            return (void*)(long)1;
        }
    }
    
    return NULL;
}

/**
 * @tc.name      : __realpath_chk_0100
 * @tc.desc      : Verify NULL resolved buffer delegates to realpath
 * @tc.level     : Level 0
 */
void __realpath_chk_0100(void)
{
    errno = 0;
    char *ret = __realpath_chk(".", NULL, PATH_MAX);

    EXPECT_TRUE(__func__, ret != NULL);

    if (ret != NULL) {
        free(ret);
    }

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __realpath_chk_0200
 * @tc.desc      : Verify Fortify protection triggers abort when buffer too small
 * @tc.level     : Level 2
 */
void __realpath_chk_0200(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        char buffer[10];
        __realpath_chk(".", buffer, sizeof(buffer));
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __realpath_chk_0300
 * @tc.desc      : Verify valid path with sufficient buffer size works correctly
 * @tc.level     : Level 0
 */
void __realpath_chk_0300(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk(".", buffer, sizeof(buffer));
    EXPECT_TRUE(__func__, ret != NULL && ret == buffer);

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __realpath_chk_0400
 * @tc.desc      : NULL path parameter
 * @tc.level     : Level 2
 */
void __realpath_chk_0400(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk(NULL, buffer, sizeof(buffer));
    if(errno != EINVAL) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, EINVAL);
        EXPECT_TRUE(__func__, ret == NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_0500
 * @tc.desc      : Non-existent path parameter
 * @tc.level     : Level 1
 */
void __realpath_chk_0500(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk("/path/does/not/exist", buffer, sizeof(buffer));
    if(errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, ENOENT);
        EXPECT_TRUE(__func__, ret == NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_0600
 * @tc.desc      : resolvedlen equals 0
 * @tc.level     : Level 2
 */
void __realpath_chk_0600(void)
{

    char buffer[PATH_MAX];
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        errno = 0;
        (void)__realpath_chk(".", buffer, 0);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**  
 * @tc.name      : __realpath_chk_0700
 * @tc.desc      : resolvedlen smaller than actual path length
 * @tc.level     : Level 1
 */
void __realpath_chk_0700(void)
{
    char buffer[PATH_MAX];
    const size_t MINIMAL_BUFFER_SIZE = 1;
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        (void)__realpath_chk(".", buffer, MINIMAL_BUFFER_SIZE); 
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __realpath_chk_0800
 * @tc.desc      : resolvedlen equals actual path length
 * @tc.level     : Level 0
 */
void __realpath_chk_0800(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk(".", buffer, PATH_MAX);

    if(errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }
}

/** 
 * @tc.name      : __realpath_chk_0900
 * @tc.desc      : resolvedlen larger than actual path length
 * @tc.level     : Level 0
 */
void __realpath_chk_0900(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk(".", buffer, sizeof(buffer)+1);

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_1000
 * @tc.desc      : Small stack buffer protection
 * @tc.level     : Level 2
 */
void __realpath_chk_1000(void)
{
    char buf[16]; 
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        (void)__realpath_chk(".", buf, sizeof(buf));
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __realpath_chk_1100
 * @tc.desc      : Symbolic link path resolution
 * @tc.level     : Level 1
 */
void __realpath_chk_1100(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    const char *target = ".";
    char link_path[] = "./test_link";

    if (symlink(target, link_path) != 0) {
        t_error("%s: symlink creation failed (errno=%d), skipping symlink test\n", __func__, errno);
        return;
    }

    char *ret = __realpath_chk(link_path, buffer, sizeof(buffer));

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }

    unlink(link_path);
}

/**
 * @tc.name      : __realpath_chk_1200
 * @tc.desc      : Path resolution with '.' and '..' components
 * @tc.level     : Level 1
 */
void __realpath_chk_1200(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk("././..", buffer, sizeof(buffer));

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_1300
 * @tc.desc      : errno preservation behavior
 * @tc.level     : Level 1
 */
void __realpath_chk_1300(void)
{
    errno = EAGAIN;
    char buffer[PATH_MAX];
    __realpath_chk(".", buffer, sizeof(buffer));

    if (errno != EAGAIN) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, EAGAIN);
    }
}

/**
 * @tc.name      : __realpath_chk_1400
 * @tc.desc      : Root path '/' resolution
 * @tc.level     : Level 1
 */
void __realpath_chk_1400(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk("/", buffer, sizeof(buffer));

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_1500
 * @tc.desc      : pathconf limit exceeding
 * @tc.level     : Level 1
 */
void __realpath_chk_1500(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        errno = 0;
        char buffer[1];
        (void) __realpath_chk(".", buffer, sizeof(buffer));
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __realpath_chk_1600
 * @tc.desc      : Parent directory resolution
 * @tc.level     : Level 1
 */
void __realpath_chk_1600(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk("..", buffer, sizeof(buffer));

    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_1700
 * @tc.desc      : Minimum valid buffer
 * @tc.level     : Level 1
 */
void __realpath_chk_1700(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        errno = 0;
        char full_path[PATH_MAX];
        char *ret = __realpath_chk(".", full_path, sizeof(full_path));

        if (errno) {
            t_error("%s failed: errno = %d\n", __func__, errno);
            return;
        }

        size_t path_len = strlen(full_path) + 1;
        char *buffer = malloc(path_len);
        if (!buffer) {
            t_error("%s failed: malloc failed\n", __func__);
            return;
        }

         errno = 0;
         (void) __realpath_chk(".", buffer, path_len);
         
         if(buffer) {
             free(buffer);
         }
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __realpath_chk_1800
 * @tc.desc      : Complex path resolution
 * @tc.level     : Level 1
 */
void __realpath_chk_1800(void)
{
    errno = 0;
    char buffer[PATH_MAX];
    char *ret = __realpath_chk("./.././../..", buffer, sizeof(buffer));
    
    if (errno) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    } else {
        EXPECT_EQ(__func__, errno, 0);
        EXPECT_TRUE(__func__, ret != NULL);
    }
}

/**
 * @tc.name      : __realpath_chk_1900
 * @tc.desc      : Multi-threaded concurrent execution
 * @tc.level     : Level 2
 */
void __realpath_chk_1900(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ(__FUNCTION__, pthread_create(&tids[i], NULL, __realpath_chk_thread_func, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(void)
{
    __realpath_chk_0100();
    __realpath_chk_0200();
    __realpath_chk_0300();
    __realpath_chk_0400();
    __realpath_chk_0500();
    __realpath_chk_0600();
    __realpath_chk_0700();
    __realpath_chk_0800();
    __realpath_chk_0900();
    __realpath_chk_1000();
    __realpath_chk_1100();
    __realpath_chk_1200();
    __realpath_chk_1300();
    __realpath_chk_1400();
    __realpath_chk_1500();
    __realpath_chk_1600();
    __realpath_chk_1700();
    __realpath_chk_1800();
    __realpath_chk_1900();
   
    return t_status;
}
