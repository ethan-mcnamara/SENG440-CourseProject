	.arch armv7-a
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bic	r0, r0, r0, asr #31
	bx	lr
	.size	comp_zero, .-comp_zero
	.align	2
	.global	comp_max
	.syntax unified
	.arm
	.fpu neon
	.type	comp_max, %function
comp_max:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #16
	movge	r0, #16
	bx	lr
	.size	comp_max, .-comp_max
	.align	2
	.global	process_frame
	.syntax unified
	.arm
	.fpu neon
	.type	process_frame, %function
process_frame:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	fp, r0
	mov	r4, r1
	movw	r0, #:lower16:.LC1
	movw	r1, #:lower16:.LC0
	sub	sp, sp, #68
	movt	r1, #:upper16:.LC0
	movt	r0, #:upper16:.LC1
	bl	fopen
	movw	r1, #:lower16:.LC0
	mov	r8, r0
	movw	r0, #:lower16:.LC2
	movt	r1, #:upper16:.LC0
	movt	r0, #:upper16:.LC2
	bl	fopen
	mov	r7, r0
	mov	r3, r8
	mov	r2, #1
	mov	r1, #54
	add	r0, sp, #8
	bl	fread
	mov	r3, r7
	add	r0, sp, #8
	mov	r2, #1
	mov	r1, #54
	bl	fread
	cmp	r7, #0
	cmpne	r8, #0
	beq	.L5
	add	r3, fp, #69632
	add	r3, r3, #256
	str	r4, [sp]
	str	r3, [sp, #4]
	add	r9, fp, #4352
.L6:
	ldr	r3, [sp]
	sub	r6, r9, #256
	sub	r10, r3, #4096
	sub	r10, r10, fp
.L10:
	sub	r4, r6, #4096
	add	r5, r10, r6
.L7:
	mov	r3, r8
	mov	r2, #1
	mov	r1, #16
	mov	r0, r4
	bl	fread
	add	r4, r4, #256
	mov	r0, r5
	mov	r3, r7
	mov	r2, #1
	mov	r1, #16
	bl	fread
	cmp	r4, r6
	add	r5, r5, #256
	bne	.L7
	add	r6, r4, #16
	cmp	r6, r9
	bne	.L10
	ldr	r3, [sp, #4]
	add	r9, r6, #4096
	cmp	r9, r3
	ldr	r3, [sp]
	add	fp, fp, #4096
	add	r3, r3, #4096
	str	r3, [sp]
	bne	.L6
	mov	r0, r8
	bl	fclose
	mov	r0, r7
	bl	fclose
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L5:
	movw	r0, #:lower16:.LC3
	movt	r0, #:upper16:.LC3
	bl	printf
	mov	r0, #1
	bl	exit
	.size	process_frame, .-process_frame
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.syntax unified
	.arm
	.fpu neon
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 132664
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	sub	sp, sp, #132096
	sub	sp, sp, #580
	bl	clock
	add	r1, sp, #66560
	str	r0, [sp, #48]
	add	r1, r1, #576
	add	r0, sp, #1600
	bl	process_frame
	mov	r1, #0
	mov	r3, #1
	.syntax divided
@ 158 "main.c" 1
	Manual_SAD r9 r1 r3
@ 0 "" 2
	.arm
	.syntax unified
	mov	r2, #3
	mov	r3, #2
	.syntax divided
@ 163 "main.c" 1
	Manual_SAD r8 r3 r2
@ 0 "" 2
	.arm
	.syntax unified
	add	r3, sp, #64
	mov	r2, r3
	str	r3, [sp, #44]
	add	r3, r8, r9, lsl #1
	add	r3, r8, r3
	str	r3, [sp, #12]
	mov	r3, r2
	mov	r10, r1
	mov	fp, r1
	add	r2, r2, #1
	str	r3, [sp, #24]
	add	r3, sp, #576
	str	r1, [sp, #20]
	str	r2, [sp, #28]
	str	r3, [sp, #52]
	str	r3, [sp, #32]
.L14:
	ldrb	r2, [sp, #20]	@ zero_extendqisi2
	add	r3, r2, #3
	sxtb	r3, r3
	cmp	r3, #16
	movge	r3, #16
	str	r2, [sp, #60]
	sub	r2, r2, #2
	sxtb	r2, r2
	str	r3, [sp, #40]
	bic	r3, r2, r2, asr #31
	str	r3, [sp, #56]
	ldr	r3, [sp, #32]
	str	r3, [sp, #36]
	mov	r3, #0
	str	r3, [sp, #16]
.L20:
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #56]
	cmp	r2, r3
	ble	.L25
	ldrb	r7, [sp, #16]	@ zero_extendqisi2
	mvn	r1, #0
	add	r5, r7, #3
	sxtb	r5, r5
	cmp	r5, #16
	movge	r5, #16
	uxtb	ip, r3
	sub	r6, r3, ip
	ldr	r3, [sp, #60]
	sub	r4, r7, #2
	add	r6, r3, r6
	sub	r6, r6, r2
	sub	ip, r3, ip
	sxtb	r4, r4
	uxtb	r6, r6
	uxtb	ip, ip
	bic	r4, r4, r4, asr #31
.L19:
	cmp	r5, r4
	ble	.L16
	ldr	r3, [sp, #12]
	add	r2, r9, r8
	add	r2, r3, r2, lsl #2
	add	r2, r2, r9, lsl #1
	add	r2, r2, r8, lsl #1
	add	r2, r2, r9, lsl #1
	add	r2, r2, r8, lsl #1
	add	r2, r2, r9, lsl #1
	add	r2, r2, r8, lsl #1
	sub	r3, r4, r7
	add	r2, r2, r9, lsl #1
	uxtb	r3, r3
	add	r2, r2, r8, lsl #1
	add	lr, r3, r5
	add	r2, r2, r9
	sub	lr, lr, r4
	add	r2, r8, r2
	uxtb	lr, lr
.L17:
	cmp	r1, r2
	add	r0, r3, #1
	movhi	fp, r3
	uxtb	r3, r0
	movhi	r10, ip
	movhi	r1, r2
	cmp	r3, lr
	bne	.L17
.L16:
	sub	ip, ip, #1
	uxtb	ip, ip
	cmp	r6, ip
	bne	.L19
.L15:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #24]
	strb	fp, [r2, r3, lsl #1]
	ldr	r2, [sp, #28]
	strb	r10, [r2, r3, lsl #1]
	add	r3, r3, #1
	str	r3, [sp, #16]
	cmp	r3, #16
	ldr	r3, [sp, #36]
	str	r1, [r3], #4
	str	r3, [sp, #36]
	bne	.L20
	ldr	r3, [sp, #20]
	add	r3, r3, #1
	str	r3, [sp, #20]
	cmp	r3, #16
	ldr	r3, [sp, #24]
	add	r3, r3, #32
	str	r3, [sp, #24]
	add	r3, r2, #32
	str	r3, [sp, #28]
	ldr	r3, [sp, #32]
	add	r3, r3, #64
	str	r3, [sp, #32]
	bne	.L14
	bl	clock
	ldr	r3, [sp, #48]
	vldr.64	d19, .L31
	sub	r0, r0, r3
	vmov	s15, r0	@ int
	vcvt.f64.s32	d16, s15
	vmov.i64	d17, #0	@ float
	vdiv.f64	d18, d16, d19
	movw	r0, #:lower16:.LC4
	vadd.f64	d16, d18, d17
	movw	r8, #:lower16:.LC5
	mov	r7, #0
	vmov	r2, r3, d16
	movt	r0, #:upper16:.LC4
	bl	printf
	ldr	r10, [sp, #44]
	ldr	r9, [sp, #52]
	add	r4, sp, #1600
	movt	r8, #:upper16:.LC5
.L22:
	mov	r6, r10
	mov	fp, r9
	mov	r5, #0
.L23:
	ldrsb	r3, [r6, #1]
	ldr	r2, [fp], #4
	str	r3, [sp]
	str	r2, [sp, #4]
	ldrsb	r3, [r6]
	mov	r2, r5
	mov	r1, r7
	add	r5, r5, #1
	mov	r0, r8
	bl	printf
	cmp	r5, #16
	add	r6, r6, #2
	bne	.L23
	add	r9, r9, #64
	cmp	r4, r9
	add	r7, r7, #1
	add	r10, r10, #32
	bne	.L22
	mov	r0, #0
	add	sp, sp, #132096
	add	sp, sp, #580
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L25:
	mvn	r1, #0
	b	.L15
.L32:
	.align	3
.L31:
	.word	0
	.word	1093567616
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"rb\000"
	.space	1
.LC1:
	.ascii	"test_images/Image1.bmp\000"
	.space	1
.LC2:
	.ascii	"test_images/Image2.bmp\000"
	.space	1
.LC3:
	.ascii	"Error!\000"
	.space	1
.LC4:
	.ascii	"The elapsed time is %f seconds\012\000"
.LC5:
	.ascii	"Block[%d][%d]: Vector: (%d, %d); Difference: %d\012"
	.ascii	"\000"
	.ident	"GCC: (GNU) 8.2.1 20180801 (Red Hat 8.2.1-2)"
	.section	.note.GNU-stack,"",%progbits
