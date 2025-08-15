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
#include "b.h"
#include <cstdio>

namespace libb {

static int next_object_id = 0;

ClassB::ClassB(unsigned value):
    object_id(next_object_id++),
    value(value) {
        Log("ctor");
}

ClassB::~ClassB() {
    Log("dtor");
}

ClassB& ClassB::Instance() {
    static ClassB instance(0);
    return instance;
}

void ClassB::Log(const char* message) const {
    printf("[ClassB::Log] \t[%s] \toid:%d \tvalue:%d\n", message, object_id, value);

}

unsigned ClassB::Value() const {
    return value;
}

void ClassB::SetFlag(unsigned bit) {
    Log("pre-SetFlag");
    value |= 1<<bit;
    Log("post-SetFlag");
}

bool ClassB::HasFlag(unsigned bit) const {
    return !!(value & (1<<bit));
}

void __attribute__((constructor)) init_b_0() {
    printf("called libb::init_b_0\n");
    ClassB::Instance().SetFlag(INIT_0);
}

void __attribute__((constructor)) init_b_1() {
    printf("called libb::init_b_1\n");
    ClassB::Instance().SetFlag(INIT_1);
}

void __attribute__((constructor)) init_b_2() {
    printf("called libb::init_b_2\n");
    ClassB::Instance().SetFlag(INIT_2);
}

void __attribute__((destructor)) fini_b_0() {
    printf("called libb::fini_b_0\n");
    ClassB::Instance().SetFlag(FINI_0);
}

void __attribute__((destructor)) fini_b_1() {
    printf("called libb::fini_b_1\n");
    ClassB::Instance().SetFlag(FINI_1);
}

void __attribute__((destructor)) fini_b_2() {
    printf("called libb::fini_b_2\n");
    ClassB::Instance().SetFlag(FINI_2);
}

void __attribute__((destructor)) fini_b_3() {
    printf("called libb::fini_b_3\n");
    ClassB::Instance().SetFlag(FINI_3);
}

const ClassB
        class_b_instance_0(42+0),
        class_b_instance_1(42+1),
        class_b_instance_2(42+2);

} // namespace libb