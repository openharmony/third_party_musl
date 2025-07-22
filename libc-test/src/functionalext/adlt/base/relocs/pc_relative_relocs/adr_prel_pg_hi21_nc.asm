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
	.text


	.globl	hi21_nc_hidden_func
	.hidden hi21_nc_hidden_func
	.p2align	4
	.type	hi21_nc_hidden_func,@function
hi21_nc_hidden_func:
	mov	w0, #1
	ret
.Lfunc_end0:
	.size	hi21_nc_hidden_func, .Lfunc_end0-hi21_nc_hidden_func


	.globl	adr_prel_pg_hi21_nc_test
	.p2align	2
	.type	adr_prel_pg_hi21_nc_test,@function
adr_prel_pg_hi21_nc_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	adrp x8, :pg_hi21_nc:hi21_nc_hidden_func // R_AARCH64_ADR_PREL_PG_HI21_NC
	add	x9, x8, #:lo12:hi21_nc_hidden_func // R_AARCH64_LDST64_ABS_LO12NC
	blr	x9
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	adr_prel_pg_hi21_nc_test, .Lfunc_end1-adr_prel_pg_hi21_nc_test


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym hi21_nc_hidden_func
