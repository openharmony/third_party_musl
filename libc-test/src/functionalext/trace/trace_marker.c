/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <trace/trace_marker.h>

#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test.h>
#include <unistd.h>

#define BUFFER_LEN 10240
#define EXPECT_TRUE(c)                \
    do                                \
    {                                 \
        if (!(c))                     \
            t_error("[%s] failed\n"); \
    } while (0)
#define EXPECT_FALSE(c)                \
    do                                 \
    {                                  \
        if ((c))                       \
            t_error("[%s] failed \n"); \
    } while (0)

typedef void (*TEST_FUN)(void);
static const int WAIT_TIME = 1;
static const int count = 10;

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end.
 * @tc.level     : Level 0
 */

static void trace_marker_0010(void)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    trace_marker_begin("Musl_Trace_Marker_0100", "");
    trace_marker_end();
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Musl_Trace_Marker_0100");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker_async
 * @tc.desc      : Test trace_marker_async_begin and trace_marker_async_end.
 * @tc.level     : Level 0
 */
static void trace_marker_0020(void)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    trace_marker_async_begin("async_begin_0200", "trace_async",1);
    trace_marker_async_end("async_end_0200", "trace_async",1);
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "async_begin_0200", "trace_async" , 1);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "async_end_0200", "trace_async" , 1);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test trace_marker_begin and trace_marker_end.
 * @tc.level     : Level 0
 */
static void trace_marker_0030(void)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    int traceCount = 5;
    trace_marker_count("traceCount", traceCount);
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return;
        }
    }

    bool trace_count_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount", traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test the multiple processes of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_0040(void)
{
    bool trace_sucess = false;
    char buffer_fir[BUFFER_LEN] = {0};
    char buffer_sec[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();
        system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
        trace_marker_begin("Trace_Marker0400_Forkfir", "");
        trace_marker_end();
        system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker0400_Forkfir");
        if (buf_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
        if (buf_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_fir, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_fir, buf_begin) != NULL && strstr(buffer_fir, buf_end) != NULL) {
                trace_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_sucess);
        close(trace_fd);
        exit(pidChild);
    } else {
        system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
        trace_marker_begin("Trace_Marker0400_Forksec", "");
        trace_marker_end();
        system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker0400_Forksec");
        if (buf_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
        if (buf_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_sec, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_sec, buf_begin) != NULL && strstr(buffer_sec, buf_end) != NULL) {
                trace_sucess = true;
                break;
            }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
    }
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test the multiple processes of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_0050(void)
{
    bool trace_async_sucess = false;
    char buffer_forkFir[BUFFER_LEN] = {0};
    char buffer_forkSec[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();
        system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
        trace_marker_async_begin("async0500_Forkfir", "begin_fir", 2);
        trace_marker_async_end("async0500_Forkfir", "end_fir", 2);
        system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "async0500_Forkfir", "begin_fir", 2);
        if (buf_async_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "async0500_Forkfir", "end_fir", 2);
        if (buf_async_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkFir, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_forkFir, buf_async_begin) != NULL && strstr(buffer_forkFir, buf_async_end) != NULL) {
                trace_async_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_async_sucess);
        close(trace_fd);
        exit(pidChild);
    } else {
        system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
        trace_marker_async_begin("async0500_Forksec", "begin_sec", 3);
        trace_marker_async_end("async0500_Forksec", "end_sec", 3);
        system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "async0500_Forksec", "begin_sec", 3);
        if (buf_async_begin_fd < 0) {
            close(trace_fd);
            return;
        }

        int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "async0500_Forksec", "end_sec", 3);
        if (buf_async_end_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkSec, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buffer_forkSec, buf_async_begin) != NULL && strstr(buffer_forkSec, buf_async_end) != NULL) {
                trace_async_sucess = true;
                break;
            }
        }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
    }
}

/**
 * @tc.name      : trace_marker
 * @tc.desc      : Test the multiple processes of trace_marker.
 * @tc.level     : Level 0
 */
static void trace_marker_0060(void)
{
    int traceCount = 5;
    bool trace_count_sucess = false;
    char buffer_forkFir[BUFFER_LEN] = {0};
    char buffer_forkSec[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork! \n");
    } else if (fpid == 0) {
        int pidChild = getpid();

        system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
        trace_marker_count("traceCount_forkfir", traceCount);
        system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_count_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_forkfir", traceCount);
        if (buf_count_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkFir, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buf_count, buffer_forkFir) != NULL) {
                trace_count_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_count_sucess);
        close(trace_fd);
        exit(pidChild);
    } else {
        system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
        trace_marker_count("traceCount_forksec", traceCount);
        system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

        int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
        if (trace_fd == -1) {
            trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY | O_APPEND);
            if (trace_fd == -1) {
                return;
            }
        }
        int buf_count_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_forksec", traceCount);
        if (buf_count_fd < 0) {
            close(trace_fd);
            return;
        }
        for (int i = 0; i < count; i++) {
            int read_fd = read(trace_fd, buffer_forkSec, BUFFER_LEN * i);
            if (read_fd == -1) {
                close(trace_fd);
                return;
            }
            if (strstr(buf_count, buffer_forkSec) != NULL) {
                trace_count_sucess = true;
                break;
            }
        }
        EXPECT_TRUE(trace_count_sucess);
        close(trace_fd);
    }
}

static void *ThreadTraceMarkerFir(void *arg)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    trace_marker_begin("Trace_Marker_Threadfir", "pthreadfir");
    trace_marker_end();
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker_Threadfir");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerFir Exit");
}

static void *ThreadTraceMarkerSec(void *arg)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    trace_marker_begin("Trace_Marker_Threadsec", "pthreadsec");
    trace_marker_end();
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }
    bool trace_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_begin[BUFFER_LEN] = {0};
    char buf_end[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_begin, BUFFER_LEN, "B|%d|%s", getpid(), "Trace_Marker_Threadsec");
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_end_fd = snprintf(buf_end, BUFFER_LEN, "E|%d", getpid());
    if (buf_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_begin) != NULL && strstr(buffer, buf_end) != NULL) {
            trace_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerSec Exit");
}

/**
 * @tc.number: trace_marker_0070
 * @tc.name: trace_marker
 * @tc.desc: Test the multithreading of trace_marker.
 */
static void trace_marker_0070(void)
{
    int res;
    const char msgThread1[1024] = {"msgThread1"};
    const char msgThread2[1024] = {"msgThread2"};
    pthread_t fatalMessageThread1, fatalMessageThread2;
    res = pthread_create(&fatalMessageThread1, NULL, ThreadTraceMarkerFir, (void *)msgThread1);
    if (res != 0) {
        t_printf("pthread_create1 error.");
    }
    sleep(WAIT_TIME);
    res = pthread_create(&fatalMessageThread2, NULL, ThreadTraceMarkerSec, (void *)msgThread2);
    if (res != 0) {
        t_printf("pthread_create2 error.");
    }
    pthread_join(fatalMessageThread1, NULL);
    pthread_join(fatalMessageThread2, NULL);
}

static void *ThreadTraceMarkerAsyncFir(void *arg)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    trace_marker_async_begin("Async_Threadfir", "begin_threadfir",4);
    trace_marker_async_end("Async_Threadfir", "end_threadfir", 4);
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "Async_Threadfir", "begin_threadfir", 4);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "Async_Threadfir", "end_threadfir", 4);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerAsyncFir Exit");
}

static void *ThreadTraceMarkerAsyncSec(void *arg)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    trace_marker_async_begin("Async_Threadsec", "begin_threadsec",5);
    trace_marker_async_end("Async_Threadsec", "end_threadsec",5);
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_async_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_async_begin[BUFFER_LEN] = {0};
    char buf_async_end[BUFFER_LEN] = {0};
    int buf_async_begin_fd = snprintf(buf_async_begin, BUFFER_LEN, "S|%d|%s|%s %d", getpid(), "Async_Threadsec", "begin_threadsec", 5);
    if (buf_async_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    int buf_async_end_fd = snprintf(buf_async_end, BUFFER_LEN, "F|%d|%s|%s %d", getpid(), "Async_Threadsec", "end_threadsec", 5);
    if (buf_async_end_fd < 0) {
        close(trace_fd);
        return NULL;
    }
    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_async_begin) != NULL && strstr(buffer, buf_async_end) != NULL) {
            trace_async_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_async_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerAsyncSec Exit");
}

/**
 * @tc.number: trace_marker_0080
 * @tc.name: trace_marker
 * @tc.desc: Test the multithreading of trace_marker.
 */
static void trace_marker_0080(void)
{
    int res;
    const char msgThread1[1024] = {"msgThread3"};
    const char msgThread2[1024] = {"msgThread4"};
    pthread_t fatalMessageThread1, fatalMessageThread2;
    res = pthread_create(&fatalMessageThread1, NULL, ThreadTraceMarkerAsyncFir, (void *)msgThread1);
    if (res != 0) {
        t_printf("pthread_create3 error.");
    }
    sleep(WAIT_TIME);
    res = pthread_create(&fatalMessageThread2, NULL, ThreadTraceMarkerAsyncSec, (void *)msgThread2);
    if (res != 0) {
        t_printf("pthread_create4 error.");
    }
    pthread_join(fatalMessageThread1, NULL);
    pthread_join(fatalMessageThread2, NULL);
}

static void *ThreadTraceMarkerCountFir(void *arg)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    int traceCount = 5;
    trace_marker_count("traceCount_Threadfir", traceCount);
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_count_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_Threadfir", traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerCountFir Exit");
}

static void *ThreadTraceMarkerCountSec(void *arg)
{
    system("cd /sys/kernel/debug/tracing;echo 1 > tracing_on");
    int traceCount = 5;
    trace_marker_count("traceCount_Threadsec", traceCount);
    system("cd /sys/kernel/debug/tracing;echo 0 > tracing_on");

    int trace_fd = open("/sys/kernel/tracing/trace", O_CLOEXEC | O_RDONLY);
    if (trace_fd == -1) {
        trace_fd = open("/sys/kernel/debug/tracing/trace", O_CLOEXEC | O_RDONLY);
        if (trace_fd == -1) {
            return NULL;
        }
    }

    bool trace_count_sucess = false;
    char buffer[BUFFER_LEN] = {0};
    char buf_count[BUFFER_LEN] = {0};

    int buf_begin_fd = snprintf(buf_count, BUFFER_LEN, "C|%d|%s %d", getpid(), "traceCount_Threadsec", traceCount);
    if (buf_begin_fd < 0) {
        close(trace_fd);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        int read_fd = read(trace_fd, buffer, BUFFER_LEN * i);
        if (read_fd == -1) {
            close(trace_fd);
            return NULL;
        }
        if (strstr(buffer, buf_count) != NULL) {
            trace_count_sucess = true;
            break;
        }
    }
    EXPECT_TRUE(trace_count_sucess);
    close(trace_fd);
    pthread_exit("ThreadTraceMarkerCountSec Exit");
}
/**
 * @tc.number: trace_marker_0090
 * @tc.name: trace_marker
 * @tc.desc: Test the multithreading of trace_marker.
 */
static void trace_marker_0090(void)
{
    int res;
    const char msgThread1[1024] = {"msgThread5"};
    const char msgThread2[1024] = {"msgThread6"};
    pthread_t fatalMessageThread1, fatalMessageThread2;
    res = pthread_create(&fatalMessageThread1, NULL, ThreadTraceMarkerCountFir, (void *)msgThread1);
    if (res != 0) {
        t_printf("pthread_create5 error.");
    }
    sleep(WAIT_TIME);
    res = pthread_create(&fatalMessageThread2, NULL, ThreadTraceMarkerCountSec, (void *)msgThread2);
    if (res != 0) {
        t_printf("pthread_create6 error.");
    }
    pthread_join(fatalMessageThread1, NULL);
    pthread_join(fatalMessageThread2, NULL);
}

TEST_FUN G_Fun_Array[] = {
    trace_marker_0010,
    trace_marker_0020,
    trace_marker_0030,
    trace_marker_0040,
    trace_marker_0050,
    trace_marker_0060,
    trace_marker_0070,
    trace_marker_0080,
    trace_marker_0090,
    };

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
