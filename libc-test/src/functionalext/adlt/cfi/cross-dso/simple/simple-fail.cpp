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
#if !defined(SHARED_LIB1) && !defined(SHARED_LIB2)
#include "adlt_common.h"
#endif

struct FA {
  virtual void a();
};

void *create_FB();
void *create_FC();

#ifdef SHARED_LIB1

#include "simple_utils.h"
struct FB {
  virtual void a();
};
void FB::a() {}

void *create_FB() {
  make_derive<FB>();
  return (void *)(new FB());
}

#elif defined SHARED_LIB2

#include "simple_utils.h"
struct FC {
  virtual void a();
};
void FC::a() {}

void *create_FC() {
  make_derive<FC>();
  return (void *)(new FC());
}

#else
#include <securec.h>

void FA::a() {}

static void CFICrossDsoSimpleFailTests_TestFail(int argc, char *argv[]) {
  fprintf(stderr, "cross-dso simple fail example\n");

  FA *a;
  void *pb = create_FB();
  void *pc = create_FC();

  fprintf(stderr, "=0=\n");

  if (argc > 1 && argv[1][0] == 'x') {
    // Test cast. BOOM.
    // CFI-DIAG-CAST: runtime error: control flow integrity check for type 'A' failed during cast to unrelated type
    // CFI-DIAG-CAST-NEXT: note: vtable is of type '{{(struct )?}}B'
    a = (FA*)pb;
  } else {
    // Invisible to CFI. Test virtual call later.
    memcpy_s(&a, sizeof(a), &pb, sizeof(pb));
  }

  fprintf(stderr, "=1=\n");

  // CFI-DIAG-CALL: runtime error: control flow integrity check for type 'A'
  // failed during virtual call CFI-DIAG-CALL-NEXT: note: vtable is of type
  // '{{(struct )?}}B'
  a->a(); // UB here

  fprintf(stderr, "=2=\n");

  if (argc > 1 && argv[1][0] == 'x') {
    // Test cast. BOOM.
    // CFI-DIAG-CAST: runtime error: control flow integrity check for type 'A' failed during cast to unrelated type
    // CFI-DIAG-CAST-NEXT: note: vtable is of type '{{(struct )?}}C'
    a = (FA*)pc;
  } else {
    // Invisible to CFI. Test virtual call later.
    memcpy_s(&a, sizeof(a), &pc, sizeof(pc));
  }

  fprintf(stderr, "=3=\n");

  // CFI-DIAG-CALL: runtime error: control flow integrity check for type 'A'
  // failed during virtual call CFI-DIAG-CALL-NEXT: note: vtable is of type
  // '{{(struct )?}}C'
  a->a(); // UB here

  fprintf(stderr, "=4=\n");
}

int main(int argc, char *argv[]) {
  CFICrossDsoSimpleFailTests_TestFail(argc, argv);
  return argc > 1 ? t_status : t_status || run_self_command("x");
}
#endif
