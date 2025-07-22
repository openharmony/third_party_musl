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


	.globl	ld_prel_lo19_test
	.p2align	2
	.type	ld_prel_lo19_test,@function
ld_prel_lo19_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	ldr x0, lo19_hidden_var // R_AARCH64_LD_PREL_LO19
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	ld_prel_lo19_test, .Lfunc_end1-ld_prel_lo19_test


	.type	lo19_hidden_var,@object
	.globl	lo19_hidden_var
	.hidden lo19_hidden_var
	.p2align	2
	.data
lo19_hidden_var:
	.word	1
	.size	lo19_hidden_var, 4


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym lo19_hidden_var
