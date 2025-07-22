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
#include "functionalext.h"

#include "lib1.h"
#include "lib3.h"

static void EhFrameTests_test13() {
  int f1_test = 0;
  int f3_test = 0;
  try {
    f1();
  } catch (...) {
    f1_test = 1;
  }

  try {
    f3();
  } catch (...) {
    f3_test = 1;
  }
  EXPECT_EQ(__func__, f1_test, 1);
  EXPECT_EQ(__func__, f3_test, 1);
}

int main(int argc, char **argv) {
  EhFrameTests_test13();
  return t_status;
}
