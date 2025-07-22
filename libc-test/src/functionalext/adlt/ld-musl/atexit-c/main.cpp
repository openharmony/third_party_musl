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
#include <cstdio>
#include <cstdlib>

#include "adlt_common.h"
#include "liba.h"
#include "libb.h"

static void atexit_main1() { dprintf(2, "main1."); }

static void atexit_main2() { dprintf(2, "main2."); }

static void atexit_main() {
  atexit(atexit_main1);
  atexit(atexit_a1);
  atexit(atexit_b1);
  atexit(atexit_a2);
  atexit(atexit_b2);
  atexit(atexit_main2);
  exit(0);
}

static void atexitMuslCTests_TestExitMultilib() {
  EXPECT_EXIT(__func__, atexit_main(), 0,
              "main2.b2.a2.b1.a1.main1.");
}

int main(int argc, char **argv) {
  atexitMuslCTests_TestExitMultilib();
  return t_status;
}
