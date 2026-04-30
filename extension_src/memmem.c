/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : memmem_0100
 * @tc.desc      : Find needle at beginning of haystack
 * @tc.level     : Level 0
 */
void memmem_0100(void)
{
    const char buffer[] = "musl test";
    const char cmp[] = "musl";
    char *ptr = NULL;
    
    ptr = memmem(buffer, sizeof(buffer), cmp, strlen(cmp));
    if (ptr == NULL) {
        t_error("%s Failed to find needle at beginning\n", __func__);
        return;
    }
    if (ptr != buffer) {
        t_error("%s Returned wrong pointer\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0200
 * @tc.desc      : Find needle in middle of haystack
 * @tc.level     : Level 0
 */
void memmem_0200(void)
{
    const char buffer[] = "hello world test";
    const char cmp[] = "world";
    char *ptr = NULL;
    
    ptr = memmem(buffer, sizeof(buffer), cmp, strlen(cmp));
    if (ptr == NULL) {
        t_error("%s Failed to find needle in middle\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {  // "world" starts at position 6
        t_error("%s Returned wrong pointer\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0300
 * @tc.desc      : Find needle at end of haystack
 * @tc.level     : Level 0
 */
void memmem_0300(void)
{
    const char buffer[] = "hello world";
    const char cmp[] = "world";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, strlen(cmp));
    if (ptr == NULL) {
        t_error("%s Failed to find needle at end\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {
        t_error("%s Returned wrong pointer\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0400
 * @tc.desc      : Empty needle (should return haystack)
 * @tc.level     : Level 1
 */
void memmem_0400(void)
{
    const char buffer[] = "test";
    const char cmp[] = "";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, 0);
    if (ptr == NULL) {
        t_error("%s Empty needle should return haystack\n", __func__);
        return;
    }
    if (ptr != buffer) {
        t_error("%s Empty needle should return start of haystack\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0500
 * @tc.desc      : Needle longer than haystack (should return NULL)
 * @tc.level     : Level 1
 */
void memmem_0500(void)
{
    const char buffer[] = "short";
    const char cmp[] = "very long needle";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, strlen(cmp));
    if (ptr != NULL) {
        t_error("%s Needle longer than haystack should return NULL\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0600
 * @tc.desc      : Needle not found
 * @tc.level     : Level 0
 */
void memmem_0600(void)
{
    const char buffer[] = "hello world";
    const char cmp[] = "xyz";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, strlen(cmp));
    if (ptr != NULL) {
        t_error("%s Needle not found should return NULL\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0700
 * @tc.desc      : Single character needle (l==1 optimization)
 * @tc.level     : Level 1
 */
void memmem_0700(void)
{
    const char buffer[] = "hello world";
    const char cmp[] = "w";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, 1);
    if (ptr == NULL) {
        t_error("%s Failed to find single character\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {  // 'w' at position 6
        t_error("%s Wrong pointer for single character\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0800
 * @tc.desc      : Two character needle (l==2 optimization)
 * @tc.level     : Level 1
 */
void memmem_0800(void)
{
    const char buffer[] = "hello world";
    const char cmp[] = "wo";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, 2);
    if (ptr == NULL) {
        t_error("%s Failed to find two-character needle\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {  // "wo" starts at position 6
        t_error("%s Wrong pointer for two-character needle\n", __func__);
    }
}

/**
 * @tc.name      : memmem_0900
 * @tc.desc      : Three character needle (l==3 optimization)
 * @tc.level     : Level 1
 */
void memmem_0900(void)
{
    const char buffer[] = "hello world";
    const char cmp[] = "wor";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, 3);
    if (ptr == NULL) {
        t_error("%s Failed to find three-character needle\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {  // "wor" starts at position 6
        t_error("%s Wrong pointer for three-character needle\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1000
 * @tc.desc      : Four character needle (l==4 optimization)
 * @tc.level     : Level 1
 */
void memmem_1000(void)
{
    const char buffer[] = "hello world";
    const char cmp[] = "worl";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, 4);
    if (ptr == NULL) {
        t_error("%s Failed to find four-character needle\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {  // "worl" starts at position 6
        t_error("%s Wrong pointer for four-character needle\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1100
 * @tc.desc      : Five character needle (uses twoway algorithm)
 * @tc.level     : Level 1
 */
void memmem_1100(void)
{
    const char buffer[] = "hello world test";
    const char cmp[] = "world";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, 5);
    if (ptr == NULL) {
        t_error("%s Failed to find five-character needle\n", __func__);
        return;
    }
    if (ptr != buffer + 6) {  // "world" starts at position 6
        t_error("%s Wrong pointer for five-character needle\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1200
 * @tc.desc      : Overlapping pattern in haystack
 * @tc.level     : Level 2
 */
void memmem_1200(void)
{
    const char buffer[] = "abcabcabc";
    const char cmp[] = "abcabc";
    char *ptr = NULL;
    
    ptr = memmem(buffer, strlen(buffer), cmp, strlen(cmp));
    if (ptr == NULL) {
        t_error("%s Failed to find overlapping pattern\n", __func__);
        return;
    }
    if (ptr != buffer) {  // Should find at beginning
        t_error("%s Wrong pointer for overlapping pattern\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1300
 * @tc.desc      : Binary data search (not null-terminated)
 * @tc.level     : Level 2
 */
void memmem_1300(void)
{
    unsigned char buffer[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    unsigned char cmp[] = {0x03, 0x04, 0x05};
    char *ptr = NULL;
    
    ptr = memmem(buffer, sizeof(buffer), cmp, sizeof(cmp));
    if (ptr == NULL) {
        t_error("%s Failed to find binary pattern\n", __func__);
        return;
    }
    if (ptr != (char *)(buffer + 3)) {  // Pattern starts at index 3
        t_error("%s Wrong pointer for binary pattern\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1400
 * @tc.desc      : Needle with null byte in middle
 * @tc.level     : Level 2
 */
void memmem_1400(void)
{
    const char buffer[] = "hello\0world";
    const char cmp[] = "lo";
    char *ptr = NULL;
    
    // Search in first 5 characters (before null)
    ptr = memmem(buffer, 5, cmp, 2);
    if (ptr == NULL) {
        t_error("%s Failed to find needle before null\n", __func__);
        return;
    }
    if (ptr != buffer + 3) {  // "lo" starts at position 3
        t_error("%s Wrong pointer for needle with null\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1500
 * @tc.desc      : Partial match at end of haystack
 * @tc.level     : Level 2
 */
void memmem_1500(void)
{
    const char buffer[] = "hello wor";
    const char cmp[] = "world";
    char *ptr = NULL;
    
    // "wor" matches but "ld" is missing
    ptr = memmem(buffer, strlen(buffer), cmp, strlen(cmp));
    if (ptr != NULL) {
        t_error("%s Partial match should return NULL\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1600
 * @tc.desc      : First character found but needle not complete
 * @tc.level     : Level 2
 */
void memmem_1600(void)
{
    const char buffer[] = "abababc";
    const char cmp[] = "ababc";
    char *ptr = NULL;
    
    // Tests the memchr optimization failure
    ptr = memmem(buffer, strlen(buffer), cmp, strlen(cmp));
    if (ptr == NULL) {
        t_error("%s Failed to find pattern after memchr\n", __func__);
        return;
    }
    if (ptr != buffer + 2) {  // "ababc" starts at position 2
        t_error("%s Wrong pointer for partial initial match\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1700
 * @tc.desc      : Zero-length haystack
 * @tc.level     : Level 2
 */
void memmem_1700(void)
{
    const char buffer[] = "test";
    const char cmp[] = "test";
    char *ptr = NULL;
    
    ptr = memmem(buffer, 0, cmp, strlen(cmp));
    if (ptr != NULL) {
        t_error("%s Zero-length haystack should return NULL\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1800
 * @tc.desc      : Empty needle with zero-length haystack
 * @tc.level     : Level 2
 */
void memmem_1800(void)
{
    const char buffer[] = "";
    const char cmp[] = "";
    char *ptr = NULL;
    
    ptr = memmem(buffer, 0, cmp, 0);
    if (ptr == NULL) {
        t_error("%s Empty needle and haystack should return haystack\n", __func__);
        return;
    }
    if (ptr != buffer) {
        t_error("%s Should return start of haystack\n", __func__);
    }
}

/**
 * @tc.name      : memmem_1900
 * @tc.desc      : Very long needle (tests twoway algorithm)
 * @tc.level     : Level 2
 */
void memmem_1900(void)
{
    char buffer[1000];
    char needle[100];
    
    // Fill buffer with pattern
    for (int i = 0; i < sizeof(buffer); i++) {
        buffer[i] = 'A' + (i % 26);
    }
    
    // Create needle in middle
    memcpy(needle, buffer + 500, sizeof(needle));
    
    char *ptr = memmem(buffer, sizeof(buffer), needle, sizeof(needle));
    if (ptr == NULL) {
        t_error("%s Failed to find long needle\n", __func__);
        return;
    }
    if (ptr != buffer + 500) {
        t_error("%s Wrong pointer for long needle\n", __func__);
    }
}

/**
 * @tc.name      : memmem_2000
 * @tc.desc      : memchr returns pointer near end, not enough space
 * @tc.level     : Level 2
 */
void memmem_2000(void)
{
    const char buffer[] = "xxxxxyyyyy";
    const char cmp[] = "yyyyy";
    char *ptr = NULL;
    
    // Search only first 7 characters
    ptr = memmem(buffer, 7, cmp, 5);
    if (ptr != NULL) {
        t_error("%s Not enough space after memchr should return NULL\n", __func__);
    }
}

int main(void)
{
    memmem_0100();
    memmem_0200();
    memmem_0300();
    memmem_0400();
    memmem_0500();
    memmem_0600();
    memmem_0700();
    memmem_0800();
    memmem_0900();
    memmem_1000();
    memmem_1100();
    memmem_1200();
    memmem_1300();
    memmem_1400();
    memmem_1500();
    memmem_1600();
    memmem_1700();
    memmem_1800();
    memmem_1900();
    memmem_2000();

    return t_status;
}