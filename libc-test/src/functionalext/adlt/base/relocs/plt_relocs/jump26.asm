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


	.globl	jump26_f1
	.p2align	2
	.type	jump26_f1,@function
jump26_f1:
	b jump26_label
.Lfunc_end0:
	.size	jump26_f1, .Lfunc_end0-jump26_f1


.globl	jump26_label
.p2align	2
jump26_label:
	mov	w0, #1
	ret


	.globl	jump26_test
	.p2align	2
	.type	jump26_test,@function
jump26_test:
	stp	x29, x30, [sp, #-16]!
	mov	x29, sp
	bl	jump26_f1
	ldp	x29, x30, [sp], #16
	ret
.Lfunc_end1:
	.size	jump26_test, .Lfunc_end1-jump26_test


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym jump26_f1
