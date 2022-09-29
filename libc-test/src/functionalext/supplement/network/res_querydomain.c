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

#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

const char name[] = "www";
const char domain[] = "example.com";
const int class = C_IN;
const int type = T_TXT;
const char options[] = "options timeout:0";

void set_timeout(void)
{
    system("cp /etc/resolv.conf /etc/resolv.conf.bak");

    FILE *f = fopen(_PATH_RESCONF, "a+");
    if (f == NULL) {
        t_error("%s failed: fopen\n", __func__);
        return;
    }

    fwrite(options, sizeof(options), 1, f);
    fclose(f);
}

void restore_conf(void)
{
    remove(_PATH_RESCONF);
    rename("/etc/resolv.conf.bak", _PATH_RESCONF);
}

/**
 * @tc.name      : res_querydomain_0100
 * @tc.desc      : query the name server
 * @tc.level     : Level 0
 */
void res_querydomain_0100(void)
{
    set_timeout();

    unsigned char buf[BUFSIZ] = {0};
    int result = res_querydomain(name, domain, class, type, buf, sizeof(buf));
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    restore_conf();
}

/**
 * @tc.name      : res_querydomain_0200
 * @tc.desc      : query the name server with an invalid type
 * @tc.level     : Level 2
 */
void res_querydomain_0200(void)
{
    set_timeout();

    int result = res_querydomain(name, domain, class, T_ANY + 1, NULL, 0);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    restore_conf();
}

int main(int argc, char *argv[])
{
    res_querydomain_0100();
    res_querydomain_0200();
    return t_status;
}