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

#include "musl_version.h"
#include "functionalext.h"

/**
 * @tc.name      : get_musl_version_0100
 * @tc.desc      : Test basic functionality of get_musl_version
 * @tc.level     : Level 0
 */
void get_musl_version_0100(void)
{
    const char *version = get_musl_version();
    EXPECT_PTRNE("get_musl_version_0100", version, NULL);
    EXPECT_MT("get_musl_version_0100", strlen(version), 1);
}

/**
 * @tc.name      : get_musl_version_0200
 * @tc.desc      : Verify that the version string conforms to the standard format (X.Y.Z or X.Y)
 * @tc.level     : Level 0
 */
void get_musl_version_0200(void)
{
    const char *version = get_musl_version();
    int major = 0, minor = 0, patch = 0;
    int count = 0;
    count = sscanf(version, "%d.%d.%d", &major, &minor, &patch);
    EXPECT_MT("get_musl_version_0200", count, 0);
    EXPECT_MT("get_musl_version_0200", major, 0);
    EXPECT_MT("get_musl_version_0200", minor, 0);
    if(count == 2) {
        EXPECT_TRUE("get_musl_version_0200", patch == 0);
    } else {
        EXPECT_MT("get_musl_version_0200", patch, 0);
    }
}

/**
 * @tc.name      : get_musl_version_0300
 * @tc.desc      : Verify that multiple calls return the same string address and content
 * @tc.level     : Level 0
 */
void get_musl_version_0300(void)
{
    const char *version1 = get_musl_version();
    const char *version2 = get_musl_version();
    EXPECT_TRUE("get_musl_version_0300", strcmp(version1, version2) == 0);
}


int main(void)
{
    get_musl_version_0100();
    get_musl_version_0200();
    get_musl_version_0300();
    return t_status;
}
