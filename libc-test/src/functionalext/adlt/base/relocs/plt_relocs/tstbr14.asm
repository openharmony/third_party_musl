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


	.globl	tstbr14_f1
	.p2align	2
	.type	tstbr14_f1,@function
tstbr14_f1:
	tbz w0, #1, tstbr14_label
.Lfunc_end0:
	.size	tstbr14_f1, .Lfunc_end0-tstbr14_f1


.globl	tstbr14_label
.p2align	2
tstbr14_label:
	mov	w0, #1
	ret


	.globl	tstbr14_test
	.p2align	2
	.type	tstbr14_test,@function
tstbr14_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	bl	tstbr14_f1
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	tstbr14_test, .Lfunc_end1-tstbr14_test


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym tstbr14_f1
