/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : res_querydomain_0100
 * @tc.desc      : Verify that the domain name can be queried (parameters are valid)
 * @tc.level     : Level 0
 */
void res_querydomain_0100(void)
{
    int aslen = 1024;
    unsigned char aswr[1024];
    char name[] = "test";
    char domain[] = "hello";
    int result = res_querydomain(name, domain, 1, 1, aswr, aslen);
    EXPECT_EQ("res_querydomain_0100", result, 0);
}

/**
 * @tc.name      : res_querydomain_0200
 * @tc.desc      : Verify that the domain name cannot be queried (the name parameter is invalid)
 * @tc.level     : Level 2
 */
void res_querydomain_0200(void)
{
    int aslen = 1024;
    unsigned char aswr[1024];
    char name[254];
    for (int i = 0; i < 254; i++) {
        name[i] = 'a';
    }
    char domain[] = "hello";
    int result = res_querydomain(name, domain, 1, 1, aswr, aslen);
    EXPECT_EQ("res_querydomain_0200", result, -1);
}

/**
 * @tc.name      : res_querydomain_0300
 * @tc.desc      : Verify that the domain name cannot be queried (the domain parameter is invalid)
 * @tc.level     : Level 2
 */
void res_querydomain_0300(void)
{
    int aslen = 1024;
    unsigned char aswr[1024];
    char domain[254];
    for (int i = 0; i < 254; i++) {
        domain[i] = 'a';
    }
    char name[] = "test";
    int result = res_querydomain(name, domain, 1, 1, aswr, aslen);
    EXPECT_EQ("res_querydomain_0300", result, -1);
}

/**
 * @tc.name      : res_querydomain_0400
 * @tc.desc      : Verify that the domain name cannot be queried (the name parameter and domain parameter are invalid)
 * @tc.level     : Level 2
 */
void res_querydomain_0400(void)
{
    int aslen = 1024;
    unsigned char aswr[1024];
    char name[254];
    char domain[254];
    for (int i = 0; i < 127; i++) {
        name[i] = 'a';
    }
    for (int j = 0; j < 127; j++) {
        domain[j] = 'b';
    }
    int result = res_querydomain(name, domain, 1, 1, aswr, aslen);
    EXPECT_EQ("res_querydomain_0400", result, -1);
}

TEST_FUN G_Fun_Array[] = {
    res_querydomain_0100,
    res_querydomain_0200,
    res_querydomain_0300,
    res_querydomain_0400,
};

int main()
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}