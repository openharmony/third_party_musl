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
#include <dlfcn.h>
#include "functionalext.h"

struct AA {
  virtual void Test();
};

void AA::Test() { printf("AA::Test()\n"); }

static void ChangeToAnotherObj() {
  AA *a = new AA;
  void *handle = dlopen("libadlt_cfi_cross_dso_ldso_test.so", RTLD_NOW);
  if (handle == nullptr) {
    return;
  }
  void *(*create_B)() = (void *(*)())dlsym(handle, "CreateObj");
  void *p = create_B();
  a->Test();
  memcpy(&a, &p, sizeof(void *));
  a->Test();
}

static void CFICrossDsoLdsoTests_TestLdso() {
  fprintf(stderr, "cross-dso cfi test\n");
  ChangeToAnotherObj();
}

int main(int argc, char **argv) {
  CFICrossDsoLdsoTests_TestLdso();
  return t_status;
}
