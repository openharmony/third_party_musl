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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/swap.h>
#include "test.h"

/**
 * @tc.name      : swapon_0100
 * @tc.desc      : Start swapping to file/device
 * @tc.level     : Level 0
 */
void swapon_0100(void)
{
    errno = 0;
    system("cd /data/tests/libc-test/src; dd if=/dev/zero of=swapfile count=1 bs=1k; mkswap swapfile");
    int result = swapon("/data/tests/libc-test/src/swapfile", SWAP_FLAG_PREFER);
    if (result == -1) {
        t_error("%s swapon failed\n", __func__);
        remove("/data/tests/libc-test/src/swapfile");
    }

    if (errno == ENOSYS) {
        t_error("%s errno is %d\n", __func__, errno);
        return;
    }
}

int main(int argc, char *argv[])
{
    // swapon_0100();
    return t_status;
}
