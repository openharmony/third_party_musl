/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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


#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <netdb.h>
#include <gtest/gtest.h>

#include "hook.h"

using namespace testing::ext;
using namespace std;

extern "C"{
int getaddrinfo (char *__restrict, char *__restrict, struct addrinfo *__restrict, struct addrinfo **__restrict);
}

namespace OHOS {
namespace {

class GetAddrInfoTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void GetAddrInfoTest::TearDownTestCase(void)
{
}

int CallGetAddrInfo(int family, int sockType)
{
    set_hook_flag(SOCKET_FLAG, true);

    char host[] = "127.0.0.1";
    struct addrinfo *result = nullptr;
    struct addrinfo hint = {0};
    hint.ai_flags = AI_ADDRCONFIG;
    hint.ai_family = family;
    hint.ai_socktype = sockType;
    hint.ai_protocol = 0;

    int ret = getaddrinfo(host, nullptr, &hint, &result);

    freeaddrinfo(result);
    result = nullptr;

    set_hook_flag(SOCKET_FLAG, false);
    return ret;
}

/**
 * @tc.name: GetAddrInfoTest_001
 * @tc.desc: test socket fail
 * @tc.type: FUNC
 */
HWTEST_F(GetAddrInfoTest, GetAddrInfoTest_001, TestSize.Level0)
{
    int ret = CallGetAddrInfo(AF_INET, 0);
    EXPECT_EQ(ret, EAI_NONAME);
}

/**
 * @tc.name: GetAddrInfoTest_002
 * @tc.desc: test socket fail 
 * @tc.type: FUNC
 */
HWTEST_F(GetAddrInfoTest, GetAddrInfoTest_002, TestSize.Level0)
{
    int ret = CallGetAddrInfo(AF_INET, SOCK_STREAM);
    EXPECT_EQ(ret, EAI_NONAME);
}

/**
 * @tc.name: GetAddrInfoTest_003
 * @tc.desc: test socket fail 
 * @tc.type: FUNC
 */
HWTEST_F(GetAddrInfoTest, GetAddrInfoTest_003, TestSize.Level0)
{
    int ret = CallGetAddrInfo(AF_INET, SOCK_DGRAM);
    EXPECT_EQ(ret, EAI_NONAME);
}

/**
 * @tc.name: GetAddrInfoTest_004
 * @tc.desc: test socket fail 
 * @tc.type: FUNC
 */
HWTEST_F(GetAddrInfoTest, GetAddrInfoTest_004, TestSize.Level0)
{
    int ret = CallGetAddrInfo(AF_INET6, 0);
    EXPECT_EQ(ret, EAI_SYSTEM);
}

/**
 * @tc.name: GetAddrInfoTest_005
 * @tc.desc: test socket fail 
 * @tc.type: FUNC
 */
HWTEST_F(GetAddrInfoTest, GetAddrInfoTest_005, TestSize.Level0)
{
    int ret = CallGetAddrInfo(AF_INET6, SOCK_STREAM);
    EXPECT_EQ(ret, EAI_SYSTEM);
}

/**
 * @tc.name: GetAddrInfoTest_001
 * @tc.desc: test socket fail 
 * @tc.type: FUNC
 */
HWTEST_F(GetAddrInfoTest, GetAddrInfoTest_006, TestSize.Level0)
{
    int ret = CallGetAddrInfo(AF_INET6, SOCK_DGRAM);
    EXPECT_EQ(ret, EAI_SYSTEM);
}

} //namespace
} //namespace OHOS