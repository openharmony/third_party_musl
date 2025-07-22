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

using namespace liba;

#define LIB_SUFFIX A
#define STRINGIZE1(value) #value
#define STRINGIZE(value) STRINGIZE1(value)
#define LIB_STRING STRINGIZE(LIB_SUFFIX)

/***
 * Constructor/destructor test classes
 ***/

Class::Class(int value) : intVar(value + 2) {
  printf("This is Class" LIB_STRING " C-tor\n");
}

Class::~Class() { printf("This is Class" LIB_STRING " D-tor\n"); }

int Class::Payload(const char *text) {
  printf("%s", text);
  return intVar;
}

/***
 * Vtable test classes
 ***/

GlobalPoly::GlobalPoly(int value) : intVar(value) {}
int GlobalPoly::VirtualMethod() {
  puts("GlobalPoly" LIB_STRING "::VirtualMethod");
  return intVar;
}

GlobalPoly *GlobalPoly::New(int value) { return new GlobalPoly(value); }
GlobalPoly *GlobalPoly::NewDerived(int value) {
  return new GlobalPolyDerived(value);
}
GlobalPoly::~GlobalPoly() = default;

GlobalPolyDerived::GlobalPolyDerived(int value) : GlobalPoly(value) {}
int GlobalPolyDerived::VirtualMethod() {
  puts("GlobalPolyDerived" LIB_STRING "::VirtualMethod");
  return intVar + 10;
}

InlinePoly *InlinePoly::New(int value) { return new InlinePoly(value); }
InlinePoly *InlinePoly::NewDerived(int value) {
  return new InlinePolyDerived(value);
}

namespace {
class LocalPolyDerived : public LocalPoly {
public:
  explicit LocalPolyDerived(int value) : LocalPoly(value) {}
  int VirtualMethod() override {
    puts("LocalPolyDerived" LIB_STRING "::VirtualMethod");
    return intVar + 30;
  }
};
} // anonymous namespace

LocalPoly::LocalPoly(int value) : intVar(value) {}
int LocalPoly::VirtualMethod() {
  puts("LocalPoly" LIB_STRING "::VirtualMethod");
  return intVar;
}

LocalPoly *LocalPoly::New(int value) { return new LocalPoly(value); }
LocalPoly *LocalPoly::NewDerived(int value) {
  return new LocalPolyDerived(value);
}
LocalPoly::~LocalPoly() = default;

/***
 * Virtual inheritance test classes
 ***/

DiamondRoot::DiamondRoot(int data) : rootData(data) {}

int DiamondRoot::VirtualMethod() { return rootData; }
int DiamondRoot::VirtualMethod1() { return rootData + 1; }
int DiamondRoot::VirtualMethod2() { return rootData - 1; }
int DiamondRoot::VirtualMethod3() { return rootData * 2; }

DiamondLeft::DiamondLeft(int rootData, int data)
    : DiamondRoot(rootData), leftData(data) {}
int DiamondLeft::VirtualMethod1() { return leftData; }

DiamondRight::DiamondRight(int rootData, int data)
    : DiamondRoot(rootData), rightData(data) {}

int DiamondRight::VirtualMethod2() { return rightData; }

DiamondChild::DiamondChild(int rootData, int leftData, int rightData, int data)
    : DiamondRoot(rootData), DiamondLeft(rootData, leftData),
      DiamondRight(rootData, rightData), childData(data) {}

int DiamondChild::VirtualMethod3() { return childData; }

#undef STRINGIZE1
#undef STRINGIZE
#undef LIB_STRING
#undef LIB_SUFFIX