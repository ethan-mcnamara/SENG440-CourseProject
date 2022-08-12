	.arch armv7-a
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	2
	.global	comp_zero
	.arch armv7-a
	.syntax unified
	.arm
	.fpu neon
	.type	comp_zero, %function
comp_zero:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	mov	r3, r0
	strb	r3, [fp, #-5]
	ldrsb	r3, [fp, #-5]
	bic	r3, r3, r3, asr #31
	sxtb	r3, r3
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	comp_zero, .-comp_zero
	.align	2
	.global	comp_max
	.syntax unified
	.arm
	.fpu neon
	.type	comp_max, %function
comp_max:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	mov	r3, r0
	strb	r3, [fp, #-5]
	ldrsb	r3, [fp, #-5]
	cmp	r3, #16
	movlt	r3, r3
	movge	r3, #16
	sxtb	r3, r3
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	comp_max, .-comp_max
	.section	.rodata
	.align	2
.LC0:
	.ascii	"rb\000"
	.align	2
.LC1:
	.ascii	"test_images/Image1.bmp\000"
	.align	2
.LC2:
	.ascii	"test_images/Image2.bmp\000"
	.align	2
.LC3:
	.ascii	"Error!\000"
	.text
	.align	2
	.global	process_frame
	.syntax unified
	.arm
	.fpu neon
	.type	process_frame, %function
process_frame:
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #92
	str	r0, [fp, #-96]
	str	r1, [fp, #-100]
	movw	r1, #:lower16:.LC0
	movt	r1, #:upper16:.LC0
	movw	r0, #:lower16:.LC1
	movt	r0, #:upper16:.LC1
	bl	fopen
	str	r0, [fp, #-28]
	movw	r1, #:lower16:.LC0
	movt	r1, #:upper16:.LC0
	movw	r0, #:lower16:.LC2
	movt	r0, #:upper16:.LC2
	bl	fopen
	str	r0, [fp, #-32]
	sub	r0, fp, #88
	ldr	r3, [fp, #-28]
	mov	r2, #1
	mov	r1, #54
	bl	fread
	sub	r0, fp, #88
	ldr	r3, [fp, #-32]
	mov	r2, #1
	mov	r1, #54
	bl	fread
	ldr	r3, [fp, #-28]
	cmp	r3, #0
	beq	.L6
	ldr	r3, [fp, #-32]
	cmp	r3, #0
	bne	.L7
.L6:
	movw	r0, #:lower16:.LC3
	movt	r0, #:upper16:.LC3
	bl	printf
	mov	r0, #1
	bl	exit
.L7:
	mov	r4, #16
	mov	r3, #0
	str	r3, [fp, #-16]
	b	.L8
.L13:
	mov	r3, #0
	str	r3, [fp, #-20]
	b	.L9
.L12:
	mov	r3, #0
	str	r3, [fp, #-24]
	b	.L10
.L11:
	ldr	r3, [fp, #-16]
	lsl	r3, r3, #12
	ldr	r2, [fp, #-96]
	add	r2, r2, r3
	ldr	r3, [fp, #-24]
	lsl	r1, r3, #4
	ldr	r3, [fp, #-20]
	add	r3, r1, r3
	lsl	r3, r3, #4
	add	r0, r2, r3
	ldr	r3, [fp, #-28]
	mov	r2, #1
	mov	r1, r4
	bl	fread
	ldr	r3, [fp, #-16]
	lsl	r3, r3, #12
	ldr	r2, [fp, #-100]
	add	r2, r2, r3
	ldr	r3, [fp, #-24]
	lsl	r1, r3, #4
	ldr	r3, [fp, #-20]
	add	r3, r1, r3
	lsl	r3, r3, #4
	add	r0, r2, r3
	ldr	r3, [fp, #-32]
	mov	r2, #1
	mov	r1, r4
	bl	fread
	ldr	r3, [fp, #-24]
	add	r3, r3, #1
	str	r3, [fp, #-24]
.L10:
	ldr	r3, [fp, #-24]
	cmp	r3, #15
	ble	.L11
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
.L9:
	ldr	r3, [fp, #-20]
	cmp	r3, #15
	ble	.L12
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-16]
.L8:
	ldr	r3, [fp, #-16]
	cmp	r3, #15
	ble	.L13
	ldr	r0, [fp, #-28]
	bl	fclose
	ldr	r0, [fp, #-32]
	bl	fclose
	nop
	sub	sp, fp, #8
	@ sp needed
	pop	{r4, fp, pc}
	.size	process_frame, .-process_frame
	.section	.rodata
	.align	2
.LC4:
	.ascii	"The elapsed time is %f seconds\012\000"
	.align	2
.LC5:
	.ascii	"Block[%d][%d]: Vector: (%d, %d); Difference: %d\012"
	.ascii	"\000"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu neon
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 132856
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #132096
	sub	sp, sp, #768
	sub	r3, fp, #131072
	sub	r3, r3, #12
	str	r0, [r3, #-1780]
	sub	r3, fp, #131072
	sub	r3, r3, #12
	str	r1, [r3, #-1784]
	mov	r2, #0
	mov	r3, #0
	strd	r2, [fp, #-52]
	bl	clock
	str	r0, [fp, #-56]
	sub	r2, fp, #131072
	sub	r2, r2, #12
	sub	r2, r2, #240
	sub	r3, fp, #65536
	sub	r3, r3, #12
	sub	r3, r3, #240
	mov	r1, r2
	mov	r0, r3
	bl	process_frame
	mov	r4, #0
	mvn	r5, #0
	mov	r3, #0
	strb	r3, [fp, #-13]
	mov	r3, #0
	strb	r3, [fp, #-14]
	mov	r3, #0
	str	r3, [fp, #-20]
	b	.L16
.L34:
	mov	r3, #0
	str	r3, [fp, #-24]
	b	.L17
.L33:
	ldr	r3, [fp, #-20]
	uxtb	r3, r3
	sub	r3, r3, #2
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_zero
	mov	r3, r0
	str	r3, [fp, #-28]
	b	.L18
.L32:
	ldr	r3, [fp, #-24]
	uxtb	r3, r3
	sub	r3, r3, #2
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_zero
	mov	r3, r0
	str	r3, [fp, #-32]
	b	.L19
.L31:
	mov	r4, #0
	mov	r3, #1
	str	r3, [fp, #-36]
	b	.L20
.L29:
	vldr	d16, .L40
	vldr	d17, .L40+8
	vstr	d16, [fp, #-76]
	vstr	d17, [fp, #-68]
	vldr	d16, .L40
	vldr	d17, .L40+8
	vstr	d16, [fp, #-92]
	vstr	d17, [fp, #-84]
	vldr	d16, [fp, #-76]
	vldr	d17, [fp, #-68]
	vstr	d16, [fp, #-252]
	vstr	d17, [fp, #-244]
	vldr	d16, [fp, #-252]
	vldr	d17, [fp, #-244]
	vmov.32	r3, d16[0]
	str	r3, [fp, #-96]
	vldr	d16, [fp, #-92]
	vldr	d17, [fp, #-84]
	vstr	d16, [fp, #-236]
	vstr	d17, [fp, #-228]
	vldr	d16, [fp, #-236]
	vldr	d17, [fp, #-228]
	vmov.32	r3, d16[0]
	str	r3, [fp, #-100]
	ldr	r3, [fp, #-96]
	ldr	r2, [fp, #-100]
	.syntax divided
@ 158 "main.c" 1
	Manual_SAD r3 r3 r2
@ 0 "" 2
	.arm
	.syntax unified
	str	r3, [fp, #-104]
	ldr	r3, [fp, #-104]
	add	r4, r4, r3
	vldr	d16, [fp, #-76]
	vldr	d17, [fp, #-68]
	vstr	d16, [fp, #-220]
	vstr	d17, [fp, #-212]
	vldr	d16, [fp, #-220]
	vldr	d17, [fp, #-212]
	vmov.32	r3, d16[1]
	str	r3, [fp, #-96]
	vldr	d16, [fp, #-92]
	vldr	d17, [fp, #-84]
	vstr	d16, [fp, #-204]
	vstr	d17, [fp, #-196]
	vldr	d16, [fp, #-204]
	vldr	d17, [fp, #-196]
	vmov.32	r3, d16[1]
	str	r3, [fp, #-100]
	ldr	r3, [fp, #-96]
	ldr	r2, [fp, #-100]
	.syntax divided
@ 165 "main.c" 1
	Manual_SAD r3 r3 r2
@ 0 "" 2
	.arm
	.syntax unified
	str	r3, [fp, #-104]
	ldr	r3, [fp, #-104]
	add	r4, r4, r3
	vldr	d16, [fp, #-76]
	vldr	d17, [fp, #-68]
	vstr	d16, [fp, #-188]
	vstr	d17, [fp, #-180]
	vldr	d16, [fp, #-188]
	vldr	d17, [fp, #-180]
	vmov.32	r3, d17[0]
	str	r3, [fp, #-96]
	vldr	d16, [fp, #-92]
	vldr	d17, [fp, #-84]
	vstr	d16, [fp, #-172]
	vstr	d17, [fp, #-164]
	vldr	d16, [fp, #-172]
	vldr	d17, [fp, #-164]
	vmov.32	r3, d17[0]
	str	r3, [fp, #-100]
	ldr	r3, [fp, #-96]
	ldr	r2, [fp, #-100]
	.syntax divided
@ 172 "main.c" 1
	Manual_SAD r3 r3 r2
@ 0 "" 2
	.arm
	.syntax unified
	str	r3, [fp, #-104]
	ldr	r3, [fp, #-104]
	add	r4, r4, r3
	vldr	d16, [fp, #-76]
	vldr	d17, [fp, #-68]
	vstr	d16, [fp, #-156]
	vstr	d17, [fp, #-148]
	vldr	d16, [fp, #-156]
	vldr	d17, [fp, #-148]
	vmov.32	r3, d17[1]
	str	r3, [fp, #-96]
	vldr	d16, [fp, #-92]
	vldr	d17, [fp, #-84]
	vstr	d16, [fp, #-140]
	vstr	d17, [fp, #-132]
	vldr	d16, [fp, #-140]
	vldr	d17, [fp, #-132]
	vmov.32	r3, d17[1]
	str	r3, [fp, #-100]
	ldr	r3, [fp, #-96]
	ldr	r2, [fp, #-100]
	.syntax divided
@ 179 "main.c" 1
	Manual_SAD r3 r3 r2
@ 0 "" 2
	.arm
	.syntax unified
	str	r3, [fp, #-104]
	ldr	r3, [fp, #-104]
	add	r4, r4, r3
	ldr	r3, [fp, #-36]
	add	r3, r3, #1
	str	r3, [fp, #-36]
.L20:
	ldr	r3, [fp, #-36]
	cmp	r3, #15
	ble	.L29
	cmp	r5, r4
	bls	.L30
	mov	r5, r4
	ldr	r3, [fp, #-32]
	uxtb	r2, r3
	ldr	r3, [fp, #-24]
	uxtb	r3, r3
	sub	r3, r2, r3
	strb	r3, [fp, #-13]
	ldr	r3, [fp, #-20]
	uxtb	r2, r3
	ldr	r3, [fp, #-28]
	uxtb	r3, r3
	sub	r3, r2, r3
	strb	r3, [fp, #-14]
.L30:
	ldr	r3, [fp, #-32]
	add	r3, r3, #1
	str	r3, [fp, #-32]
.L19:
	ldr	r3, [fp, #-24]
	uxtb	r3, r3
	add	r3, r3, #3
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_max
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-32]
	cmp	r3, r2
	blt	.L31
	ldr	r3, [fp, #-28]
	add	r3, r3, #1
	str	r3, [fp, #-28]
.L18:
	ldr	r3, [fp, #-20]
	uxtb	r3, r3
	add	r3, r3, #3
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_max
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-28]
	cmp	r3, r2
	blt	.L32
	sub	r3, fp, #131072
	sub	r3, r3, #12
	mov	r1, r3
	ldr	r3, [fp, #-20]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-24]
	add	r3, r2, r3
	lsl	r3, r3, #2
	add	r3, r1, r3
	str	r5, [r3, #-1264]
	ldrsb	r1, [fp, #-13]
	sub	r3, fp, #131072
	sub	r3, r3, #12
	mov	r0, r3
	ldr	r3, [fp, #-20]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-24]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r0, r3
	mov	r2, r1
	strb	r2, [r3, #-1776]
	ldrsb	r1, [fp, #-14]
	sub	r3, fp, #131072
	sub	r3, r3, #12
	mov	r0, r3
	ldr	r3, [fp, #-20]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-24]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r0, r3
	mov	r2, r1
	strb	r2, [r3, #-1775]
	mvn	r5, #0
	ldr	r3, [fp, #-24]
	add	r3, r3, #1
	str	r3, [fp, #-24]
.L17:
	ldr	r3, [fp, #-24]
	cmp	r3, #15
	ble	.L33
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
.L16:
	ldr	r3, [fp, #-20]
	cmp	r3, #15
	ble	.L34
	bl	clock
	str	r0, [fp, #-108]
	ldr	r2, [fp, #-108]
	ldr	r3, [fp, #-56]
	sub	r3, r2, r3
	vmov	s15, r3	@ int
	vcvt.f64.s32	d17, s15
	vldr.64	d18, .L40+16
	vdiv.f64	d16, d17, d18
	vldr.64	d17, [fp, #-52]
	vadd.f64	d16, d17, d16
	vstr.64	d16, [fp, #-52]
	ldrd	r2, [fp, #-52]
	movw	r0, #:lower16:.LC4
	movt	r0, #:upper16:.LC4
	bl	printf
	mov	r3, #0
	str	r3, [fp, #-40]
	b	.L35
.L38:
	mov	r3, #0
	str	r3, [fp, #-44]
	b	.L36
.L41:
	.align	3
.L40:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	0
	.word	1093567616
.L37:
	sub	r3, fp, #131072
	sub	r3, r3, #12
	mov	r1, r3
	ldr	r3, [fp, #-40]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-44]
	add	r3, r2, r3
	lsl	r3, r3, #2
	add	r3, r1, r3
	ldr	r3, [r3, #-1264]
	str	r3, [fp, #-112]
	sub	r3, fp, #131072
	sub	r3, r3, #12
	mov	r1, r3
	ldr	r3, [fp, #-40]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-44]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r1, r3
	sub	r3, r3, #1776
	ldrsb	r3, [r3]
	str	r3, [fp, #-116]
	sub	r3, fp, #131072
	sub	r3, r3, #12
	mov	r1, r3
	ldr	r3, [fp, #-40]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-44]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r1, r3
	sub	r3, r3, #1760
	sub	r3, r3, #15
	ldrsb	r3, [r3]
	str	r3, [fp, #-120]
	ldr	r3, [fp, #-112]
	str	r3, [sp, #4]
	ldr	r3, [fp, #-120]
	str	r3, [sp]
	ldr	r3, [fp, #-116]
	ldr	r2, [fp, #-44]
	ldr	r1, [fp, #-40]
	movw	r0, #:lower16:.LC5
	movt	r0, #:upper16:.LC5
	bl	printf
	ldr	r3, [fp, #-44]
	add	r3, r3, #1
	str	r3, [fp, #-44]
.L36:
	ldr	r3, [fp, #-44]
	cmp	r3, #15
	ble	.L37
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
.L35:
	ldr	r3, [fp, #-40]
	cmp	r3, #15
	ble	.L38
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #12
	@ sp needed
	pop	{r4, r5, fp, pc}
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180801 (Red Hat 8.2.1-2)"
	.section	.note.GNU-stack,"",%progbits
