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

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#include "hook.h"

using namespace testing::ext;
using namespace std;

extern "C" {

struct node {
    void (*func)(void *);
    void *arg;
    void *dso;
    void *internal_dso;
    struct node *prev;
    struct node *next;
};

extern struct node *tail;

extern struct node* remove_node(struct node *node);

extern void __cxa_finalize(void *dso);

extern int invalidate_exit_funcs(struct dso *p);

}

namespace OHOS {
namespace {

class AtexitTest : public testing::Test {
public:
    static void TearDownTestCase(void);
};

void AtexitTest::TearDownTestCase(void)
{
}

/**
 * @tc.name: test_invalidate_exit_funcs_001
 * @tc.desc: remove_node
 * @tc.type: FUNC
 */
HWTEST_F(AtexitTest, invalidate_exit_funcs_001, TestSize.Level0)
{
    struct node tmp;
    tmp.internal_dso = tail->internal_dso;
    tmp.dso = tail->dso;
    tmp.func = tail->func;
    tmp.arg = tail->arg;
    tail->internal_dso = NULL;
    tail->dso = NULL;
    tail->func = NULL;
    invalidate_exit_funcs(NULL);
    tail->internal_dso = tmp.internal_dso;
    tail->dso = tmp.dso;
    tail->func = tmp.func;
    ASSERT_TRUE(tmp.arg == tail->arg);
}

/**
 * @tc.name: test_invalidate_exit_funcs_002
 * @tc.desc: remove_node
 * @tc.type: FUNC
 */
HWTEST_F(AtexitTest, invalidate_exit_funcs_002, TestSize.Level0)
{
    struct node tmp;
    tmp.internal_dso = tail->internal_dso;
    tmp.dso = tail->dso;
    tmp.func = tail->func;
    tmp.arg = tail->arg;
    tail->internal_dso = NULL;
    tail->dso = NULL;
    tail->func = NULL;
    invalidate_exit_funcs(NULL);
    tail->internal_dso = tmp.internal_dso;
    tail->dso = tmp.dso;
    tail->func = tmp.func;
    ASSERT_TRUE(tmp.arg == tail->arg);
}

/**
 * @tc.name: test_AtexitTest_001
 * @tc.desc: remove_node
 * @tc.type: FUNC
 */
HWTEST_F(AtexitTest, AtexitTest_001, TestSize.Level0)
{
    struct node *nodes;
    nodes = (struct node *)malloc(sizeof(struct node));
    nodes->next = NULL;
    nodes->prev = NULL;
    tail->next = NULL;
    remove_node(nodes);
    free(nodes);
}

/**
 * @tc.name: test_AtexitTest_002
 * @tc.desc: remove_node
 * @tc.type: FUNC
 */
HWTEST_F(AtexitTest, AtexitTest_002, TestSize.Level0)
{
    struct node *nodes;
    nodes = (struct node *)malloc(sizeof(struct node));
    struct node *next;
    next = (struct node *)malloc(sizeof(struct node));
    struct node *prev;
    prev = (struct node *)malloc(sizeof(struct node));
    nodes->next = next;
    nodes->prev = prev;

    remove_node(nodes);

    free(nodes);
    free(next);
    free(prev);
}

/**
 * @tc.name: test_AtexitTest_003
 * @tc.desc: remove_node
 * @tc.type: FUNC
 */
HWTEST_F(AtexitTest, AtexitTest_003, TestSize.Level0)
{
    tail->prev = NULL;
    remove_node(nodes);
}

}
}

