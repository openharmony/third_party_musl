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
#include "common.h"

static void DuplicatesTests_TestGetVal1() {
  EXPECT_EQ(__func__, 1, get_val_from_A());
  EXPECT_EQ(__func__, 2, get_val_from_B());
  EXPECT_EQ(__func__, 3, get_val_from_C());
}

static void DuplicatesTests_TestGetVal2() {
  EXPECT_EQ(__func__, 1, get_valA_from_A());
  EXPECT_EQ(__func__, 2, get_valB_from_A());
  EXPECT_EQ(__func__, 3, get_valC_from_A());

  EXPECT_EQ(__func__, 1, get_valA_from_B());
  EXPECT_EQ(__func__, 2, get_valB_from_B());
  EXPECT_EQ(__func__, 3, get_valC_from_B());

  EXPECT_EQ(__func__, 1, get_valA_from_C());
  EXPECT_EQ(__func__, 2, get_valB_from_C());
  EXPECT_EQ(__func__, 3, get_valC_from_C());
}

// This test used symbols from first lib (A)
static void DuplicatesTests_TestGetVal3() {
  EXPECT_EQ(__func__, 1, val);
  EXPECT_EQ(__func__, 1, get_val());
}

int main(int argc, char **argv) {
  DuplicatesTests_TestGetVal1();
  DuplicatesTests_TestGetVal2();
  DuplicatesTests_TestGetVal3();
  return t_status;
}
