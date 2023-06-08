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

#include "util.h"

#include <err.h>
#include <math.h>
#include <sched.h>
#include <stdio.h>
#include <wchar.h>
#include <cstdlib>

char* AlignUpMemoy(char* origPtr, size_t alignment)
{
    if ((alignment & (alignment - 1)) != 0) {
        perror("alignment is not a power of two.");
    }

    uintptr_t ptr = reinterpret_cast<uintptr_t>(origPtr);
    if (alignment > 0) {
        ptr = ((ptr + (alignment - 1)) & (~(alignment - 1)));
    }

    return reinterpret_cast<char*>(ptr);
}

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes)
{
    buf->resize(nbytes + 2 * alignment);
    return AlignUpMemoy(buf->data(), alignment);
}

wchar_t* GetAlignedPtr(std::vector<wchar_t>* buf, size_t alignment, size_t nchars)
{
    buf->resize(nchars + ceil((2 * alignment) / sizeof(wchar_t)));
    return reinterpret_cast<wchar_t*>(AlignUpMemoy(reinterpret_cast<char*>(buf->data()), alignment));
}

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fillByte)
{
    char* bufAligned = GetAlignedPtr(buf, alignment, nbytes);
    memset(bufAligned, fillByte, nbytes);
    return bufAligned;
}

wchar_t* GetAlignedPtrFilled(std::vector<wchar_t>* buf, size_t alignment, size_t nbytes, wchar_t fillByte)
{
  wchar_t* bufAligned = GetAlignedPtr(buf, alignment, nbytes);
  wmemset(bufAligned, fillByte, nbytes);
  return bufAligned;
}