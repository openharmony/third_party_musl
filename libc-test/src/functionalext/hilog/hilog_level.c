#ifdef OHOS_ENABLE_PARAMETER
#undef _GNU_SOURCE
#include <hilog/hilog_adapter.c>
// #include <hilog_adapter.h>
#define _GNU_SOURCE
#include <musl_log.h>
#include <test.h>
#endif
#include <stdlib.h>

#define OVERWRITE 1
#define ENABLE_LOG "param set musl.log.enable true"
#define LOG_LEVEL_ERROR "param set musl.log.level ERROR"

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) \
        t_error("failed %d != %d \n", a, b); \
} while (0)

#define EXPECT_NE(a, b) do { \
    if ((a) == (b)) \
        t_error("failed %d == %d \n", a, b); \
} while (0)

int main()
{
#ifdef OHOS_ENABLE_PARAMETER
    system(ENABLE_LOG);
    system(LOG_LEVEL_ERROR);
    musl_log_reset();
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_FATAL)) {
        t_error("LOG_FATAL print failed \n");
    }
    if (!HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_ERROR)) {
        t_error("LOG_ERROR print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_WARN)) {
        t_error("LOG_WARN print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_INFO)) {
        t_error("LOG_INFO print failed \n");
    }
    if (HiLogAdapterIsLoggable(MUSL_LOG_DOMAIN, MUSL_LOG_TAG, LOG_DEBUG)) {
        t_error("LOG_DEBUG print failed \n");
    }
#endif
    return t_status;
}