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
#ifndef _ADLT_TEST_CDTORS_A_H_
#define _ADLT_TEST_CDTORS_A_H_

namespace liba {

enum Flags {
    INIT_0 = 0,
    INIT_1 = 1,
    INIT_2 = 2,
    INIT_3 = 3,
    FINI_0 = 4,
    FINI_1 = 5,
    FINI_2 = 6,
    FINI_3 = 7,
};

class ClassA {
public:
    explicit ClassA(unsigned value);
    ~ClassA();

    static ClassA& Instance();
    void Log(const char* message="") const;

    unsigned Value() const;
    void SetFlag(unsigned bit);
    bool HasFlag(unsigned bit) const;

private:
    int object_id;
    unsigned value = 0;
};

extern "C" const ClassA
        class_a_instance_0,
        class_a_instance_1;

} // namespace liba

#endif