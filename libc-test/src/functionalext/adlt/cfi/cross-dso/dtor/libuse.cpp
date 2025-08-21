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
#include <cstdio>
#include "common.h"

struct base_t {
	virtual void f();
};

struct derived_t : base_t {
	void f() override;
};

void derived_t::f() {
	puts("derived_t::f");
}

static struct dummy {
	base_t *ptr = nullptr;

	~dummy() {
	puts("~dummy");
	if (ptr)
		do_f(ptr);
	}
} __dummy;

extern "C" void create() {
	puts("create");
	__dummy.ptr = new derived_t();
}
