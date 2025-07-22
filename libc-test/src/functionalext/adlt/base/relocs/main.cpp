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
#include "relocs_tests.h"

static void RelocTests_dyn_relocs() {
  EXPECT_EQ(__func__, 5, relative_test());
  EXPECT_EQ(__func__, 2, glob_dat_test());
  EXPECT_EQ(__func__, 1, jump_slot_test());
}

static void RelocTests_abs_relocs() { EXPECT_EQ(__func__, 3, abs64_test()); }

static void RelocTests_plt_relocs() {
  EXPECT_EQ(__func__, 1, jump26_test());
  EXPECT_EQ(__func__, 1, call26_test());
  EXPECT_EQ(__func__, 1, condbr19_test());
  EXPECT_EQ(__func__, 1, tstbr14_test());
}

static void RelocTests_got_relocs() {
  EXPECT_EQ(__func__, 1, ld64_got_lo12_nc_test());
  EXPECT_EQ(__func__, 1, ld64_gotpage_lo15_test());
}

static void RelocTests_pc_relative_relocs() {
  EXPECT_EQ(__func__, 1, adr_prel_pg_hi21_test());
  EXPECT_EQ(__func__, 1, adr_prel_pg_hi21_nc_test());
  EXPECT_EQ(__func__, 1, adr_prel_lo21_test());
  EXPECT_EQ(__func__, 1, ld_prel_lo19_test());
}

static void RelocTests_null_relocs() { EXPECT_EQ(__func__, 0, none_test()); }

static void RelocTests_tlsdesc_relocs() { EXPECT_EQ(__func__, 2, tlsdesc_test()); }

static void RelocTests_tls_dyn_relocs() { EXPECT_EQ(__func__, 2, tls_tprel64_test()); }

static void RelocTests_tlsie_relocs() { EXPECT_EQ(__func__, 2, tlsie_adr_ld64_test()); }

int main(int argc, char **argv) {
  RelocTests_dyn_relocs();
  RelocTests_abs_relocs();
  RelocTests_plt_relocs();
  RelocTests_got_relocs();
  RelocTests_pc_relative_relocs();
  RelocTests_null_relocs();
  RelocTests_tlsdesc_relocs();
  RelocTests_tls_dyn_relocs();
  RelocTests_tlsie_relocs();
  return t_status;
}
