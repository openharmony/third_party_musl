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
#include "functionalext.h"
#include <cstdio>
#include <cstdlib>

#include "a.h"
#include "b.h"

struct Last {};

template <typename FirstT, typename... NextT>
struct Types {
    using First = FirstT;
    using Next = Types<NextT...>;
};

template <typename FirstT>
struct Types<FirstT> {
    using First = FirstT;
    using Next = Last;
};

static void CtorDtorTests_TestClassA() {
    int initA = 1;
    liba::Class *a = new liba::Class(initA);
    const char *payloadA = "Class A payload\n";
    int retA = a->Payload(payloadA);
    delete a;
    EXPECT_EQ(__func__, retA, initA + 2);
}

static void CtorDtorTests_TestClassB() {
    int initB = 2;
    libb::Class *b = new libb::Class(initB);
    const char *payloadB = "Class B payload\n";
    int retB = b->Payload(payloadB);
    delete b;
    EXPECT_EQ(__func__, retB, initB + 2);
}

template <typename T, int Add = 0> struct InitA {
    using Type = T;
    static constexpr int InitVal = 1;
    static constexpr int AddVal = Add;
};

template <typename T, int Add = 0> struct InitB {
    using Type = T;
    static constexpr int InitVal = 2;
    static constexpr int AddVal = Add;
};

using PolyTypes =
        Types<InitA<liba::GlobalPoly, 0>, InitB<libb::GlobalPoly, 0>,
                                         InitA<liba::InlinePoly, 10>, InitB<libb::InlinePoly, 10>,
                                         InitA<liba::LocalPoly, 20>, InitB<libb::LocalPoly, 20>>;

template <typename PolyT> class VtableTests {
public:
    void TestVirtualMethod() {
        int initVal = PolyT::InitVal;
        auto *ptr = PolyT::Type::New(initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod(), initVal);
        delete ptr;
    }

    void TestDerivedVirtualMethod() {
        int initVal = PolyT::InitVal;
        int addVal = PolyT::AddVal;
        auto *ptr = PolyT::Type::NewDerived(initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod(), initVal + 10 + addVal);
        delete ptr;
    }
};

static void TestVtable_GlobalPoly() {
    VtableTests<InitA<liba::GlobalPoly, 0>> tests;
    tests.TestVirtualMethod();
    tests.TestDerivedVirtualMethod();
}

static void TestVtable_LocalPoly() {
    VtableTests<InitA<liba::LocalPoly, 20>> tests;
    tests.TestVirtualMethod();
    tests.TestDerivedVirtualMethod();
}

using DiamondTypes =
        Types<InitA<liba::DiamondChild>, InitB<libb::DiamondChild>>;

template <typename DiamondT>
class VirtualInheritanceTests {
public:
    __attribute__((noinline)) liba::DiamondRoot *createRoot(int val) {
        return new liba::DiamondRoot(val);
    }
    __attribute__((noinline)) liba::DiamondRoot *createLeft(int val, int leftVal) {
        return new liba::DiamondLeft(val, leftVal);
    }
    __attribute__((noinline)) liba::DiamondRoot *createRight(int val, int rightVal) {
        return new liba::DiamondRight(val, rightVal);
    }
    __attribute__((noinline)) liba::DiamondRoot *
    createChild(int val, int leftVal, int rightVal, int childVal) {
        return new liba::DiamondChild(val, leftVal, rightVal, childVal);
    }

    void TestRoot() {
        int initVal = DiamondT::InitVal;
        auto *ptr = createRoot(initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod(), initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod1(), initVal + 1);
        EXPECT_EQ(__func__, ptr->VirtualMethod2(), initVal - 1);
        EXPECT_EQ(__func__, ptr->VirtualMethod3(), initVal * 2);
        delete ptr;
    }

    void TestLeft() {
        int initVal = DiamondT::InitVal;
        int leftVal = 10 + initVal;
        auto *ptr = createLeft(initVal, leftVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod(), initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod1(), leftVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod2(), initVal - 1);
        EXPECT_EQ(__func__, ptr->VirtualMethod3(), initVal * 2);
        delete ptr;
    }

    void TestRight() {
        int initVal = DiamondT::InitVal;
        int rightVal = 20 + initVal;
        auto *ptr = createRight(initVal, rightVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod(), initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod1(), initVal + 1);
        EXPECT_EQ(__func__, ptr->VirtualMethod2(), rightVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod3(), initVal * 2);
        delete ptr;
    }

    void TestChild() {
        int initVal = DiamondT::InitVal;
        int leftVal = 10 + initVal;
        int rightVal = 20 + initVal;
        int childVal = 30 + initVal;
        auto *ptr = createChild(initVal, leftVal, rightVal, childVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod(), initVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod1(), leftVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod2(), rightVal);
        EXPECT_EQ(__func__, ptr->VirtualMethod3(), childVal);
        delete ptr;
    }
};

static void TestDiamondChild() {
    VirtualInheritanceTests<InitA<liba::DiamondChild>> tests;
    tests.TestRoot();
    tests.TestLeft();
    tests.TestRight();
    tests.TestChild();
}

int main(int argc, char **argv) {
    CtorDtorTests_TestClassA();
    CtorDtorTests_TestClassB();
    TestVtable_GlobalPoly();
    TestVtable_LocalPoly();
    TestDiamondChild();
    return t_status;
}