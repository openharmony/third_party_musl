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


	.globl	lo21_hidden_func
	.hidden lo21_hidden_func
	.p2align	4
	.type	lo21_hidden_func,@function
lo21_hidden_func:
	mov	w0, #1
	ret
.Lfunc_end0:
	.size	lo21_hidden_func, .Lfunc_end0-lo21_hidden_func


	.globl	adr_prel_lo21_test
	.p2align	2
	.type	adr_prel_lo21_test,@function
adr_prel_lo21_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	adr x8, lo21_hidden_func // R_AARCH64_ADR_PREL_LO21
	blr	x8
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	adr_prel_lo21_test, .Lfunc_end1-adr_prel_lo21_test


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym lo21_hidden_func
