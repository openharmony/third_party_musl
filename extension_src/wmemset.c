/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wmemset_0100
 * @tc.desc      : Fill array with wide character
 * @tc.level     : Level 0
 */
void wmemset_0100(void)
{
    const int index = 20;
    int count = 10;
    wchar_t ch = L'C';
    wchar_t des[index];
    
    wmemset(des, ch, count);
    for (int i = 0; i < count; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is %lc, not %lc\n", __func__, i, des[i], ch);
        }
    }
    
    // Verify remaining elements are untouched
    for (int i = count; i < index; i++) {
        // des[i] contains uninitialized data, can't verify
    }
}

/**
 * @tc.name      : wmemset_0200
 * @tc.desc      : Fill entire array
 * @tc.level     : Level 1
 */
void wmemset_0200(void)
{
    const int size = 10;
    wchar_t ch = L'X';
    wchar_t des[size];
    
    wmemset(des, ch, size);
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is %lc, not %lc\n", __func__, i, des[i], ch);
        }
    }
}

/**
 * @tc.name      : wmemset_0300
 * @tc.desc      : n = 0, should do nothing
 * @tc.level     : Level 1
 */
void wmemset_0300(void)
{
    wchar_t des[10] = {L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J'};
    wchar_t backup[10];
    wchar_t ch = L'Z';
    
    // Backup original data
    for (int i = 0; i < 10; i++) {
        backup[i] = des[i];
    }
    
    wmemset(des, ch, 0);
    
    // Verify nothing changed
    for (int i = 0; i < 10; i++) {
        if (des[i] != backup[i]) {
            t_error("%s des[%d] changed when n=0: %lc -> %lc\n", 
                    __func__, i, backup[i], des[i]);
        }
    }
}

/**
 * @tc.name      : wmemset_0400
 * @tc.desc      : Fill with null wide character
 * @tc.level     : Level 1
 */
void wmemset_0400(void)
{
    const int size = 5;
    wchar_t ch = L'\0';
    wchar_t des[size];
    
    wmemset(des, ch, size);
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is not null character\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wmemset_0500
 * @tc.desc      : Fill with Unicode wide character
 * @tc.level     : Level 1
 */
void wmemset_0500(void)
{
    const int size = 8;
    wchar_t ch = L'中';
    wchar_t des[size];
    
    wmemset(des, ch, size);
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is %lc, not %lc\n", __func__, i, des[i], ch);
        }
    }
}

/**
 * @tc.name      : wmemset_0600
 * @tc.desc      : Fill with control character
 * @tc.level     : Level 1
 */
void wmemset_0600(void)
{
    const int size = 5;
    wchar_t ch = L'\n';
    wchar_t des[size];
    
    wmemset(des, ch, size);
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is not newline character\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wmemset_0700
 * @tc.desc      : Verify return value is destination pointer
 * @tc.level     : Level 1
 */
void wmemset_0700(void)
{
    wchar_t des[10];
    wchar_t ch = L'R';
    wchar_t *result = wmemset(des, ch, 5);
    
    if (result != des) {
        t_error("%s Return value %p is not destination %p\n", 
                __func__, (void*)result, (void*)des);
    }
}

/**
 * @tc.name      : wmemset_0800
 * @tc.desc      : Overwrite existing data
 * @tc.level     : Level 2
 */
void wmemset_0800(void)
{
    wchar_t des[5] = {L'1', L'2', L'3', L'4', L'5'};
    wchar_t ch = L'O';
    
    wmemset(des, ch, 3);
    
    // First 3 should be 'O'
    for (int i = 0; i < 3; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is %lc, not %lc\n", __func__, i, des[i], ch);
        }
    }
    
    // Last 2 should remain unchanged
    if (des[3] != L'4') {
        t_error("%s des[3] overwritten: %lc\n", __func__, des[3]);
    }
    if (des[4] != L'5') {
        t_error("%s des[4] overwritten: %lc\n", __func__, des[4]);
    }
}

/**
 * @tc.name      : wmemset_0900
 * @tc.desc      : Large n value
 * @tc.level     : Level 2
 */
void wmemset_0900(void)
{
    const int size = 1000;
    wchar_t *des = (wchar_t *)malloc(size * sizeof(wchar_t));
    if (des == NULL) {
        t_error("%s Memory allocation failed\n", __func__);
        return;
    }
    
    wchar_t ch = L'*';
    wmemset(des, ch, size);
    
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is %lc, not %lc\n", __func__, i, des[i], ch);
            break;
        }
    }
    
    free(des);
}

/**
 * @tc.name      : wmemset_1000
 * @tc.desc      : Fill with maximum wchar_t value
 * @tc.level     : Level 2
 */
void wmemset_1000(void)
{
    const int size = 5;
    wchar_t ch = (wchar_t)-1;  // Maximum value
    wchar_t des[size];
    
    wmemset(des, ch, size);
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is 0x%x, not 0x%x\n", 
                    __func__, i, des[i], ch);
        }
    }
}

/**
 * @tc.name      : wmemset_1100
 * @tc.desc      : Single element fill
 * @tc.level     : Level 1
 */
void wmemset_1100(void)
{
    wchar_t des[3] = {L'A', L'B', L'C'};
    wchar_t ch = L'X';
    
    wmemset(des, ch, 1);
    
    if (des[0] != ch) {
        t_error("%s des[0] is %lc, not %lc\n", __func__, des[0], ch);
    }
    
    // Others should remain unchanged
    if (des[1] != L'B') {
        t_error("%s des[1] changed: %lc\n", __func__, des[1]);
    }
    if (des[2] != L'C') {
        t_error("%s des[2] changed: %lc\n", __func__, des[2]);
    }
}

/**
 * @tc.name      : wmemset_1200
 * @tc.desc      : Fill odd number of elements
 * @tc.level     : Level 2
 */
void wmemset_1200(void)
{
    const int size = 7;
    wchar_t des[size];
    wchar_t ch = L'7';
    
    wmemset(des, ch, size);
    for (int i = 0; i < size; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is %lc, not %lc\n", __func__, i, des[i], ch);
        }
    }
}

/**
 * @tc.name      : wmemset_1300
 * @tc.desc      : Verify no buffer overflow
 * @tc.level     : Level 2
 */
void wmemset_1300(void)
{
    wchar_t buffer[3] = {L'A', L'B', L'C'};
    wchar_t guard[3] = {L'X', L'Y', L'Z'};
    wchar_t ch = L'F';
    
    // This should only fill the buffer, not overflow into guard
    wmemset(buffer, ch, 3);
    
    // Check buffer
    for (int i = 0; i < 3; i++) {
        if (buffer[i] != ch) {
            t_error("%s buffer[%d] is %lc, not %lc\n", __func__, i, buffer[i], ch);
        }
    }
    
    // Check guard (should remain unchanged)
    for (int i = 0; i < 3; i++) {
        if (guard[i] != (wchar_t)(L'X' + i)) {
            t_error("%s Possible buffer overflow at guard[%d]\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wmemset_1400
 * @tc.desc      : Fill overlapping memory regions
 * @tc.level     : Level 2
 */
void wmemset_1400(void)
{
    wchar_t data[10] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9'};
    wchar_t ch = L'#';
    
    // Overwrite from middle
    wmemset(data + 3, ch, 4);
    
    // Check first 3 unchanged
    for (int i = 0; i < 3; i++) {
        if (data[i] != (wchar_t)(L'0' + i)) {
            t_error("%s data[%d] changed: %lc\n", __func__, i, data[i]);
        }
    }
    
    // Check middle 4 filled
    for (int i = 3; i < 7; i++) {
        if (data[i] != ch) {
            t_error("%s data[%d] is %lc, not %lc\n", __func__, i, data[i], ch);
        }
    }
    
    // Check last 3 unchanged
    for (int i = 7; i < 10; i++) {
        if (data[i] != (wchar_t)(L'0' + i)) {
            t_error("%s data[%d] changed: %lc\n", __func__, i, data[i]);
        }
    }
}

/**
 * @tc.name      : wmemset_1500
 * @tc.desc      : Compare with manual memset loop
 * @tc.level     : Level 1
 */
void wmemset_1500(void)
{
    const int size = 6;
    wchar_t des1[size];
    wchar_t des2[size];
    wchar_t ch = L'M';
    
    // Using wmemset
    wmemset(des1, ch, size);
    
    // Manual loop
    wchar_t *p = des2;
    size_t n = size;
    while (n--) *p++ = ch;
    
    // Compare results
    for (int i = 0; i < size; i++) {
        if (des1[i] != des2[i]) {
            t_error("%s Mismatch at [%d]: wmemset=%lc, manual=%lc\n", 
                    __func__, i, des1[i], des2[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    wmemset_0100();
    wmemset_0200();
    wmemset_0300();
    wmemset_0400();
    wmemset_0500();
    wmemset_0600();
    wmemset_0700();
    wmemset_0800();
    wmemset_0900();
    wmemset_1000();
    wmemset_1100();
    wmemset_1200();
    wmemset_1300();
    wmemset_1400();
    wmemset_1500();

    return t_status;
}
