/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "test.h"
#include <errno.h>
#include <iconv.h>
#include <pthread.h>

#include "functionalext.h"

#define TCOUNT 16
#define PARAM_2 2
#define CONV_INPUT "Hello, iconv!"

pthread_mutex_t mutex;
pthread_mutex_t mutex_half;

pthread_cond_t cond;
pthread_cond_t cond_half;

int generation = 0;
int generation_half = 0;

int arrived = 0;
int arrived_half = 0;

void barrier_wait()
{
    int gen = generation;
    pthread_mutex_lock(&mutex);
    if (++arrived == TCOUNT) {
        arrived = 0;
        generation++;
        pthread_cond_broadcast(&cond);
    } else {
        while (gen == generation) {
            pthread_cond_wait(&cond, &mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
}

/*  Execute iconv_open, iconv and iconv_close in a synchronized way in
    conjunction with other sibling worker threads. If any step fails,
    print an error to stdout and return NULL to the main thread to
    indicate the error. */

static void* worker(void *arg)
{
    long int tidx = (long int)arg;

    iconv_t cd;

    char ascii[] = CONV_INPUT;
    size_t bytes = sizeof(CONV_INPUT) - 1;
    char *inbufpos = ascii;
    size_t inbytesleft = bytes;

    char *utf8 = calloc(bytes, 1);
    if (utf8 == NULL) {
        t_error("calloc failed \n");
        return 0;
    }
    char *outbufpos = utf8;
    size_t outbytesleft = bytes;

    if (tidx < TCOUNT / PARAM_2) {
        /*  The first half of the worker thread pool synchronize together here,
            then call iconv_open immediately after. */
        int gen = generation_half;
        pthread_mutex_lock(&mutex_half);
        if (++arrived_half == TCOUNT / PARAM_2) {
            generation_half++;
            pthread_cond_broadcast(&cond_half);
        } else {
            while (gen == generation_half) {
                pthread_cond_wait(&cond_half, &mutex_half);
            }
        }
        pthread_mutex_unlock(&mutex_half);
    } else {
        /*  The second half wait for the first half to finish iconv_open and continue
            to the next barrier (before the call to iconv below). */
        barrier_wait();
    }
    /*  The above block of code staggers all subsequent pthread_barrier_wait
        calls in a way that ensures a high chance of encountering these
        combinations of concurrent iconv usage:
        1) concurrent calls to iconv_open,
        2) concurrent calls to iconv_open *and* iconv,
        3) concurrent calls to iconv,
        4) concurrent calls to iconv *and* iconv_close,
        5) concurrent calls to iconv_close. */
    cd = iconv_open("UTF8", "ASCII");
    if (cd == (iconv_t)-1) {
        t_error("iconv_open failed: %s\n", strerror(errno));
        return 0;
    }

    barrier_wait();

    size_t ret = iconv(cd, &inbufpos, &inbytesleft, &outbufpos, &outbytesleft);
    if (ret == (size_t)-1) {
        t_error("iconv failed: %s\n", strerror(errno));
        return 0;
    }

    barrier_wait();

    iconv_close(cd);

    /*  The next conditional barrier wait is needed because we staggered the
        threads into two groups in the beginning and at this point, the second
        half of worker threads are waiting for the first half to finish
        iconv_close before they can executing the same: */
    if (tidx < TCOUNT / PARAM_2) {
        barrier_wait();
    }

    return 0;
}

/**
 * @tc.name         : iconv_0100
 * @tc.desc         : Test the multi-thread status of the iconv.
 * @tc.level        : Level 0
 */

void iconv_0100()
{
    pthread_t thread[TCOUNT];
    int i;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_half, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond_half, NULL);

    for (i = 0; i < TCOUNT; i++) {
        pthread_create(&thread[i], NULL, worker, (void*)(long int)i);
    }

    for (i = 0; i < TCOUNT; i++) {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_half);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond_half);
}

int main(void)
{
    int icu_res = set_iconv_icu_enable();
    if (icu_res != 0) {
        t_error("set_iconv_icu_enable failed, error: %d \n", icu_res);
        return t_status;
    }
    iconv_0100();
    return t_status;
}