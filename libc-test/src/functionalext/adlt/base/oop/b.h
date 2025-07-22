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
#ifndef _ADLT_TEST_OOP_B_H_
#define _ADLT_TEST_OOP_B_H_

#include <cstdio>

namespace libb {
#define LIB_SUFFIX B
#define STRINGIZE1(value) #value
#define STRINGIZE(value) STRINGIZE1(value)
#define LIB_STRING STRINGIZE(LIB_SUFFIX)

/***
 * Constructor/destructor test classes
 ***/

class Class {
public:
  explicit Class(int);
  ~Class();
  int Payload(const char *);

private:
  int intVar = 0;
};

/***
 * Vtable test classes
 ***/

class GlobalPoly {
public:
  explicit GlobalPoly(int);
  virtual int VirtualMethod();
  static GlobalPoly *New(int value);
  static GlobalPoly *NewDerived(int value);

  virtual ~GlobalPoly();

protected:
  int intVar = 0;
};

class GlobalPolyDerived : public GlobalPoly {
public:
  explicit GlobalPolyDerived(int);
  int VirtualMethod() override;
};

class InlinePoly {
public:
  explicit InlinePoly(int value) : intVar(value) {}
  virtual int VirtualMethod() {
    puts("InlinePoly" LIB_STRING "::VirtualMethod");
    return intVar;
  }
  static InlinePoly *New(int value);
  static InlinePoly *NewDerived(int value);

  virtual ~InlinePoly() = default;

protected:
  int intVar = 0;
};

class InlinePolyDerived : public InlinePoly {
public:
  explicit InlinePolyDerived(int value) : InlinePoly(value) {}
  int VirtualMethod() override {
    puts("InlinePolyDerived" LIB_STRING "::VirtualMethod");
    return intVar + 20;
  }
};

class LocalPoly {
public:
  explicit LocalPoly(int value);
  virtual int VirtualMethod();
  static LocalPoly *New(int value);
  static LocalPoly *NewDerived(int value);

  virtual ~LocalPoly();

protected:
  int intVar = 0;
};

/***
 * Virtual inheritance test classes
 ***/

class DiamondRoot {
public:
  explicit DiamondRoot(int data);
  virtual ~DiamondRoot() = default;

  virtual int VirtualMethod();
  virtual int VirtualMethod1();
  virtual int VirtualMethod2();
  virtual int VirtualMethod3();

protected:
  int rootData;
};

class DiamondLeft : public virtual DiamondRoot {
public:
  explicit DiamondLeft(int rootData, int data);
  ~DiamondLeft() override = default;

  int VirtualMethod1() override;

protected:
  int leftData;
};

class DiamondRight : public virtual DiamondRoot {
public:
  explicit DiamondRight(int rootData, int data);
  ~DiamondRight() override = default;

  int VirtualMethod2() override;

protected:
  int rightData;
};

class DiamondChild : public DiamondLeft, public DiamondRight {
public:
  explicit DiamondChild(int rootData, int leftData, int rightData, int data);

  int VirtualMethod3() override;

protected:
  int childData;
};

#undef STRINGIZE1
#undef STRINGIZE
#undef LIB_STRING
#undef LIB_SUFFIX
} // namespace libb

#endif