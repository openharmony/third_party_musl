#ifndef LIBC_TEST_SRC_FUNCTIONALEXT_RPATH_TEST_H
#define LIBC_TEST_SRC_FUNCTIONALEXT_RPATH_TEST_H

static const char* g_absoluteLib = "rpath_lib/libprimary_absolute.so";
static const char* g_originLib = "rpath_lib/libprimary_origin.so";
static const char* g_multipleLib = "rpath_lib/libprimary_multiple.so";
static const char* g_nsOriginLib = "libprimary_origin.so";
static const char* g_nsMultipleLib = "libprimary_multiple.so";

static const char* g_dlName = "dlns_rpath_test";
static const char* g_dlpath = "/data/tests/libc-test/src/rpath_lib:/system/lib:/system/lib64";
static const char* g_path = "/data/tests/libc-test/src";
static const char* g_errPath = "/data/tests/libc-test/src/rpath_lib/rpath_support_B";

typedef void(*TEST_FUN)(void);
#endif