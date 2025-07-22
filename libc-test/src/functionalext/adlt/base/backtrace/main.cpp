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
#include <stdlib.h>
#include <stdio.h>

#include "testbacktrace.h"
#include "functionalext.h"

static void backtrace_TestGetVer() {
  EXPECT_STREQ(__func__, "1.0", testbt::testVer("libadlt_base_backtrace_bt_lib.so").c_str());
  
  auto ret = testbt::testVerBT("libadlt_base_backtrace_bt_lib.so");
  EXPECT_TRUE(__func__, ret.find("clibGetVer") != std::string::npos);
}

static void backtrace_TestMax() {
  EXPECT_EQ(__func__, 42, testbt::testMax("libadlt_base_backtrace_bt_lib.so", 42, 1));
  
  auto ret = testbt::testMaxBT("libadlt_base_backtrace_bt_lib.so");
  EXPECT_TRUE(__func__, ret.find("clibMax") != std::string::npos);
}

int main(int argc, char **argv) {
  backtrace_TestGetVer();
  backtrace_TestMax();
  return t_status;
}
