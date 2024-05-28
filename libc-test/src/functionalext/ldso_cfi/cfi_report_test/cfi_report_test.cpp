/**
* Copyright (c) 2024 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <cstdlib>
#include "cfi_util.h"

int main() {
	const char* log_tag = "cfi_icall_report_test";
	const char* test_case = "/data/local/tmp/libc-test/cfi_icall_report_test";
	ClearCfiLog(log_tag, FAULTLOG_DIR);
	system(test_case);
	FindAndCheck("CFI check failed. Function Address:", log_tag, FAULTLOG_DIR);
}
