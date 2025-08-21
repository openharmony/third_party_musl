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

struct A {
	virtual void Test();
};

void A::Test()
{
	fprintf(stderr, "A::Test()\n");
}

struct B : A {
	void Test() override;
};

void B::Test()
{
	fprintf(stderr, "B::Test()\n");
}

struct C : A {
};

struct D {
	virtual void Test();
};

void D::Test()
{
	fprintf(stderr, "D::Test()\n");
}

void CfiCastStrict()
{
	fprintf(stderr, "CfiCastStrict()\n");
	C *c = new C;
	A a;
	c = static_cast<C *>(&a);
}

void CfiDerivedCast()
{
	fprintf(stderr, "CfiDerivedCast()\n");
	B *b = new B;
	A a;
	b = static_cast<B *>(&a);
}

void CfiUnrelatedCast()
{
	fprintf(stderr, "CfiUnrelatedCast()\n");
	D *d = new D;
	A a;
	d = reinterpret_cast<D *>(&a);
}

int test_cast()
{
	// clang allow it by default. It can be disabled with -fsanitize=cfi-cast-strict.
	CfiCastStrict();

	// runtime error: control flow integrity check for type 'B' failed during base-to-derived cast
	CfiDerivedCast();

	// runtime error: control flow integrity check for type 'D' failed during cast to unrelated type
	CfiUnrelatedCast();

	return 0;
}
