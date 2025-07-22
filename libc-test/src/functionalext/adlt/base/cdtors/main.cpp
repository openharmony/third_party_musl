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
#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include "functionalext.h"
#include "a.h"
#include "b.h"

static void ApiTests_ClassAFlagsAPI() {
  int initA = 0b01;
  auto sp_a = std::make_unique<liba::ClassA>(initA);
  EXPECT_EQ(__func__, sp_a->Value(), 0b01);
  sp_a->SetFlag(1);
  auto ret = sp_a->Value();
  sp_a.reset();
  EXPECT_EQ(__func__, ret, 0b11);
}

static void ApiTests_ClassBFlagsAPI() {
  int initB = 0b10;
  auto sp_b = std::make_unique<libb::ClassB>(initB);
  EXPECT_EQ(__func__, sp_b->Value(), 0b10);
  sp_b->SetFlag(0);
  auto ret = sp_b->Value();
  EXPECT_EQ(__func__, ret, 0b11);
}

static void CtorTests_TestACtors() {
  const auto& so_a_data = liba::ClassA::Instance();
  so_a_data.Log("CtorTests::TestACtors");
  EXPECT_TRUE(__func__, so_a_data.HasFlag(liba::INIT_0));
  EXPECT_TRUE(__func__, so_a_data.HasFlag(liba::INIT_1));
  EXPECT_FALSE(__func__, so_a_data.HasFlag(liba::INIT_2));
  EXPECT_FALSE(__func__, so_a_data.HasFlag(liba::INIT_3));
  EXPECT_FALSE(__func__, so_a_data.HasFlag(liba::FINI_0));
  EXPECT_FALSE(__func__, so_a_data.HasFlag(liba::FINI_1));
  EXPECT_FALSE(__func__, so_a_data.HasFlag(liba::FINI_2));
  EXPECT_FALSE(__func__, so_a_data.HasFlag(liba::FINI_3));
}

static void CtorTests_TestBCtors() {
  const auto& so_b_data = libb::ClassB::Instance();
  so_b_data.Log("CtorTests::TestBCtors");
  EXPECT_TRUE(__func__, so_b_data.HasFlag(liba::INIT_0));
  EXPECT_TRUE(__func__, so_b_data.HasFlag(liba::INIT_1));
  EXPECT_TRUE(__func__, so_b_data.HasFlag(liba::INIT_2));
  EXPECT_FALSE(__func__, so_b_data.HasFlag(liba::INIT_3));
  EXPECT_FALSE(__func__, so_b_data.HasFlag(liba::FINI_0));
  EXPECT_FALSE(__func__, so_b_data.HasFlag(liba::FINI_1));
  EXPECT_FALSE(__func__, so_b_data.HasFlag(liba::FINI_2));
  EXPECT_FALSE(__func__, so_b_data.HasFlag(liba::FINI_3));
}

static void CtorTests_TestAExternStaticInit() {
  EXPECT_EQ(__func__, liba::class_a_instance_0.Value(), 42+0);
  EXPECT_EQ(__func__, liba::class_a_instance_1.Value(), 42+1);
}

static void CtorTests_TestBExternStaticInit() {
   EXPECT_EQ(__func__, libb::class_b_instance_0.Value(), 42+0);
   EXPECT_EQ(__func__, libb::class_b_instance_1.Value(), 42+1);
   EXPECT_EQ(__func__, libb::class_b_instance_2.Value(), 42+2);
}

int main(int argc, char **argv) {
  ApiTests_ClassAFlagsAPI();
  ApiTests_ClassBFlagsAPI();
  CtorTests_TestACtors();
  CtorTests_TestBCtors();
  CtorTests_TestAExternStaticInit();
  CtorTests_TestBExternStaticInit();
  return t_status;
}
