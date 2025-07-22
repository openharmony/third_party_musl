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
#include <string>

#include "cpp-lib.h"

namespace cpplib {
std::string CppDynamicLib::makeVersion() const {
  return std::to_string(versionMajor) + "." + std::to_string(versionMinor);
}

int CppDynamicLib::getMax(int a, int b) const {
  return a > b ? a : b;
}

std::string CppDynamicLib::getVersion(void) const {
  return makeVersion();
};

int CppDynamicLib::getStorage(void) const {
  return storageVar;
}

extern "C" ICppDynamicLib* create(int value) {
    return new CppDynamicLib(value);
}

extern "C" void destroy(ICppDynamicLib* p) {
    delete p;
}

} //namespace cpplib