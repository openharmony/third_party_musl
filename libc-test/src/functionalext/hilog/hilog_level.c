/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#ifdef OHOS_ENABLE_PARAMETER
#undef _GNU_SOURCE
#include <hilog/hilog_adapter.c>
#define _GNU_SOURCE
#include <musl_log.h>
#include <test.h>
#endif
#include <stdlib.h>

#define OVERWRITE 1
#define ENABLE_LOG "param set musl.log.enable true"
#define LOG_LEVEL_FATAL "param set musl.log.level FATAL"
#define LOG_LEVEL_WARN "param set musl.log.level WARN"
#define LOG_LEVEL_ERROR "param set musl.log.level ERROR"
#define LOG_LEVEL_DEBUG "param set musl.log.level DEBUG"
#define LOG_LEVEL_INFO "param set musl.log.level INFO"

void TestFatalLevel()
{
    system(LOG_LEVEL_FATAL);
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_FATAL level LOG_FATAL print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_FATAL level LOG_ERROR print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_FATAL level LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_FATAL level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_FATAL level LOG_DEBUG print failed \n");
    }
}

void TestErrorLevel()
{
    system(LOG_LEVEL_ERROR);
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_ERROR level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_ERROR level LOG_ERROR print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_ERROR level LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_ERROR level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_ERROR level LOG_DEBUG print failed \n");
    }
}

void TestWarnLevel()
{
    system(LOG_LEVEL_WARN);
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_WARN level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_WARN level LOG_ERROR print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_WARN level LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_WARN level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_WARN level LOG_DEBUG print failed \n");
    }
}

void TestInfoLevel()
{
    system(LOG_LEVEL_INFO);
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_INFO level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_INFO level LOG_ERROR print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_INFO level LOG_WARN print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_INFO level LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_INFO level LOG_DEBUG print failed \n");
    }
}

void TestDebugLevel()
{
    system(LOG_LEVEL_DEBUG);
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_LEVEL_DEBUG level LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_LEVEL_DEBUG level LOG_ERROR print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_LEVEL_DEBUG level LOG_WARN print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_LEVEL_DEBUG level LOG_INFO print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_LEVEL_DEBUG level LOG_DEBUG print failed \n");
    }
}

int main()
{
#ifdef OHOS_ENABLE_PARAMETER
    system(ENABLE_LOG);
    TestFatalLevel();
    TestErrorLevel();
    TestWarnLevel();
    TestInfoLevel();
    TestDebugLevel();
#endif
    return t_status;
}