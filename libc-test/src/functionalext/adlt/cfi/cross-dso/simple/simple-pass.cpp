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
#if !defined(SHARED_LIB1) && !defined(SHARED_LIB2)
#include "functionalext.h"
#endif

struct PA {
	virtual void a();
};

PA *create_PB();
PA *create_PC();

int icall1();
int icall2();

#ifdef SHARED_LIB1

#include "simple_utils.h"
struct PB : public PA {
	virtual void a();
};
void PB::a() {}

PA *create_PB() {
	make_derive<PB>();
	return new PB();
}

int icall1() {
	return 1;
}

#elif defined SHARED_LIB2

#include "simple_utils.h"
struct PC : public PA {
	virtual void a();
};
void PC::a() {}

PA *create_PC() {
	make_derive<PC>();
	return new PC();
}

int icall2() {
	return 0;
}
#else

void PA::a() {}

static void CFICrossDsoSimplePassTests_TestVPtr() {
	fprintf(stderr, "cross-dso simple pass example vptr\n");

	PA *b = create_PB();
	PA *c = create_PC();

	fprintf(stderr, "=1=\n");
	b->a(); // OK
	fprintf(stderr, "=2=\n");
	c->a(); // OK
	fprintf(stderr, "=3=\n");
}

static void CFICrossDsoSimplePassTests_TestICall() {
	fprintf(stderr, "cross-dso simple pass example icall\n");

	using func_t = int (*)();

	func_t arr[] = {icall1, icall2};
	int idx = 0;

	fprintf(stderr, "=1=\n");
	idx = arr[idx](); // OK, call icall1
	fprintf(stderr, "=2=\n");
	idx = arr[idx](); // OK, call icall2
	fprintf(stderr, "=3=\n");
	idx = arr[idx](); // OK, call icall1
	fprintf(stderr, "=4=\n");
}

int main(int argc, char *argv[]) {
	CFICrossDsoSimplePassTests_TestVPtr();
	CFICrossDsoSimplePassTests_TestICall();
	return t_status;
}
#endif
