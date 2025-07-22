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


	.globl	tls_tprel64_f1
	.p2align	2
	.type	tls_tprel64_f1,@function
tls_tprel64_f1:
	sub	sp, sp, #16
	str	x0, [sp, #8]

	// R_AARCH64_TLS_TPREL64
	adrp	x9, :gottprel:tls_tprel64_tls_var1
	ldr	x9, [x9, :gottprel_lo12:tls_tprel64_tls_var1]
	mrs	x8, TPIDR_EL0
	ldr	w10, [x8, x9]

	adrp	x9, :got:tls_tprel64_var1
	ldr	x9, [x9, :got_lo12:tls_tprel64_var1]
	ldr	w8, [x9]
	add	w8, w8, w10
	str	w8, [x9]

	mov	x0, xzr
	add	sp, sp, #16
	ret
.Lfunc_end0:
	.size	tls_tprel64_f1, .Lfunc_end0-tls_tprel64_f1


	.globl	tls_tprel64_test
	.p2align	2
	.type	tls_tprel64_test,@function
tls_tprel64_test:
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]
	add	x29, sp, #32
	sub	x0, x29, #8
	mov	x3, xzr
	str	x3, [sp, #8]
	mov	x1, x3
	adrp	x2, :got:tls_tprel64_f1
	ldr	x2, [x2, :got_lo12:tls_tprel64_f1]
	str	x2, [sp]
	bl	pthread_create
	ldr	x2, [sp]
	ldr	x3, [sp, #8]
	add	x0, sp, #16
	mov	x1, x3
	bl	pthread_create
	ldr	x1, [sp, #8]
	ldur	x0, [x29, #-8]
	bl	pthread_join
	ldr	x1, [sp, #8]
	ldr	x0, [sp, #16]
	bl	pthread_join
	adrp	x8, :got:tls_tprel64_var1
	ldr	x8, [x8, :got_lo12:tls_tprel64_var1]
	ldr	w0, [x8]
	ldp	x29, x30, [sp, #32]
	add	sp, sp, #48
	ret
.Lfunc_end3:
	.size	tls_tprel64_test, .Lfunc_end3-tls_tprel64_test


	.type	tls_tprel64_tls_var1,@object
	.section	.tdata,"awT",@progbits
	.globl	tls_tprel64_tls_var1
	.p2align	2
tls_tprel64_tls_var1:
	.word	1
	.size	tls_tprel64_tls_var1, 4


	.type	tls_tprel64_var1,@object
	.bss
	.globl	tls_tprel64_var1
	.p2align	2
tls_tprel64_var1:
	.word	0
	.size	tls_tprel64_var1, 4


	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym tls_tprel64_f1
	.addrsig_sym pthread_create
	.addrsig_sym pthread_join
	.addrsig_sym tls_tprel64_var1
