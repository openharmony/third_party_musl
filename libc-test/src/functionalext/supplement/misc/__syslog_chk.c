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

#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>

#include "functionalext.h"

#define FORTIFY_FLAG 2

static void *__syslog_chk_thread_func(void *arg)
{
    (void)arg;
    for (int i = 0; i < 100; i++) {
        errno = 0;
        __syslog_chk(LOG_INFO, 0, "ok");
        __syslog_chk(LOG_INFO, FORTIFY_FLAG, "ok");
        
        if (errno && errno != ENOENT) {
            return (void*)(long)1;
        }
    }
    
    return NULL;
}
/**
 * @tc.name      : __syslog_chk_0100
 * @tc.desc      : Test normal branch - valid parameters with format string
 * @tc.level     : Level 0
 */
void __syslog_chk_0100(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0, "__syslog_chk test: %s", "normal message");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "__syslog_chk test: %s", "normal message");
    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0200
 * @tc.desc      : Test normal branch - all priority levels
 * @tc.level     : Level 0
 */
void __syslog_chk_0200(void)
{
    errno = 0;
    __syslog_chk(LOG_EMERG, 0, "EMERGENCY message");
    __syslog_chk(LOG_ALERT, 0, "ALERT message");
    __syslog_chk(LOG_CRIT, 0, "CRITICAL message");
    __syslog_chk(LOG_ERR, 0, "ERROR message");
    __syslog_chk(LOG_WARNING, 0, "WARNING message");
    __syslog_chk(LOG_NOTICE, 0, "NOTICE message");
    __syslog_chk(LOG_INFO, 0, "INFO message");
    __syslog_chk(LOG_DEBUG, 0, "DEBUG message");
    __syslog_chk(LOG_EMERG, FORTIFY_FLAG, "EMERGENCY message");
    __syslog_chk(LOG_ALERT, FORTIFY_FLAG, "ALERT message");
    __syslog_chk(LOG_CRIT, FORTIFY_FLAG, "CRITICAL message");
    __syslog_chk(LOG_ERR, FORTIFY_FLAG, "ERROR message");
    __syslog_chk(LOG_WARNING, FORTIFY_FLAG, "WARNING message");
    __syslog_chk(LOG_NOTICE, FORTIFY_FLAG, "NOTICE message");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "INFO message");
    __syslog_chk(LOG_DEBUG, FORTIFY_FLAG, "DEBUG message");

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0300
 * @tc.desc      : Test normal branch - boundary priority values
 * @tc.level     : Level 1
 */
void __syslog_chk_0300(void)
{
    errno = 0;
    __syslog_chk(0, 0, "Minimum priority");
    __syslog_chk(0x3ff, 0, "Maximum valid priority");
    __syslog_chk(0, FORTIFY_FLAG, "Minimum priority");
    __syslog_chk(0x3ff, FORTIFY_FLAG, "Maximum valid priority");

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0400
 * @tc.desc      : Test normal branch - empty string message
 * @tc.level     : Level 1
 */
void __syslog_chk_0400(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0, "");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "");

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0500
 * @tc.desc      : Test error branch - NULL message parameter
 * @tc.level     : Level 2
 */
void __syslog_chk_0500(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0, NULL);
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, NULL);

    if (errno != EINVAL) {
        t_error("%s failed: expected errno = EINVAL, got %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0600
 * @tc.desc      : Test normal branch - different flag values
 * @tc.level     : Level 1
 */
void __syslog_chk_0600(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0, "Test message with flag 0");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "Test message with FORTIFY_FLAG");

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0700
 * @tc.desc      : Test normal branch - long message with special characters
 * @tc.level     : Level 1
 */
void __syslog_chk_0700(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0,
                 "Long message with special chars: \\n\\t\\\\\"%s\" and numbers: %d %f",
                 "test", 123, 45.67);
    __syslog_chk(LOG_INFO, FORTIFY_FLAG,
                 "Long message with special chars: \\n\\t\\\\\"%s\" and numbers: %d %f",
                 "test", 123, 45.67);

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_0800
 * @tc.desc      : Test parameter validation that should cause abort (position parameter mismatch)
 * @tc.level     : Level 2
 */
void __syslog_chk_0800(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("fork failed\n");
    } else if (pid == 0) {
        __syslog_chk(LOG_INFO, 2, "%3$d", 0);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        EXPECT_TRUE(__func__, WIFSIGNALED(status));
        EXPECT_EQ(__func__, WTERMSIG(status), SIGABRT);
    }
}

/**
 * @tc.name      : __syslog_chk_0900
 * @tc.desc      : Test normal branch - specified position parameters with flag 2
 * @tc.level     : Level 0
 */
void __syslog_chk_0900(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0, "%2$s, %3$s, %1$s", "test", "this", "is");
    __syslog_chk(LOG_INFO, 2, "%2$s, %3$s, %1$s", "test", "this", "is");

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1000
 * @tc.desc      : Test normal branch - complex position parameters with flag 2
 * @tc.level     : Level 0
 */
void __syslog_chk_1000(void)
{
    errno = 0;
    const char *service = "webserver";
    int port = 8080;
    const char *host = "localhost";

    __syslog_chk(LOG_INFO, 0, "Service %1$s running on %3$s:%2$d", service, port, host);
    __syslog_chk(LOG_INFO, 2, "Service %1$s running on %3$s:%2$d", service, port, host);

    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1100
 * @tc.desc      : Verify valid priority values with different flags
 * @tc.level     : Level 0
 */
void __syslog_chk_1100(void)
{
    errno = 0;
    __syslog_chk(LOG_USER | LOG_INFO, 0, "Valid priority test: %s", "normal execution");
    __syslog_chk(LOG_USER | LOG_INFO, FORTIFY_FLAG, "Valid priority test: %s", "normal execution");
    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1200
 * @tc.desc      : Verify behavior with undefined priority values
 * @tc.level     : Level 2
 */
void __syslog_chk_1200(void)
{

    errno = 0;
    (void)__syslog_chk(0x7fffffff, 0, "Undefined priority test");
     if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
    errno = 0;
    __syslog_chk(0x7fffffff, FORTIFY_FLAG, "Undefined priority test");
     if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1300
 * @tc.desc      : Verify behavior when flags > 0 (validation enabled)
 * @tc.level     : Level 1
 */
void __syslog_chk_1300(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "flags>0 test: %s", "normal execution");
    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1400
 * @tc.desc      : NULL message parameter
 * @tc.level     : Level 2
 */
void __syslog_chk_1400(void)
{
    errno = 0;
    __syslog_chk(LOG_INFO, 0, NULL);
    if (errno != EINVAL) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
    errno = 0;
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, NULL);
    if (errno != EINVAL) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1500
 * @tc.desc      : Verify format string with integer parameter
 * @tc.level     : Level 0
 */
void __syslog_chk_1500(void)
{
    errno = 0;
     __syslog_chk(LOG_INFO, 0, "value=%d", 10);
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "value=%d", 10);
    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1600
 * @tc.desc      : errno preservation behavior
 * @tc.level     : Level 1
 */
void __syslog_chk_1600(void)
{
    errno = EAGAIN;
    __syslog_chk(LOG_INFO, 0, "ok");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "ok");
    if (errno != EAGAIN && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1700
 * @tc.desc      : Test comprehensive boundary conditions
 * @tc.level     : Level 1
 */
void __syslog_chk_1700(void)
{
    errno = 0;
    
    __syslog_chk(LOG_INFO, 0, "Short message");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "Short message");
    
    char long_string[256];
    memset(long_string, 'A', 255);
    long_string[255] = '\0';
    
    __syslog_chk(LOG_NOTICE, 0, "Long string test: %s (length: %zu)", 
                long_string, strlen(long_string));
    __syslog_chk(LOG_NOTICE, FORTIFY_FLAG, "Long string test: %s (length: %zu)", 
                long_string, strlen(long_string));
    
    __syslog_chk(LOG_INFO, 0, "Special characters: newline\n tab\t percent%%");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "Special characters: newline\n tab\t percent%%");
    
    __syslog_chk(LOG_INFO, 0, "Multiple formats: integer %d, float %.2f, string %s", 
                100, 3.14159, "test");
    __syslog_chk(LOG_INFO, FORTIFY_FLAG, "Multiple formats: integer %d, float %.2f, string %s", 
                100, 3.14159, "test");
    
    if (errno && errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : __syslog_chk_1800
 * @tc.desc      : Multi-threaded concurrent execution
 * @tc.level     : Level 2
 */
void __syslog_chk_1800(void)
{
    const int N = 8;
    pthread_t tids[N];
    for (int i = 0; i < N; i++) {
        EXPECT_EQ(__FUNCTION__, pthread_create(&tids[i], NULL, __syslog_chk_thread_func, NULL), 0);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
    }
}

int main(void)
{
    __syslog_chk_0100();
    __syslog_chk_0200();
    __syslog_chk_0300();
    __syslog_chk_0400();
    __syslog_chk_0500();
    __syslog_chk_0600();
    __syslog_chk_0700();
    __syslog_chk_0800();
    __syslog_chk_0900();
    __syslog_chk_1000();
    __syslog_chk_1100();
    __syslog_chk_1200();
    __syslog_chk_1300();
    __syslog_chk_1400();
    __syslog_chk_1500();
    __syslog_chk_1600();
    __syslog_chk_1700();
    __syslog_chk_1800();

    return t_status;
}
