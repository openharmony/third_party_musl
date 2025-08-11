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
#include "a.h"
#include <cstdio>

namespace liba {

static int next_object_id = 0;

ClassA::ClassA(unsigned value):
    object_id(next_object_id++),
    value(value) {
        Log("ctor");
}

ClassA::~ClassA() {
    Log("dtor");
}

ClassA& ClassA::Instance() {
    static ClassA instance(0);
    return instance;
}

void ClassA::Log(const char* message) const {
    printf("[ClassA::Log] \t[%s] \toid:%d \tvalue:%d\n", message, object_id, value);

}

unsigned ClassA::Value() const {
    return value;
}

void ClassA::SetFlag(unsigned bit) {
    Log("pre-SetFlag");
    value |= 1<<bit;
    Log("post-SetFlag");
}

bool ClassA::HasFlag(unsigned bit) const {
    return !!(value & (1<<bit));
}

void __attribute__((constructor)) init_a_0() {
    printf("called liba::init_a_0\n");
    ClassA::Instance().SetFlag(INIT_0);
}

void __attribute__((constructor)) init_a_1() {
    printf("called liba::init_a_1\n");
    ClassA::Instance().SetFlag(INIT_1);
}

void __attribute__((destructor)) fini_a_0() {
    printf("called liba::fini_a_0\n");
    ClassA::Instance().SetFlag(FINI_0);
}

void __attribute__((destructor)) fini_a_1() {
    printf("called liba::fini_a_1\n");
    ClassA::Instance().SetFlag(FINI_1);
}

void __attribute__((destructor)) fini_a_2() {
    printf("called liba::fini_a_2\n");
    ClassA::Instance().SetFlag(FINI_2);
}

const ClassA
        class_a_instance_0(42+0),
        class_a_instance_1(42+1)
        ;

} // namespace liba