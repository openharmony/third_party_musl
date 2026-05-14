/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "functionalext.h"

#ifndef MUSL_EXTERNAL_FUNCTION

struct qsort_r_stub_ctx {
    int calls;
};

static int qsort_r_stub_compare(const void *left, const void *right, void *arg)
{
    struct qsort_r_stub_ctx *ctx = arg;

    (void)left;
    (void)right;
    ++ctx->calls;
    return 0;
}

/**
 * @tc.name      : qsort_r_0100
 * @tc.desc      : Verify that qsort_r reports ENOSYS and keeps input unchanged when MUSL_EXTERNAL_FUNCTION is disabled
 * @tc.level     : Level 0
 */
static void qsort_r_0100(void)
{
    int values[] = {9, 2, 7, 11, 3, 87, 34, 6};
    int expected[] = {9, 2, 7, 11, 3, 87, 34, 6};
    struct qsort_r_stub_ctx ctx = {0};

    errno = 0;
    qsort_r(values, sizeof(values) / sizeof(values[0]), sizeof(values[0]), qsort_r_stub_compare, &ctx);

    EXPECT_EQ("qsort_r_0100", errno, ENOSYS);
    EXPECT_EQ("qsort_r_0100", ctx.calls, 0);
    EXPECT_TRUE("qsort_r_0100", memcmp(values, expected, sizeof(values)) == 0);
}

/**
 * @tc.name      : qsort_r_0200
 * @tc.desc      : Verify that qsort_r still reports ENOSYS for zero or one element when MUSL_EXTERNAL_FUNCTION is disabled
 * @tc.level     : Level 1
 */
static void qsort_r_0200(void)
{
    int value = 1234;
    struct qsort_r_stub_ctx ctx = {0};

    errno = 0;
    qsort_r(&value, 0, sizeof(value), qsort_r_stub_compare, &ctx);
    EXPECT_EQ("qsort_r_0200", errno, ENOSYS);
    EXPECT_EQ("qsort_r_0200", value, 1234);
    EXPECT_EQ("qsort_r_0200", ctx.calls, 0);

    errno = 0;
    qsort_r(&value, 1, sizeof(value), qsort_r_stub_compare, &ctx);
    EXPECT_EQ("qsort_r_0200", errno, ENOSYS);
    EXPECT_EQ("qsort_r_0200", value, 1234);
    EXPECT_EQ("qsort_r_0200", ctx.calls, 0);
}

#else

#define QSORT_R_LARGE_SIZE 47
#define QSORT_R_MAX_ELEMS  64
#define QSORT_R_MAX_WIDTH  QSORT_R_LARGE_SIZE

enum qsort_r_kind {
    QSORT_R_KIND_INT,
    QSORT_R_KIND_U8,
    QSORT_R_KIND_U16,
    QSORT_R_KIND_U32,
    QSORT_R_KIND_U64,
    QSORT_R_KIND_LARGE,
};

enum qsort_r_pattern {
    QSORT_R_PATTERN_SORTED,
    QSORT_R_PATTERN_SHUFFLED,
    QSORT_R_PATTERN_REPEATED,
    QSORT_R_PATTERN_BITONIC,
    QSORT_R_PATTERN_DUPLICATED,
};

struct qsort_r_ctx {
    unsigned char *base;
    size_t nmemb;
    size_t width;
    enum qsort_r_kind kind;
    int descending;
    int calls;
    int out_of_range;
};

struct qsort_r_case_desc {
    enum qsort_r_kind kind;
    size_t width;
    const char *name;
};

struct qsort_r_pattern_desc {
    enum qsort_r_pattern pattern;
    const char *name;
};

static unsigned char *qsort_r_elem(void *base, size_t index, size_t width)
{
    return (unsigned char *)base + index * width;
}

static uint32_t qsort_r_next_u32(uint32_t *state)
{
    *state = (*state * 1103515245u) + 12345u;
    return *state;
}

static void qsort_r_store_large(unsigned char *elem, unsigned char value)
{
    size_t i;

    for (i = 0; i < QSORT_R_LARGE_SIZE; ++i) {
        elem[i] = (unsigned char)(value + i);
    }
}

static void qsort_r_store_value(void *elem, enum qsort_r_kind kind, uint32_t value)
{
    switch (kind) {
        case QSORT_R_KIND_INT:
            *(int *)elem = (int)value;
            break;
        case QSORT_R_KIND_U8:
            *(uint8_t *)elem = (uint8_t)value;
            break;
        case QSORT_R_KIND_U16:
            *(uint16_t *)elem = (uint16_t)value;
            break;
        case QSORT_R_KIND_U32:
            *(uint32_t *)elem = (uint32_t)value;
            break;
        case QSORT_R_KIND_U64:
            *(uint64_t *)elem = (uint64_t)value;
            break;
        case QSORT_R_KIND_LARGE:
            qsort_r_store_large(elem, (unsigned char)value);
            break;
        default:
            break;
    }
}

static int qsort_r_value_order(const void *left, const void *right, const struct qsort_r_ctx *ctx)
{
    int ret;

    switch (ctx->kind) {
        case QSORT_R_KIND_INT: {
            int lhs = *(const int *)left;
            int rhs = *(const int *)right;
            ret = (lhs > rhs) - (lhs < rhs);
            break;
        }
        case QSORT_R_KIND_U8: {
            uint8_t lhs = *(const uint8_t *)left;
            uint8_t rhs = *(const uint8_t *)right;
            ret = (lhs > rhs) - (lhs < rhs);
            break;
        }
        case QSORT_R_KIND_U16: {
            uint16_t lhs = *(const uint16_t *)left;
            uint16_t rhs = *(const uint16_t *)right;
            ret = (lhs > rhs) - (lhs < rhs);
            break;
        }
        case QSORT_R_KIND_U32: {
            uint32_t lhs = *(const uint32_t *)left;
            uint32_t rhs = *(const uint32_t *)right;
            ret = (lhs > rhs) - (lhs < rhs);
            break;
        }
        case QSORT_R_KIND_U64: {
            uint64_t lhs = *(const uint64_t *)left;
            uint64_t rhs = *(const uint64_t *)right;
            ret = (lhs > rhs) - (lhs < rhs);
            break;
        }
        case QSORT_R_KIND_LARGE:
            ret = memcmp(left, right, QSORT_R_LARGE_SIZE);
            ret = (ret > 0) - (ret < 0);
            break;
        default:
            ret = 0;
            break;
    }

    if (ctx->descending) {
        ret = -ret;
    }
    return ret;
}

static int qsort_r_compare(const void *left, const void *right, void *arg)
{
    struct qsort_r_ctx *ctx = arg;
    unsigned char *limit;

    ctx->calls++;
    limit = ctx->base + (ctx->nmemb * ctx->width);
    if ((const unsigned char *)left < ctx->base || (const unsigned char *)left >= limit ||
        (const unsigned char *)right < ctx->base || (const unsigned char *)right >= limit) {
        ctx->out_of_range = 1;
    }

    return qsort_r_value_order(left, right, ctx);
}

static void qsort_r_reference_sort(void *base, size_t nmemb, size_t width, struct qsort_r_ctx *ctx)
{
    unsigned char tmp[QSORT_R_MAX_WIDTH];
    size_t i;
    size_t j;

    if (nmemb <= 1) {
        return;
    }

    for (i = 1; i < nmemb; ++i) {
        memcpy(tmp, qsort_r_elem(base, i, width), width);
        j = i;
        while (j > 0 && qsort_r_value_order(qsort_r_elem(base, j - 1, width), tmp, ctx) > 0) {
            memmove(qsort_r_elem(base, j, width), qsort_r_elem(base, j - 1, width), width);
            --j;
        }
        memcpy(qsort_r_elem(base, j, width), tmp, width);
    }
}

static void qsort_r_fill_pattern(unsigned char *buffer, size_t nmemb, size_t width, enum qsort_r_kind kind,
    enum qsort_r_pattern pattern)
{
    uint32_t state = (uint32_t)(0x13572468u + width * 17u + nmemb * 131u + pattern * 977u);
    size_t i;

    for (i = 0; i < nmemb; ++i) {
        uint32_t value;

        switch (pattern) {
            case QSORT_R_PATTERN_SORTED:
                value = (uint32_t)i;
                break;
            case QSORT_R_PATTERN_SHUFFLED:
                value = qsort_r_next_u32(&state) % 91u;
                break;
            case QSORT_R_PATTERN_REPEATED:
                value = qsort_r_next_u32(&state) % 67u;
                if ((i % 4u) == 0) {
                    value = 21u;
                }
                break;
            case QSORT_R_PATTERN_BITONIC:
                if (i < (nmemb / 2u)) {
                    value = (uint32_t)i;
                } else {
                    value = (uint32_t)(nmemb - 1u - i);
                }
                break;
            case QSORT_R_PATTERN_DUPLICATED:
                value = ((i % 5u) == 0) ? 7u : 54u;
                break;
            default:
                value = 0;
                break;
        }

        qsort_r_store_value(buffer + i * width, kind, value);
    }
}

/**
 * @tc.name      : qsort_r_0100
 * @tc.desc      : Verify that qsort_r sorts integers with a comparator context
 * @tc.level     : Level 0
 */
static void qsort_r_0100(void)
{
    int values[] = {9, 2, 7, 11, 3, 87, 34, 6};
    int expected[] = {87, 34, 11, 9, 7, 6, 3, 2};
    struct qsort_r_ctx ctx = {
        .base = (unsigned char *)values,
        .nmemb = sizeof(values) / sizeof(values[0]),
        .width = sizeof(values[0]),
        .kind = QSORT_R_KIND_INT,
        .descending = 1,
    };

    qsort_r(values, ctx.nmemb, ctx.width, qsort_r_compare, &ctx);

    EXPECT_TRUE("qsort_r_0100", memcmp(values, expected, sizeof(values)) == 0);
    EXPECT_TRUE("qsort_r_0100", ctx.calls > 0);
    EXPECT_EQ("qsort_r_0100", ctx.out_of_range, 0);
}

/**
 * @tc.name      : qsort_r_0200
 * @tc.desc      : Verify that qsort_r does not call the comparator for zero or one element
 * @tc.level     : Level 1
 */
static void qsort_r_0200(void)
{
    int value = 1234;
    struct qsort_r_ctx ctx = {
        .base = (unsigned char *)&value,
        .nmemb = 0,
        .width = sizeof(value),
        .kind = QSORT_R_KIND_INT,
    };

    qsort_r(&value, 0, sizeof(value), qsort_r_compare, &ctx);
    EXPECT_EQ("qsort_r_0200", value, 1234);
    EXPECT_EQ("qsort_r_0200", ctx.calls, 0);
    EXPECT_EQ("qsort_r_0200", ctx.out_of_range, 0);

    ctx.nmemb = 1;
    qsort_r(&value, 1, sizeof(value), qsort_r_compare, &ctx);
    EXPECT_EQ("qsort_r_0200", value, 1234);
    EXPECT_EQ("qsort_r_0200", ctx.calls, 0);
    EXPECT_EQ("qsort_r_0200", ctx.out_of_range, 0);
}

/**
 * @tc.name      : qsort_r_0300
 * @tc.desc      : Verify qsort_r against a reference sort across data widths and input patterns
 * @tc.level     : Level 1
 */
static void qsort_r_0300(void)
{
    static const struct qsort_r_case_desc cases[] = {
        {QSORT_R_KIND_U8, sizeof(uint8_t), "u8"},
        {QSORT_R_KIND_U16, sizeof(uint16_t), "u16"},
        {QSORT_R_KIND_U32, sizeof(uint32_t), "u32"},
        {QSORT_R_KIND_U64, sizeof(uint64_t), "u64"},
        {QSORT_R_KIND_LARGE, QSORT_R_LARGE_SIZE, "large"},
    };
    static const struct qsort_r_pattern_desc patterns[] = {
        {QSORT_R_PATTERN_SORTED, "sorted"},
        {QSORT_R_PATTERN_SHUFFLED, "shuffled"},
        {QSORT_R_PATTERN_REPEATED, "repeated"},
        {QSORT_R_PATTERN_BITONIC, "bitonic"},
        {QSORT_R_PATTERN_DUPLICATED, "duplicated"},
    };
    static const size_t counts[] = {0, 1, 7, 16, 63};
    unsigned char actual[QSORT_R_MAX_ELEMS * QSORT_R_MAX_WIDTH];
    unsigned char reference[QSORT_R_MAX_ELEMS * QSORT_R_MAX_WIDTH];
    size_t i;
    size_t j;
    size_t k;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        for (j = 0; j < sizeof(patterns) / sizeof(patterns[0]); ++j) {
            for (k = 0; k < sizeof(counts) / sizeof(counts[0]); ++k) {
                size_t count = counts[k];
                size_t bytes = count * cases[i].width;
                struct qsort_r_ctx ctx = {
                    .base = actual,
                    .nmemb = count,
                    .width = cases[i].width,
                    .kind = cases[i].kind,
                    .descending = 0,
                };

                memset(actual, 0, sizeof(actual));
                memset(reference, 0, sizeof(reference));
                qsort_r_fill_pattern(actual, count, cases[i].width, cases[i].kind, patterns[j].pattern);
                memcpy(reference, actual, bytes);

                qsort_r(actual, count, cases[i].width, qsort_r_compare, &ctx);
                qsort_r_reference_sort(reference, count, cases[i].width, &ctx);

                if (count <= 1) {
                    EXPECT_EQ("qsort_r_0300", ctx.calls, 0);
                } else {
                    EXPECT_TRUE("qsort_r_0300", ctx.calls > 0);
                }
                EXPECT_EQ("qsort_r_0300", ctx.out_of_range, 0);
                if (memcmp(actual, reference, bytes) != 0) {
                    t_error("[qsort_r_0300] failed kind=%s pattern=%s count=%zu\n",
                        cases[i].name, patterns[j].name, count);
                }
            }
        }
    }
}

#endif

int main(void)
{
#ifndef MUSL_EXTERNAL_FUNCTION
    qsort_r_0100();
    qsort_r_0200();
#else
    qsort_r_0100();
    qsort_r_0200();
    qsort_r_0300();
#endif
    return t_status;
}
