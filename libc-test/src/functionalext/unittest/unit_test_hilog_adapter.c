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

#include <hilog_adapter.h>
#include "functionalext.h"
#include "test.h"

#define MUSL_LOG_TYPE LOG_CORE
#define MUSL_LOG_DOMAIN 0xD003F00
#define MUSL_LOG_TAG "MUSL"
#define LOG_ERROR 6

#define MUSL_LOGE(...) ((void)HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, __VA_ARGS__))

/**
 * @tc.name     : reboot_0010
 * @tc.desc     : Test HiLogAdapterPrint after musl_log_reset
 * @tc.level    : Level 2
 */
static void HiLogAdapterPrint_0010(void)
{
    musl_log_reset();
    int ret = HiLogAdapterPrint(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, "a");
    EXPECT_EQ("HiLogAdapterPrint_0010", ret, -1);
}

int main(void)
{
    HiLogAdapterPrint_0010();

	return t_status;
}
