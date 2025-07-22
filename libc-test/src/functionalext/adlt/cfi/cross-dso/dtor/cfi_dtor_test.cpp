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
#include "common.h"

static void CFICrossDsoDtorTests_MainTest() {
  using create_t = void (*)();
  auto *lib = dlopen("libadlt_cfi_cross_dso_dtor_use.so", RTLD_NOW);
  auto *fn = (create_t)dlsym(lib, "create");
  fn();
  // check that calls to `dlclose`ing libuse from libbase still work
  dlclose(lib);
  // check that calls to libbase NDSO still work after unloading libuse NDSO
  using do_f_t = void(*)(base_t *);
  do_f_t ptr = do_f;
  (*ptr)(nullptr);
}

int main(int argc, char **argv) {
  CFICrossDsoDtorTests_MainTest();
  return t_status;
}
