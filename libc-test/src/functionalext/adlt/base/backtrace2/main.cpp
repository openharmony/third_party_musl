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

#include "bt-test.h"
#include "functionalext.h"

static void backtrace2_TestGetVer() {
  std::string verA("");
  std::string verB("");

  testbt::TestVer test_1(verA, verB);
  test_1.symbolNameA = "clibGetVerA";
  test_1.fileNameA = "libadlt_base_backtrace2_lib_a.so";
  test_1.symbolNameB = "clibGetVerB";
  test_1.fileNameB = "libadlt_base_backtrace2_lib_b.so";

  std::string resVer = test_1.test();

  EXPECT_STREQ(__func__, "1.0.a", verA.c_str());
  EXPECT_STREQ(__func__, "1.0.b", verB.c_str());

  std::string btA("");
  std::string btB("");
  testbt::TestVerBT test_2(btA, btB);
  test_2.symbolNameA = "clibGetVerA";
  test_2.fileNameA = "libadlt_base_backtrace2_lib_a.so";
  test_2.symbolNameB = "clibGetVerB";
  test_2.fileNameB = "libadlt_base_backtrace2_lib_b.so";

  std::string resBT = test_2.test();

  bool okA = (btA.find("clibGetVerA") != std::string::npos) &&
    (btA.find("backtrace2_lib_a.so") != std::string::npos); 
  bool okB = (btB.find("clibGetVerB") != std::string::npos) &&
    (btB.find("backtrace2_lib_b.so") != std::string::npos); 
  
  EXPECT_TRUE(__func__, okA && okB);
}

static void backtrace2_TestMaxMin() {
  int max = 0;
  int min = 0;

  testbt::TestMaxMin test_1(max, min);
  test_1.symbolNameA = "clibMaxA";
  test_1.fileNameA = "libadlt_base_backtrace2_lib_a.so";
  test_1.aa = 1;
  test_1.ab = 55;
  test_1.symbolNameB = "clibMinB";
  test_1.fileNameB = "libadlt_base_backtrace2_lib_b.so";
  test_1.ba = 2;
  test_1.bb = 11;

  std::string resMaxMin = test_1.test();

  EXPECT_EQ(__func__, 55, max);
  EXPECT_EQ(__func__, 2, min);
  
  std::string btA("");
  std::string btB("");
  testbt::TestVerBT test_2(btA, btB);
  test_2.symbolNameA = "clibMaxA";
  test_2.fileNameA = "libadlt_base_backtrace2_lib_a.so";
  test_2.symbolNameB = "clibMinB";
  test_2.fileNameB = "libadlt_base_backtrace2_lib_b.so";

  std::string resBT = test_2.test();

  bool okA = (btA.find("clibMaxA") != std::string::npos) &&
    (btA.find("backtrace2_lib_a.so") != std::string::npos); 
  bool okB = (btB.find("clibMinB") != std::string::npos) &&
    (btB.find("backtrace2_lib_b.so") != std::string::npos); 
  
  EXPECT_TRUE(__func__, okA && okB);
}

int main(int argc, char **argv) {
  backtrace2_TestGetVer();
  backtrace2_TestMaxMin();
  return t_status;
}