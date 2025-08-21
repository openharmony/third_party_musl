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
#include <cstring>
#include <securec.h>

#include "common.h"

struct CallTestA {
	virtual void VcallFunc();
	void CallFunc();
};

void CallTestA::VcallFunc()
{
	fprintf(stderr, "CallTestA::VcallFunc()\n");
}

void CallTestA::CallFunc()
{
	fprintf(stderr, "CallTestA::CallFunc()\n");
}

struct CallTestB {
	virtual void VcallFunc();
	void CallFunc();
};

void CallTestB::VcallFunc()
{
	fprintf(stderr, "CallTestB::VcallFunc()\n");
}

void CallTestB::CallFunc()
{
	fprintf(stderr, "CallTestB::CallFunc()\n");
}

void Icall()
{
	fprintf(stderr, "Icall()\n");
}

void CfiIcall()
{
	(reinterpret_cast<void(*)(int)>(Icall))(42);
}

void CfiVcall()
{
	CallTestA *a;
	void *p = static_cast<void *>(new CallTestB());
	memcpy_s(&a, sizeof(a), &p, sizeof(p));
	a->VcallFunc();
}

void CfiNvcall()
{
	CallTestA *a;
	void *p = static_cast<void *>(new CallTestB());
	memcpy_s(&a, sizeof(a), &p, sizeof(p));
	a->CallFunc();
}

int test_call()
{
	// runtime error: control flow integrity check for type 'CallTestA' failed during non-virtual call
	CfiNvcall();

	// runtime error: control flow integrity check for type 'CallTestA' failed during virtual call
	CfiVcall();

	// runtime error: control flow integrity check for type 'void (int)' failed during indirect function call
	CfiIcall();

	return 0;
}
