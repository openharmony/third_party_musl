/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <malloc.h>
#include <cstdio>
#include <cstdlib>

#include "testmalloc.h"

namespace testmalloc {
int allocateInt(void) {
  printf("Trying to allocate %d\n", static_cast<int>(sizeof(int)));

  int *mem = static_cast<int *>(malloc(sizeof(int)));
  if (mem == nullptr) {
    printf("Could not allocate memory\n");
    return 1;
  } else {
    printf("Memory allocated\n");
    free(mem);
    printf("Memory freed\n");
    return 0;
  }
}

int allocateChar(void) {
  printf("Trying to allocate %d\n", static_cast<int>(sizeof(char)));

  char *mem = static_cast<char *>(malloc(sizeof(char)));
  if (mem == nullptr) {
    printf("Could not allocate memory\n");
    return 1;
  } else {
    printf("Memory allocated\n");
    free(mem);
    printf("Memory freed\n");
    return 0;
  }
}

int allocateDouble(void) {
  printf("Trying to allocate %d\n", static_cast<int>(sizeof(double)));

  double *mem = static_cast<double *>(malloc(sizeof(double)));
  if (mem == nullptr) {
    printf("Could not allocate memory\n");
    return 1;
  } else {
    printf("Memory allocated\n");
    free(mem);
    printf("Memory freed\n");
    return 0;
  }
}

} // namespace testmalloc
