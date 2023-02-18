/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#define SIGCHAIN_TEST_SET_MASK(set, fun, signo, num)  do{ \
    int result = sigemptyset(&set);                       \
    if (result != 0) {                                    \
        EXPECT_FALSE(fun, true);                 \
    }                                                     \
    for (int i = 0; i < num; i++) {                       \
        result = sigaddset(&set, signo[i]);               \
        if (result != 0) {                                \
            EXPECT_FALSE(fun, true);             \
        }                                                 \
    }                                                     \
    result = sigprocmask(SIG_BLOCK, &set, NULL);          \
    if (result != 0) {                                    \
        EXPECT_FALSE(fun, true);                 \
    }                                                     \
} while (0)

#define SIGCHIAN_TEST_SIGNAL_NUM_1 1
#define SIGCHIAN_TEST_SIGNAL_NUM_2 2
#define SIGCHIAN_TEST_SIGNAL_NUM_3 3
#define SIGCHIAN_TEST_SIGNAL_NUM_4 4
#define SIGCHIAN_TEST_SIGNAL_NUM_10 10

#define SIGCHAIN_SIGNAL_37 37
#define SIGCHAIN_SIGNAL_43 43
#define SIGCHAIN_SIGNAL_50 50
#define SIGCHAIN_SIGNAL_56 56
#define SIGCHAIN_SIGNAL_64 64
#define SIGCHAIN_SIGNAL_65 65

extern bool get_sigchain_mask_enable();