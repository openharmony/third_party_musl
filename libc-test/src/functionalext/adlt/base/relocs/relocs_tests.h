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
#ifndef _ADLT_TEST_RELOCS_H_
#define _ADLT_TEST_RELOCS_H_

extern "C" {
// Dynamic relocations
int relative_test(void);  // R_AARCH64_RELATIVE
int glob_dat_test(void);  // R_AARCH64_GLOB_DAT
int jump_slot_test(void); // R_AARCH64_JUMP_SLOT

// ABS relocations
int abs64_test(void); // R_AARCH64_ABS64

// PLT relocations
int jump26_test(void);   // R_AARCH64_JUMP26
int call26_test(void);   // R_AARCH64_CALL26
int condbr19_test(void); // R_AARCH64_CONDBR19
int tstbr14_test(void);  // R_AARCH64_TSTBR14

// GOT relocations
// R_AARCH64_ADR_GOT_PAGE and R_AARCH64_LD64_GOT_LO12_NC
int ld64_got_lo12_nc_test(void);
// R_AARCH64_ADR_GOT_PAGE and R_AARCH64_LD64_GOTPAGE_LO15
int ld64_gotpage_lo15_test(void);

// Relocations to generate 19, 21 and 33 bit PC-relative addresses
// R_AARCH64_ADD_ABS_LO12_NC and R_AARCH64_ADR_PREL_PG_HI21
int adr_prel_pg_hi21_test(void);
// R_AARCH64_ADD_ABS_LO12_NC and R_AARCH64_ADR_PREL_PG_HI21_NC
int adr_prel_pg_hi21_nc_test(void);
// R_AARCH64_ADR_PREL_LO21
int adr_prel_lo21_test(void);
// R_AARCH64_LD_PREL_LO19
int ld_prel_lo19_test(void);

// Null relocations
int none_test(void); // R_AARCH64_NONE

// TLS descriptor relocations
// R_AARCH64_TLSDESC, R_AARCH64_TLSDESC_ADR_PAGE21, R_AARCH64_TLSDESC_LD64_LO12,
// R_AARCH64_TLSDESC_ADD_LO12 and R_AARCH64_TLSDESC_CALL
int tlsdesc_test(void);

// TLS dynamic relocations
int tls_tprel64_test(void); // R_AARCH64_TLS_TPREL64

// TLS relocations init exec TLSIE
// R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21 and R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC
int tlsie_adr_ld64_test(void);
}
#endif
