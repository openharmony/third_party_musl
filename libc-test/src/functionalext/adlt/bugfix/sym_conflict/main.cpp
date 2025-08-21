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
#include "header.h"

static void test_bugfix_sym_conflict_add() {
	int sum = 0;
	sum = add(3, 4);
	EXPECT_EQ(__func__, 7, sum);
}

static void test_bugfix_sym_conflict_sub() {
	int dif = 0;
	dif = sub(10, 9);
	EXPECT_EQ(__func__, 1, dif);
}

int main(int argc, char **argv) {
	test_bugfix_sym_conflict_add();
	test_bugfix_sym_conflict_sub();
	return t_status;
}