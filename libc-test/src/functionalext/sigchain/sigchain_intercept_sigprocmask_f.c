/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <signal.h>
#include <wchar.h>
#include <stdlib.h>
#include "test.h"
#include "functionalext.h"
#include "sigchain_util.h"

/**
 * @tc.name      : sigchain_intercept_sigprocmask_006
 * @tc.desc      : Test the influence of sigchain on sigprocmask, the new set is null.
 * @tc.level     : Level 0
 */
static void sigchain_intercept_sigprocmask_006()
{
    int result = sigprocmask(SIG_BLOCK, NULL, NULL);
    if (result != 0) {                                    
        EXPECT_FALSE("sigchain_intercept_sigprocmask_006", true);
    }   
}

int main(void)
{
    sigchain_intercept_sigprocmask_006();
    return t_status;
}