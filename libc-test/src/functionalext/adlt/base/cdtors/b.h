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
#ifndef _ADLT_TEST_CDTORS_B_H_
#define _ADLT_TEST_CDTORS_B_H_

namespace libb {

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

class ClassB {
public:
  explicit ClassB(unsigned value);
  ~ClassB();

  static ClassB& Instance();
  void Log(const char* message="") const;

  unsigned Value() const;
  void SetFlag(unsigned bit);
  bool HasFlag(unsigned bit) const;

private:
  int object_id;
  unsigned value = 0;
};

extern "C" const ClassB
    class_b_instance_0,
    class_b_instance_1,
    class_b_instance_2;

} // namespace libb
#endif