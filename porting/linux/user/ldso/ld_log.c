/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "ld_log.h"

static bool ld_log_enable = false;

#ifdef OHOS_ENABLE_PARAMETER
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "sys_param.h"

#define SYSPARAM_LENGTH 32
#define PROCESS_NAME_LEN 1024

static char *get_app_name(char *buf, size_t length)
{
    char *app = NULL;
    int fd = open("/proc/self/cmdline", O_RDONLY);
    if (fd != -1) {
        ssize_t ret = read(fd, buf, length - 1);
        if (ret != -1) {
            buf[ret] = 0;
            app = strrchr(buf, '/');
            if (app) {
                app++;
            } else {
                app = buf;
            }
            char *app_end = strchr(app, ':');
            if (app_end) {
                *app_end = 0;
            }
        }
        close(fd);
    }
    return app;
}

static bool get_ld_log_app_value(char *buffer, uint32_t *length)
{
    char buf[PROCESS_NAME_LEN];
    char *path = get_app_name(buf, PROCESS_NAME_LEN);
    if (!path) {
        buffer[0] = 0;
        return false;
    }

    char app_param_name[PROCESS_NAME_LEN] = "musl.log.ld.app.";
    strcat(app_param_name, path);
    if (SystemReadParam(app_param_name, buffer, length) == 0) {
        buffer[*length] = 0;
        return true;
    }
    buffer[0] = 0;
    return false;
}

static bool get_ld_log_all_value(char *buffer, uint32_t *length)
{
    if (SystemReadParam("musl.log.ld.all", buffer, length) == 0) {
        buffer[*length] = 0;
        return true;
    }
    buffer[0] = 0;
    return false;
}

static inline void assign_ld_log_enable(char *param_value, const char *expect_value)
{
    ld_log_enable = (strcmp(param_value, expect_value) == 0);
}

#endif

void ld_log_reset()
{
#if (defined(OHOS_ENABLE_PARAMETER))
    if (!is_musl_log_enable()) {
        ld_log_enable = false;
        return;
    }

    char app_param_value[SYSPARAM_LENGTH];
    uint32_t app_param_value_len = SYSPARAM_LENGTH;
    char all_param_value[SYSPARAM_LENGTH];
    uint32_t all_param_value_len = SYSPARAM_LENGTH;

    if (get_ld_log_app_value(app_param_value, &app_param_value_len)) {
        assign_ld_log_enable(app_param_value, "true");
    } else {
        if (get_ld_log_all_value(all_param_value, &all_param_value_len)) {
            assign_ld_log_enable(all_param_value, "true");
        } else {
            ld_log_enable = false;
        }
    }
#else
    ld_log_enable = is_musl_log_enable();
#endif
}

bool get_ld_log_enable()
{
    return ld_log_enable;
}
