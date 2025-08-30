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

static void tsan_DataRaceTwoLibs() {
	int pid = 0;
	run_subprocess([](){
		testA();
		testB();
	}, pid, "/data/local/tmp/libc-test/tsan_report.log");

	std::string filename = "/data/local/tmp/libc-test/tsan_report.log." + std::to_string(pid);
	std::ifstream reportFile(filename);
	if (!reportFile) {
		t_error("%s: Unable to open file: %s\n", __func__, filename.c_str());
		return;
	}

	// Read all data from the file.
	std::stringstream strStream;
	strStream << reportFile.rdbuf();
	std::string reportData = strStream.str();

	std::string libA = "libadlt_tsan_data_race_two_libs_a.so";
	std::string libB = "libadlt_tsan_data_race_two_libs_b.so";
	std::string errorMsg = "SUMMARY: ThreadSanitizer: data race";

	if(reportData.empty()) {
		t_error("%s: Report file: %s is empty!\n", __func__, filename.c_str());
		return;
	}

	if (reportData.find(libA) == std::string::npos) {
		t_error("%s: Failed to match a string in the report file: %s\n", __func__, libA.c_str());
	}

	if (reportData.find(errorMsg) == std::string::npos) {
		t_error("%s: Failed to match a string in the report file: %s\n", __func__, libB.c_str());
	}

	// We expect that there will be no mention of the 'libtsanDataRaceTwoLibs_b'
	// library in the report.
	if (reportData.find(libB) != std::string::npos) {
		t_error("%s: Found a string we didn't expect to find: %s\n", __func__, libB.c_str());
	}
}

int main(int argc, char **argv) {
	tsan_DataRaceTwoLibs();
	return t_status;
}
