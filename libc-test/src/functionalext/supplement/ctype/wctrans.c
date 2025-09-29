/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include "test.h"

/**
 * @tc.name      : wctrans_0100
 * @tc.desc      : Test that wctrans returns a character conversion of a scalar type
 * @tc.level     : Level 0
 */
void wctrans_0100(void)
{
    if (!wctrans("tolower")) {
        t_error("%s wctrans error get result is NULL\n", __func__);
    }
}

/**
 * @tc.name      : wctrans_0200
 * @tc.desc      : Test result of wctrans when incoming uppercase conversion
 * @tc.level     : Level 1
 */
void wctrans_0200(void)
{
    if (!wctrans("toupper")) {
        t_error("%s wctrans error get result is NULL\n", __func__);
    }
}

/**
 * @tc.name      : wctrans_0300
 * @tc.desc      : Test the result of wctrans when passing in an exception parameter
 * @tc.level     : Level 2
 */
void wctrans_0300(void)
{
    if (wctrans("monkeys")) {
        t_error("%s wctrans error get result is not NULL\n", __func__);
    }
}

/**
 * @tc.name : wctrans_0400
 * @tc.desc : Test that wctrans returns different values for tolower/toupper
 * @tc.level : Level 1
 */
void wctrans_0400(void)
{
    if (wctrans("tolower") == wctrans("toupper")) {
        t_error("%s wctrans error: tolower and toupper should return different values\n", __func__);
    }
}

/**
 * @tc.name : wctrans_0500
 * @tc.desc : Test that wctrans is case-sensitive for conversion names
 * @tc.level : Level 2
 */
void wctrans_0500(void)
{
    if (wctrans("TOLOWER")) {
        t_error("%s wctrans error: should be case-sensitive\n", __func__);
    }
}

/**
 * @tc.name : wctrans_0600
 * @tc.desc : Test that wctrans returns NULL for unknown conversion
 * @tc.level : Level 1
 */
void wctrans_0600(void)
{
    if (wctrans("unknown")) {
        t_error("%s wctrans error: should return NULL for unknown conversion\n", __func__);
    }
}

/**
 * @tc.name : wctrans_0700
 * @tc.desc : Test that wctrans handles long invalid strings correctly
 * @tc.level : Level 2
 */
void wctrans_0700(void)
{
    const char *long_str = "this_is_a_very_long_string_that_is_not_a_valid_conversion_type";
    if (wctrans(long_str)) {
        t_error("%s wctrans error: should return NULL for long invalid strings\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wctrans_0100();
    wctrans_0200();
    wctrans_0300();
    wctrans_0400();
    wctrans_0500();
    wctrans_0600();
    wctrans_0700();
    return t_status;
}