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
#include <fstream>
#include <string>
#include <sstream>

#include "adlt_common.h"
#include "common.h"

static void heap_use_after_free() {
  int pid = 0;
  run_subprocess([](){
    int result = testHeapUseAfterFree();
    (void)result;
  }, pid, "/data/local/tmp/libc-test/asan_report.log");

  std::string filename = "/data/local/tmp/libc-test/asan_report.log." + std::to_string(pid);

  EXPECT_FALSE(__func__, filename.empty());

  std::ifstream reportFile(filename);
  EXPECT_TRUE(__func__, reportFile);

  // Read all data from the file.
  std::stringstream strStream;
  strStream << reportFile.rdbuf();
  std::string reportData = strStream.str();

  EXPECT_FALSE(__func__, reportData.empty());

  std::string firstCase = "ERROR: AddressSanitizer: heap-use-after-free";
  EXPECT_FALSE(__func__, reportData.find(firstCase) == std::string::npos);
}

int main(int argc, char **argv) {
  heap_use_after_free();
  return t_status;
}
