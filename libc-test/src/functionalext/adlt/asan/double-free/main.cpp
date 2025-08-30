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

static void double_free_test() {
	int pid = 0;
	run_subprocess([](){
		testDoubleFreeA();
		testFreeB();
	}, pid, "/data/local/tmp/libc-test/asan_report.log");

	std::string filename = "/data/local/tmp/libc-test/asan_report.log." + std::to_string(pid);

	EXPECT_TRUE(__func__, !filename.empty());

	std::ifstream reportFile(filename);
	EXPECT_TRUE(__func__, reportFile);

	// Read all data from the file.
	std::stringstream strStream;
	strStream << reportFile.rdbuf();

	std::string libA = "libadlt_asan_double_free_a.so";
	std::string libB = "libadlt_asan_double_free_b.so";
	std::string errorMsg = "ERROR: AddressSanitizer: attempting double-free";

	bool libAErrorFound = false;
	bool libBErrorFound = false;

	// Let's look through the whole log and find the lines we need.
	bool skipLine = false;
	std::string line;
	while (std::getline(strStream, line)) {
		if (line.find("memory map") != std::string::npos) {
			skipLine = !skipLine;
		}

		if (skipLine) {
			continue;
		}

		// If we found an error related to 'b' library - mark it.
		// We expect that no error will be found.
		if (line.find(libB) != std::string::npos) {
			libBErrorFound = true;
		}

		// If we found the expected error message, we need to check the second
		// line after that error, which will reference the library.
		if (line.find(errorMsg) == std::string::npos) {
			continue;
		}

		std::string libLine;
		if (std::getline(strStream, line) && std::getline(strStream, libLine) &&
			libLine.find(libA) != std::string::npos) {
				libAErrorFound = true;
			}
	}

	EXPECT_TRUE(__func__, libAErrorFound);
	EXPECT_TRUE(__func__, !libBErrorFound);
}

int main(int argc, char **argv) {
	double_free_test();
	return t_status;
}
