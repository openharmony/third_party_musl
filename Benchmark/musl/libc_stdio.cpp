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
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"
#include "util.h"

using namespace std;

#define SSCANF_SIZE 32
#define BUFFERSIZE 100
char g_buffer[BUFFERSIZE];

int MyPrintfVf(FILE *stream, const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vfprintf(stream, format, args);
    va_end(args);
    return (ret);
}

int MyPrintfVs(const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vsprintf(g_buffer, format, args);
    va_end(args);
    return (ret);
}

int MyPrintfVsn(const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vsnprintf(g_buffer, BUFFERSIZE, format, args);
    va_end(args);
    return (ret);
}

static void Bm_function_Fopen_read(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "r");
        if (fp == nullptr) {
            perror("fopen read");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_write(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "w");
        if (fp == nullptr) {
            perror("fopen write");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_append(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "a");
        if (fp == nullptr) {
            perror("fopen append");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_rplus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "r+");
        if (fp == nullptr) {
            perror("fopen r+");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_wplus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "w+");
        if (fp == nullptr) {
            perror("fopen w+");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_append_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "a+");
        if (fp == nullptr) {
            perror("fopen a+");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_rb(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "rb");
        if (fp == nullptr) {
            perror("fopen rb");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_wb(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "wb");
        if (fp == nullptr) {
            perror("fopen wb");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_ab(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "ab");
        if (fp == nullptr) {
            perror("fopen ab");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_rb_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "rb+");
        if (fp == nullptr) {
            perror("fopen rb+");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_wb_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "wb+");
        if (fp == nullptr) {
            perror("fopen wb+");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_ab_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "ab+");
        if (fp == nullptr) {
            perror("fopen ab+");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fclose(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        FILE *fp = fopen("/dev/zero", "w+");
        if (fp == nullptr) {
            perror("fclose proc");
            exit(EXIT_FAILURE);
        }
        if (fp != nullptr) {
            state.ResumeTiming();
            benchmark::DoNotOptimize(fclose(fp));
        }
    }
}

// Used to convert a file descriptor to a file pointer
static void Bm_function_Fdopen(benchmark::State &state)
{
    for (auto _ : state) {
        int fp = open("/dev/zero", O_RDONLY, OPEN_MODE);
        FILE *fd = fdopen(fp, "r");
        if (fd == nullptr) {
            perror("fdopen");
            exit(EXIT_FAILURE);
        }
        benchmark::DoNotOptimize(fd);
        fclose(fd);
    }

    state.SetBytesProcessed(state.iterations());
}

// Use the parameter list to send formatted output to the stream.
// string type
static void Bm_function_Vfprintf_str(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    const char *arr1 = "hello";
    const char *arr2 = "world";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %s %s success", arr1, arr2));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Vfprintf_int(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    int arr1 = 233;
    int arr2 = 322;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %d %d success", arr1, arr2));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Vfprintf_float(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    float i = 22.33f;
    float j = 33.22f;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %f %f success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Vfprintf_longdouble(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    long double i = 2250996946.3365252546L;
    long double j = 9583454321234.226342465121L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %Lf %Lf success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Vfprintf_unsigned(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    unsigned int i = 4294967295U;
    unsigned int j = 3456264567U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %u %u success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Vfprintf_long(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    long i = 1234567890L;
    long j = 954611731L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %ld %ld success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Vfprintf_short(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    short i = 32767;
    short j = -32768;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %hd %hd success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Vfprintf_char(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    char i = 'n';
    char j = 'Z';
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %c %c success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// Use the parameter list to send formatted output to a string
// string type
static void Bm_function_Vsprintf_str(benchmark::State &state)
{
    const char *arr = "signal_stack";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%s", arr));
    }

    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Vsprintf_int(benchmark::State &state)
{
    int i = 2233;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%d", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Vsprintf_float(benchmark::State &state)
{
    float i = 22.33;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%f", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Vsprintf_longdouble(benchmark::State &state)
{
    long double i = 9583454321234.226342465121L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%Lf", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Vsprintf_unsigned(benchmark::State &state)
{
    unsigned int u = 4294967295U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%u", u));
    }

    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Vsprintf_long(benchmark::State &state)
{
    long l = 1234567890L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%ld", l));
    }

    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Vsprintf_short(benchmark::State &state)
{
    short s = 32767;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%hd", s));
    }

    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Vsprintf_char(benchmark::State &state)
{
    char c = 'Z';
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%c", c));
    }

    state.SetBytesProcessed(state.iterations());
}

// Prints a format string to a string buffer and can limit the maximum length of the formatted string that is printed
// string type
static void Bm_function_Vsnprintf_str(benchmark::State &state)
{
    const char *i = "holy";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %s", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Vsnprintf_int(benchmark::State &state)
{
    int i = 2233;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %d", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Vsnprintf_float(benchmark::State &state)
{
    float i = 22.33;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %f", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Vsnprintf_longdouble(benchmark::State &state)
{
    long double i = 23423523.769563665L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %Lf", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Vsnprintf_unsigned(benchmark::State &state)
{
    unsigned int u = 4294967295U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %u", u));
    }

    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Vsnprintf_long(benchmark::State &state)
{
    long l = 1234567890L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %ld", l));
    }

    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Vsnprintf_short(benchmark::State &state)
{
    short s = 32767;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %hd", s));
    }

    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Vsnprintf_char(benchmark::State &state)
{
    char s = 'R';
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %c", s));
    }

    state.SetBytesProcessed(state.iterations());
}

// Wait for the file to no longer be locked by another thread, then make the current thread the owner of the file
// and then increase the lock count and decrease the lock count
static void Bm_function_Flock_Funlockfile(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("fopen funlockfile");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        flockfile(fp);
        funlockfile(fp);
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Rename(benchmark::State &state)
{
    mkdir("/data/data/test_rename", S_IRWXU);
    char oldname[32] = "/data/data/test_rename";
    char newname[32] = "test_newname";
    for (auto _ : state) {
        benchmark::DoNotOptimize(rename(oldname, newname));
    }
    rmdir(newname);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fseek_set(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek set");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseek(f, 0, SEEK_SET));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

#define OFFSET_SIZE 10L
static void Bm_function_Fseek_cur(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek cur");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseek(f, OFFSET_SIZE, SEEK_CUR));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fseek_end(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek end");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseek(f, -OFFSET_SIZE, SEEK_END));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Sscanf_int(benchmark::State &state)
{
    for (auto _ : state) {
        int year, month, day;
        benchmark::DoNotOptimize(sscanf("20230515", "%04d%02d%02d", &year, &month, &day));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_double(benchmark::State &state)
{
    double longitude, latitude;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sscanf("113.123456789 31.123456789", "%lf %lf", &longitude, &latitude));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_char(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("123456abcdedf", "%31[0-9]", str));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_char1(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("test/unique_11@qq.com", "%*[^/]/%[^@]", str));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_str(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("123456abcdedfBCDEF", "%[1-9A-Z]", str));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_str1(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("test TEST", "%*s%s", str));
    }
    state.SetBytesProcessed(state.iterations());
}

int MyScanf1(FILE *stream, const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vfscanf(stream, format, args);
    va_end(args);
    return (ret);
}

int MyVfscanf(const char* str, const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vsscanf(str, format, args);
    va_end(args);
    return (ret);
}

static void Bm_function_Vfscanf_str(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_str.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf str");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%s", "vfscanfStrTest") < 0) {
        perror("fprintf vfscanf str");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        char str[1024] = {'0'};
        benchmark::DoNotOptimize(MyScanf1(stream, "%s", str));
    }
    fclose(stream);
    remove("/data/data/vfscanf_str.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_int(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_int.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf int");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%d", 123) < 0) {
        perror("fprintf vfscanf int");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%d", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_int.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_double(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_double.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf double");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%lf", 123.4567) < 0) {
        perror("fprintf vfscanf double");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%d", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_double.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_float(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_float.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf float");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%f", 40.0) < 0) {
        perror("fprintf vfscanf float");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        float val = 0.0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%f", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_float.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_char(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_char.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf char");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%c", 'a') < 0) {
        perror("fprintf vfscanf char");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        char val = ' ';
        benchmark::DoNotOptimize(MyScanf1(stream, "%c", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_char.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_iformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_iformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf iformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%i", -1) < 0) {
        perror("fprintf vfscanf iformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        signed int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%i", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_iformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_oformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_oformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf oformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%o", 0123) < 0) {
        perror("fprintf vfscanf oformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        unsigned int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%o", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_oformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_uformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_uformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf uformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%u", 1024) < 0) {
        perror("fprintf vfscanf uformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        unsigned int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%u", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_uformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_xformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_xformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf xformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%x", 0xabc) < 0) {
        perror("fprintf vfscanf xformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        int val = 0x000;
        benchmark::DoNotOptimize(MyScanf1(stream, "%x", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_xformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_Xformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_Xformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf Xformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%X", 0xabc) < 0) {
        perror("fprintf vfscanf Xformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        int val = 0x000;
        benchmark::DoNotOptimize(MyScanf1(stream, "%X", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_Xformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_eformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_eformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf eformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%.2e", 123456.0) < 0) {
        perror("fprintf vfscanf eformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        float val = 0.0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%.2e", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_eformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_gformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_gformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf gformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%g", 12.3) < 0) {
        perror("fprintf vfscanf gformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        float val = 12.3;
        benchmark::DoNotOptimize(MyScanf1(stream, "%g", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_gformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_ldformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_gformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf ldformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%ld", 2147483646L) < 0) {
        perror("fprintf vfscanf ldformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        long int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%ld", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_ldformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_luformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_luformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf luformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%lu", 4294967294UL) < 0) {
        perror("fprintf vfscanf luformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        unsigned long val = 0UL;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lu", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_luformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_lxformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_lxformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lxformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%lx", 0x41L) < 0) {
        perror("fprintf vfscanf lxformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        long val = 0x0L;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lx", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_lxformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_loformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_loformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf loformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%lo", 0234L) < 0) {
        perror("fprintf vfscanf loformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        long val = 00L;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lo", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_loformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_hdformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_hdformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf hdformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%hd", (short)144) < 0) {
        perror("fprintf vfscanf hdformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        short int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hd", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_hdformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_huformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_huformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf huformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%hu", (unsigned short)256) < 0) {
        perror("fprintf vfscanf huformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        unsigned short int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hu", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_huformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_hhuformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_hhuformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf hhuformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%hhu", (unsigned char)256) < 0) {
        perror("fprintf vfscanf hhuformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        unsigned char val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hhu", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_hhuformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_hhxformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_hhxformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf hhxformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%hhx", (unsigned char)0x23) < 0) {
        perror("fprintf vfscanf hhxformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        short val = 0x0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hhx", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_hhxformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_llxformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_llxformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf llxformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%llx", 0x6543LL) < 0) {
        perror("fprintf vfscanf llxformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        long long val = 0x0LL;
        benchmark::DoNotOptimize(MyScanf1(stream, "%llx", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_llxformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_lldformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_lldformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lldformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%lld", -23234534LL) < 0) {
        perror("fprintf vfscanf lldformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        long long val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lld", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_lldformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_lluformat(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_lluformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lluformat");
        exit(EXIT_FAILURE);
    }
    if (fprintf(stream, "%llu", 23234534LL) < 0) {
        perror("fprintf vfscanf lluformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        unsigned long long val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%llu", &val));
    }
    fclose(stream);
    remove("/data/data/vfscanf_lluformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fileno_unlocked(benchmark::State &state)
{
    FILE *stream = fopen("/data/data/vfscanf_lluformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lluformat");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fileno_unlocked(stream));
    }
    fclose(stream);
    remove("/data/data/vfscanf_lluformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fseek_fflush(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek set");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fflush(f));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}


static void Bm_function_Sscanf_vsscanf_int(benchmark::State &state)
{
    int year, month, day;
    const char* src = "20230515";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyVfscanf(src, "%04d%02d%02d", &year, &month, &day));
    }
}

static void Bm_function_Feof(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("feof");
        exit(EXIT_FAILURE);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(feof(fp));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Fopen_read);
MUSL_BENCHMARK(Bm_function_Fopen_write);
MUSL_BENCHMARK(Bm_function_Fopen_append);
MUSL_BENCHMARK(Bm_function_Fopen_rplus);
MUSL_BENCHMARK(Bm_function_Fopen_wplus);
MUSL_BENCHMARK(Bm_function_Fopen_append_plus);
MUSL_BENCHMARK(Bm_function_Fopen_rb);
MUSL_BENCHMARK(Bm_function_Fopen_wb);
MUSL_BENCHMARK(Bm_function_Fopen_ab);
MUSL_BENCHMARK(Bm_function_Fopen_rb_plus);
MUSL_BENCHMARK(Bm_function_Fopen_wb_plus);
MUSL_BENCHMARK(Bm_function_Fopen_ab_plus);
MUSL_BENCHMARK(Bm_function_Fclose);
MUSL_BENCHMARK(Bm_function_Fdopen);
MUSL_BENCHMARK(Bm_function_Vfprintf_str);
MUSL_BENCHMARK(Bm_function_Vfprintf_int);
MUSL_BENCHMARK(Bm_function_Vfprintf_float);
MUSL_BENCHMARK(Bm_function_Vfprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Vfprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Vfprintf_long);
MUSL_BENCHMARK(Bm_function_Vfprintf_short);
MUSL_BENCHMARK(Bm_function_Vfprintf_char);
MUSL_BENCHMARK(Bm_function_Vsprintf_str);
MUSL_BENCHMARK(Bm_function_Vsprintf_int);
MUSL_BENCHMARK(Bm_function_Vsprintf_float);
MUSL_BENCHMARK(Bm_function_Vsprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Vsprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Vsprintf_long);
MUSL_BENCHMARK(Bm_function_Vsprintf_short);
MUSL_BENCHMARK(Bm_function_Vsprintf_char);
MUSL_BENCHMARK(Bm_function_Vsnprintf_str);
MUSL_BENCHMARK(Bm_function_Vsnprintf_int);
MUSL_BENCHMARK(Bm_function_Vsnprintf_float);
MUSL_BENCHMARK(Bm_function_Vsnprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Vsnprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Vsnprintf_long);
MUSL_BENCHMARK(Bm_function_Vsnprintf_short);
MUSL_BENCHMARK(Bm_function_Vsnprintf_char);
MUSL_BENCHMARK(Bm_function_Flock_Funlockfile);
MUSL_BENCHMARK(Bm_function_Rename);
MUSL_BENCHMARK(Bm_function_Fseek_set);
MUSL_BENCHMARK(Bm_function_Fseek_cur);
MUSL_BENCHMARK(Bm_function_Fseek_end);
MUSL_BENCHMARK(Bm_function_Sscanf_int);
MUSL_BENCHMARK(Bm_function_Sscanf_double);
MUSL_BENCHMARK(Bm_function_Sscanf_str);
MUSL_BENCHMARK(Bm_function_Sscanf_str1);
MUSL_BENCHMARK(Bm_function_Sscanf_char);
MUSL_BENCHMARK(Bm_function_Sscanf_char1);
MUSL_BENCHMARK(Bm_function_Vfscanf_str);
MUSL_BENCHMARK(Bm_function_Vfscanf_int);
MUSL_BENCHMARK(Bm_function_Vfscanf_double);
MUSL_BENCHMARK(Bm_function_Vfscanf_float);
MUSL_BENCHMARK(Bm_function_Vfscanf_char);
MUSL_BENCHMARK(Bm_function_Vfscanf_iformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_oformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_uformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_xformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_Xformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_eformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_gformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_ldformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_luformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_lxformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_loformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_hdformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_huformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_hhuformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_hhxformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_llxformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_lldformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_lluformat);
MUSL_BENCHMARK(Bm_function_Fileno_unlocked);
MUSL_BENCHMARK(Bm_function_Fseek_fflush);
MUSL_BENCHMARK(Bm_function_Sscanf_vsscanf_int);
MUSL_BENCHMARK(Bm_function_Feof);
