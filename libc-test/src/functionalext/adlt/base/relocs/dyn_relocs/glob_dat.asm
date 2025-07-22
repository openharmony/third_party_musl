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


	.globl	glob_dat_f1
	.p2align	2
	.type	glob_dat_f1,@function
glob_dat_f1:
	mov	w0, #1
	ret
.Lfunc_end0:
	.size	glob_dat_f1, .Lfunc_end0-glob_dat_f1


	.globl	glob_dat_test
	.p2align	2
	.type	glob_dat_test,@function
glob_dat_test:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]
	add	x29, sp, #16
	adrp	x8, :got:glob_dat_f1
	ldr	x8, [x8, :got_lo12:glob_dat_f1]
	str	x8, [sp, #8]
	adrp	x8, :got:glob_var1
	ldr	x8, [x8, :got_lo12:glob_var1]
	str	x8, [sp]
	ldr	x8, [sp, #8]
	blr	x8
	ldr	x8, [sp]
	ldr	w8, [x8]
	add	w0, w0, w8
	ldp	x29, x30, [sp, #16]
	add	sp, sp, #32
	ret
.Lfunc_end1:
	.size	glob_dat_test, .Lfunc_end1-glob_dat_test


	.type	glob_var1,@object
	.data
	.globl	glob_var1
	.p2align	2
glob_var1:
	.word	1
	.size	glob_var1, 4


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym glob_dat_f1
	.addrsig_sym glob_var1
