/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <benchmark/benchmark.h>
#include "string.h"
#include "wchar.h"
#include "err.h"
#include "util.h"

using namespace std;

static const std::vector<int> bufferSizes {
    8,
    16,
    32,
    64,
    512,
    1 * K,
    8 * K,
    16 * K,
    32 * K,
    64 * K,
    128 * K,
};

static const std::vector<int> limitSizes{
    1,
    2,
    3,
    4,
    5,
    6,
    7,
};

static void StringtestArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : bufferSizes) {
        for (auto f : limitSizes) {
            b->Args({l, f, 0});
        }
    }
}

// Searches for the first occurrence of the character x in the first n bytes of the selected string
static void Bm_function_Memchr(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t bmmemchrAlignment = state.range(2);
    vector<char> bmmemchr;
    char *bmmemchrAligned = GetAlignedPtrFilled(&bmmemchr, bmmemchrAlignment, nbytes, 'n');
    bmmemchrAligned[nbytes - 1] = '\0';
    while (state.KeepRunning()) {
        if (memchr(bmmemchrAligned, 'x', limitsize) != nullptr) {
            errx(1, "ERROR: memchr found a chr where it should have failed.");
        }
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Finds the last occurrence of the specified character in a string and returns a pointer to that position
static void Bm_function_Strrchr(benchmark::State &state)
{
    const char *strrchrtestsrc[] = { "com.ohos.launcher", "/system/lib/libfilemgmt_libhilog.z.so",
                                     "/system/lib/libstatic_subscriber_extension.z.so",
                                     "../../base/startup/init/services/param/base/param_base.c",
                                     "/system/lib/libwallpapermanager.z.so",
                                     "/system/lib/libwallpaperextension.z.so",
                                     "/system/lib/module/libaccessibility.z.so",
                                     "/../base/startup/init/services/param/base/param_trie.c" };
    const char strrchrtesttag[] = { 'm', 'l', 's', 'o', 'z', 't', 'i', 'c', '\0' };
    const char *test = strrchrtestsrc[state.range(0)];
    const char ch = strrchrtesttag[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(strrchr(test, ch));
    }

    state.SetBytesProcessed(state.iterations());
}

// The selected range calculates the length
static void Bm_function_Strnlen(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t bmstrnlenAlignment = state.range(2);
    vector<char> bmstrnlen;
    char *bmstrnlenAligned = GetAlignedPtrFilled(&bmstrnlen, bmstrnlenAlignment, nbytes + 1, 'n');
    bmstrnlenAligned[nbytes - 1] = '\0';

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += strnlen(bmstrnlenAligned, limitsize);
    }

    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Specifies the maximum number of copies to replicate
static void Bm_function_Stpncpy(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t srcAlignment = state.range(2);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'z');
    char *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        stpncpy(dstAligned, srcAligned, limitsize);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strncpy(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t limitsize = state.range(1);
    const size_t srcAlignment = state.range(2);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'z');
    char *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    srcAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        stpncpy(dstAligned, srcAligned, limitsize);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Comparing whether two binary blocks of data are equal is functionally similar to MEMCMP
static void Bm_function_Bcmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<char> src;
    vector<char> dst;
    char *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, 'x');
    char *dstAligned = GetAlignedPtrFilled(&dst, dstAlignment, nbytes, 'x');

    volatile int c __attribute__((unused)) = 0;
    while (state.KeepRunning()) {
        c += bcmp(dstAligned, srcAligned, nbytes);
    }

    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Find the first character in a given string that matches any character in another specified string
static void Bm_function_Strpbrk(benchmark::State &state)
{
    const char *strpbrktestsrc[] = { "method", "setTimeout", "open.harmony",
                                     "libfilemgmt_libhilog", "libwallpaperextension",
                                     "startup", "libwallpapermanager", "param_trie" };
    const char *strpbrktesttag[] = { "th", "me", "enh", "lo", "en", "tu", "ag", "pa" };
    const char *src = strpbrktestsrc[state.range(0)];
    const char *tag = strpbrktesttag[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(strpbrk(src, tag));
    }
}

// Set the first n characters in the wide string to another wide character
static void Bm_function_Wmemset(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t alignment = state.range(1);

    vector<wchar_t> buf;
    wchar_t *bufAligned = GetAlignedPtr(&buf, alignment, nbytes + 1);

    while (state.KeepRunning()) {
        wmemset(bufAligned, L'n', nbytes);
    }

    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Wmemcpy(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t srcAlignment = state.range(1);
    const size_t dstAlignment = state.range(2);

    vector<wchar_t> src;
    vector<wchar_t> dst;
    wchar_t *srcAligned = GetAlignedPtrFilled(&src, srcAlignment, nbytes, L'z');
    wchar_t *dstAligned = GetAlignedPtr(&dst, dstAlignment, nbytes);
    while (state.KeepRunning()) {
        wmemcpy(dstAligned, srcAligned, nbytes);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

// Returns the index value of the first successful match
static void Bm_function_Strcspn(benchmark::State &state)
{
    const char *strcspnsrc[] = { "system/lib64", "system/lib64/chipset-pub-sdk", "vendor/lib64/chipsetsdk",
                                 "system/lib64/ndk", "system/lib64/platformsdk", "system/lib64/priv-platformsdk",
                                 "system/lib64/module/data", "tem/lib64/module/security" };
    const char *strcspntag[] = { "vendor/lib64", "/system/lib64/chipset-sdk", "/system/lib64/ndk",
                                 "lib64/chipset-pub-sdk", "priv-platformsdk", "/system/lib64/priv-module",
                                 "/system/lib64/module/multimedia", "/system/lib" };
    const char *src = strcspnsrc[state.range(0)];
    const char *tag = strcspntag[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcspn(src, tag));
    }
}

static void Bm_function_Strchrnul_exist(benchmark::State &state)
{
    const char *str = "musl.ld.debug.dlclose";
    int c = 46;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strchrnul(str, c));
    }
}

static void Bm_function_Strchrnul_noexist(benchmark::State &state)
{
    const char *str = "all";
    int c = 46;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strchrnul(str, c));
    }
}

static void Bm_function_Strchrnul(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    std::vector<char> haystack;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    haystackAligned[nbytes - 1] = '\0';

    while (state.KeepRunning()) {
        strchrnul(haystackAligned, '.');
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strcasecmp_capital_equal(benchmark::State &state)
{
    const char *l = "ABCDEF";
    const char *r = "ABCDEF";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp_small_equal(benchmark::State &state)
{
    const char *l = "abcdef";
    const char *r = "abcdef";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp_equal(benchmark::State &state)
{
    const char *l = "aBcDeD";
    const char *r = "ABCdEd";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp_not_equal(benchmark::State &state)
{
    const char *l = "bbcdef";
    const char *r = "bBCdEd";
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(l, r));
    }
}

static void Bm_function_Strcasecmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    const size_t needleAlignment = state.range(2);

    std::vector<char> haystack;
    std::vector<char> needle;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    char *needleAligned = GetAlignedPtrFilled(&needle, needleAlignment, nbytes, 'x');

    for (auto _ : state) {
        benchmark::DoNotOptimize(strcasecmp(haystackAligned, needleAligned));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strncasecmp(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    const size_t needleAlignment = state.range(2);

    std::vector<char> haystack;
    std::vector<char> needle;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    char *needleAligned = GetAlignedPtrFilled(&needle, needleAlignment, nbytes, 'x');
    for (auto _ : state) {
        benchmark::DoNotOptimize(strncasecmp(haystackAligned, needleAligned, nbytes));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strdup(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    std::vector<char> haystack;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    haystackAligned[nbytes - 1] = '\0';
    char* ptr = 0;
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(ptr = strdup(haystackAligned));
        free(ptr);
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

static void Bm_function_Strncat(benchmark::State &state)
{
    const size_t nbytes = state.range(0);
    const size_t haystackAlignment = state.range(1);
    std::vector<char> haystack;
    char *haystackAligned = GetAlignedPtrFilled(&haystack, haystackAlignment, nbytes, 'x');
    haystackAligned[nbytes - 1] = '\0';
    std::vector<char> dstStack;
    char *dst = GetAlignedPtrFilled(&dstStack, haystackAlignment, nbytes, '0');
    while (state.KeepRunning()) {
        dst[0] = 0;
        benchmark::DoNotOptimize(strncat(dst, haystackAligned, nbytes));
    }
    state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
}

BENCHMARK(Bm_function_Memchr)->Apply(StringtestArgs);
BENCHMARK(Bm_function_Strnlen)->Apply(StringtestArgs);
BENCHMARK(Bm_function_Stpncpy)->Apply(StringtestArgs);
BENCHMARK(Bm_function_Strncpy)->Apply(StringtestArgs);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strcspn, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Strchrnul_exist);
MUSL_BENCHMARK(Bm_function_Strchrnul_noexist);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strchrnul, "ALIGNED_ONEBUF");
MUSL_BENCHMARK(Bm_function_Strcasecmp_capital_equal);
MUSL_BENCHMARK(Bm_function_Strcasecmp_small_equal);
MUSL_BENCHMARK(Bm_function_Strcasecmp_equal);
MUSL_BENCHMARK(Bm_function_Strcasecmp_not_equal);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strcasecmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strncasecmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strrchr, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Bcmp, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strpbrk, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wmemset, "ALIGNED_ONEBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Wmemcpy, "ALIGNED_TWOBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strdup, "ALIGNED_ONEBUF");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strncat, "ALIGNED_ONEBUF");