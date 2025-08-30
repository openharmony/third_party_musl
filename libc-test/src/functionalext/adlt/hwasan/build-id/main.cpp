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
#include <sstream>

#include <sys/stat.h>
#include <pwd.h>

#include "adlt_common.h"
#include "common.h"

static std::string build_id_liba;
static std::string build_id_libb;

static void test_hwasan_build_id_read() {
	std::string filename = "/data/local/tmp/libc-test-lib/unittest_adlt_hwasan_build_id_output.txt";
	std::ifstream in(filename);
	if (!in) {
		t_error("%s: Unable to open file: %s\n", __func__, filename.c_str());
		return;
	}

	auto get_build_id = [&in] (const std::string &libName, std::string &build_id) {
		std::string line;
		if (!std::getline(in, line)) {
			t_error("%s: Error reading build id for %s or end of file reached\n", __func__, libName.c_str());
			return false;
		}

		size_t index = line.find("Build ID: ");
		if (index == std::string::npos) {
			t_error("%s: Failed to find build id for %s\n", __func__, libName.c_str());
			return false;
		}

		index += strlen("Build ID: ");
		build_id = line.substr(index, 16);
		if (build_id.empty()) {
			t_error("%s: Get build id for %s failed\n", __func__, libName.c_str());
			return false;
		}

		return true;
	};

	get_build_id("libadlt_hwasan_build_id_a.so", build_id_liba);
	get_build_id("libadlt_hwasan_build_id_b.so", build_id_libb);
}

static void test_hwasan_build_id_lib_a() {
	int pid = 0;
	run_subprocess([](){
		f_a();
	}, pid, "/data/local/tmp/libc-test/hwasan_report.log");

	std::string filename = "/data/local/tmp/libc-test/hwasan_report.log." + std::to_string(pid);
	std::ifstream reportFile(filename);
	if (!reportFile) {
		t_error("%s: Unable to open file: %s\n", __func__, filename.c_str());
		return;
	}

	std::stringstream strStream;
	strStream << reportFile.rdbuf();
	std::string reportData = strStream.str();

	if (reportData.empty()) {
		t_error("%s: Report file: %s is empty!\n", __func__, filename.c_str());
		return;
	}

	if (reportData.find(build_id_liba) == std::string::npos) {
		t_error("%s: Failed to match build id for liba in the report file: %s\n", __func__, build_id_liba.c_str());
	}
}

// - orig test will be PASSED
// - adlt test will be FAILED for the B library for now, because HWAsan in the
// message only specifies the first build ID that matches the LibA build ID
static void test_hwasan_build_id_lib_b() {
	int pid = 0;
	run_subprocess([](){
		f_b();
	}, pid, "/data/local/tmp/libc-test/hwasan_report.log");

	std::string filename = "/data/local/tmp/libc-test/hwasan_report.log." + std::to_string(pid);
	std::ifstream reportFile(filename);
	if (!reportFile) {
		t_error("%s: Unable to open file: %s\n", __func__, filename.c_str());
		return;
	}

	std::stringstream strStream;
	strStream << reportFile.rdbuf();
	std::string reportData = strStream.str();

	if (reportData.empty()) {
		t_error("%s: Report file: %s is empty!\n", __func__, filename.c_str());
		return;
	}

	if (reportData.find(build_id_libb) != std::string::npos) {
		t_error("%s: Finding build id for libb in the report file: %s\n", __func__, build_id_libb.c_str());
	}
}

int main(int argc, char **argv) {
	test_hwasan_build_id_read();
	test_hwasan_build_id_lib_a();
	test_hwasan_build_id_lib_b();
	return t_status;
}