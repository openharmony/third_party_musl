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

#define SIGCHIAN_TEST_SET_MASK(set, fun, signo, num)  do{ \
    int result = sigemptyset(&set);                       \
    if (result != 0) {                                    \
        EXPECT_FALSE(fun, (result == 0));                 \
    }                                                     \
    for (int i = 0; i < num; i++) {                       \
        result = sigaddset(&set, signo[i]);               \
        if (result != 0) {                                \
            EXPECT_FALSE(fun, (result == 0));             \
        }                                                 \
    }                                                     \
    result = sigprocmask(SIG_BLOCK, &set, NULL);          \
    if (result != 0) {                                    \
        EXPECT_FALSE(fun, (result == 0));                 \
    }                                                     \
} while (0)
