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

static void VerneedsTest_testCase1() {
  // get sym from adlt from outer lib
  EXPECT_EQ(__func__, 1, get_val_fromA());
  EXPECT_EQ(__func__, 2, get_val_fromB());
}

static void VerneedsTest_testCase2() {
  // different names, same versions
  EXPECT_EQ(__func__, 3, get_duplicateA_fromA());
  EXPECT_EQ(__func__, 4, get_duplicateB_fromB());
  // same names, different versions
  EXPECT_EQ(__func__, 5, get_duplicate1_fromA());
  EXPECT_EQ(__func__, 6, get_duplicate1_fromB());
  // same names, same versions
  EXPECT_EQ(__func__, 7, get_duplicate2_fromA());
  EXPECT_EQ(__func__, 7, get_duplicate2_fromB());
}

int main(int argc, char **argv) {
  VerneedsTest_testCase1();
  VerneedsTest_testCase2();
  return t_status;
}
