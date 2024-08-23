/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2024. All rights reserved.
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
#include "util.h"
#include <exception>
#include <cassert>

using namespace std;

static int gThrowExceptionCnt = 0;
static int gCatchExceptionCnt = 0;

struct TestException : public exception {
    TestException()
    {
        gThrowExceptionCnt++;
    }
    const char* what() const _NOEXCEPT override
    {
        return "Test Exception!";
    }

    void check_result()
    {
        gCatchExceptionCnt++;
    }
};

// increase call stack depth
void __attribute__((noinline)) ThrowExceptionCallStub()
{
    throw TestException();
}

// increase call stack depth
void __attribute__((noinline)) TopCallStub()
{
    ThrowExceptionCallStub();
}

static void Bm_function_throw_exception(benchmark::State &state)
{
    int ret;
    for (auto _ : state) {
        try {
            TopCallStub();
        } catch (TestException& e) {
            e.check_result();
            ret = gThrowExceptionCnt;
        }
        benchmark::DoNotOptimize(ret);
    }
}

MUSL_BENCHMARK(Bm_function_throw_exception);
