/**
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <locale.h>
#include <string.h>
#include <pthread.h>
#include "functionalext.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *thread_func(void *arg)
{
    struct lconv *p = localeconv();
    if (p == NULL) {
        t_error("localeconv_multithread: thread %ld localeconv failed\n", (long)arg);
        return NULL;
    }
    EXPECT_STREQ("localeconv_multithread", ".", p->decimal_point);
    EXPECT_STREQ("localeconv_multithread", "", p->thousands_sep);
    EXPECT_STREQ("localeconv_multithread", "", p->grouping);
    return NULL;
}

int main()
{
    struct lconv *pconv = NULL;
    pthread_t tid1, tid2;
    (void)setlocale(LC_ALL, "C");
    (void)setlocale(LC_MONETARY, "C");
    if (pthread_create(&tid1, NULL, thread_func, (void *)1) != 0) {
        t_error("localeconv_multithread: pthread_create tid1 failed\n");
        return -1;
    }

    if (pthread_create(&tid2, NULL, thread_func, (void *)2) != 0) {
        t_error("localeconv_multithread: pthread_create tid2 failed\n");
        return -1;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return t_status;
}