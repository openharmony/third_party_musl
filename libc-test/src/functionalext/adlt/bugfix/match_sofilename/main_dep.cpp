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
#include <iostream>
#include "functionalext.h"
#include "header.h"

static void test_bugfix_match_sofilename_dep() {
	int b = 10;
	int resultAdd = add(b);
	int resultSub = sub(100, 20);
	EXPECT_EQ(__func__, resultAdd, 110);
	EXPECT_EQ(__func__, resultSub, 80);
}

int main(int argc, char **argv) {
	test_bugfix_match_sofilename_dep();
	return t_status;
}