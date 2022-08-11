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
	@ args = 0, pretend = 0, frame = 132704
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	sub	sp, sp, #132096
	sub	sp, sp, #620
	add	r1, sp, #66560
	add	r0, sp, #1632
	add	r1, r1, #616
	add	r0, r0, #8
	bl	process_frame
	mov	r3, #0
	add	r0, sp, #1632
	add	r0, r0, #8
	add	r0, r0, #16
	str	r0, [sp, #68]
	add	r0, sp, #66560
	add	r0, r0, #616
	add	r2, sp, #616
	add	r1, sp, #104
	add	r0, r0, #16
	str	r3, [sp, #56]
	str	r2, [sp, #92]
	str	r1, [sp, #88]
	str	r0, [sp, #100]
	str	r1, [sp, #64]
	str	r2, [sp, #60]
	str	r3, [sp, #16]
	str	r3, [sp, #12]
.L14:
	ldrb	r3, [sp, #56]	@ zero_extendqisi2
	sub	r1, r3, #2
	sxtb	r1, r1
	add	r2, r3, #3
	bic	r1, r1, r1, asr #31
	sxtb	r2, r2
	sub	r3, r3, r1
	uxtb	r3, r3
	cmp	r2, #16
	str	r3, [sp, #96]
	movlt	r3, r2
	movge	r3, #16
	str	r3, [sp, #80]
	ldr	r3, [sp, #68]
	str	r1, [sp, #84]
	add	fp, r3, #240
	str	r3, [sp, #28]
	ldr	r3, [sp, #64]
	str	r3, [sp, #76]
	ldr	r3, [sp, #60]
	str	r3, [sp, #72]
	mov	r3, #0
	str	r3, [sp, #52]
.L21:
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #84]
	cmp	r1, r0
	ble	.L25
	ldrb	r2, [sp, #52]	@ zero_extendqisi2
	add	r3, r2, #3
	sxtb	r3, r3
	cmp	r3, #16
	movlt	ip, r3
	ldr	r3, [sp, #96]
	movge	ip, #16
	str	r3, [sp, #24]
	mvn	r3, #0
	str	r3, [sp, #8]
	mov	r3, #16
	smlabb	r0, r0, r3, ip
	smlabb	r1, r1, r3, ip
	sub	r3, r2, #2
	sxtb	r3, r3
	bic	r3, r3, r3, asr #31
	sub	r2, r3, r2
	str	ip, [sp, #36]
	str	r3, [sp, #32]
	sub	r3, r3, ip
	ldr	ip, [sp, #100]
	uxtb	r2, r2
	add	r0, ip, r0, lsl #8
	add	r1, ip, r1, lsl #8
	lsl	r3, r3, #8
	str	r0, [sp, #20]
	str	r1, [sp, #40]
	str	r2, [sp, #48]
	str	r3, [sp, #44]
.L20:
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #36]
	cmp	r3, r2
	bge	.L16
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #44]
	ldr	r9, [sp, #48]
	add	r10, r3, r2
.L19:
	mov	ip, r10
	mov	r0, #0
	ldr	r1, [sp, #28]
.L17:
	vld1.64	{d16-d17}, [r1:64]!
	vld1.64	{d18-d19}, [ip:64]!
	// This line is performing the absolute difference
	vabd.u8	q8, q8, q9
	// This is the widening addition
	vaddl.u8	q8, d17, d16
	// The individual elements of the d16-d17 must be 
	// placed into the other registers; this causes the
	// the registers to be used up. This allows for introduction
	// of the other instruction.
	vmov.u16	r2, d16[0]
	vmov.u16	r8, d16[1]
	vmov.u16	r3, d16[2]
	vmov.u16	r7, d16[3]
	vmov.u16	r6, d17[0]
	vmov.u16	r5, d17[1]
	vmov.u16	r4, d17[2]
	vmov.u16	lr, d17[3]
	// Changing order of the adds to improve performance.
	// This can be used to remove one line of the addition
	add	r2, r2, r8
	add	r3, r3, r7
	add	r4, r5, r6
	add	r2, r2, lr
	add	r3, r3, r4
	add	r3, r2, r3
	cmp	r1, fp
	add	r0, r0, r3
	bne	.L17
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #24]
	cmp	r3, r0
	ldr	r2, [sp, #16]
	movhi	r2, r1
	movhi	r3, r0
	str	r2, [sp, #16]
	ldr	r2, [sp, #12]
	movhi	r2, r9
	str	r3, [sp, #8]
	ldr	r3, [sp, #20]
	add	r10, r10, #256
	add	r9, r9, #1
	cmp	r10, r3
	str	r2, [sp, #12]
	uxtb	r9, r9
	bne	.L19
.L16:
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #40]
	add	r3, r3, #4096
	str	r3, [sp, #20]
	cmp	r3, r2
	ldr	r3, [sp, #24]
	sub	r3, r3, #1
	uxtb	r3, r3
	str	r3, [sp, #24]
	bne	.L20
.L15:
	ldr	r3, [sp, #52]
	ldr	r2, [sp, #12]
	add	r3, r3, #1
	str	r3, [sp, #52]
	cmp	r3, #16
	ldr	r3, [sp, #76]
	ldr	r1, [sp, #8]
	strb	r2, [r3]
	ldr	r2, [sp, #16]
	add	r3, r3, #2
	strb	r2, [r3, #-1]
	ldr	r2, [sp, #72]
	str	r3, [sp, #76]
	ldr	r3, [sp, #28]
	str	r1, [r2], #4
	add	r3, r3, #256
	str	r2, [sp, #72]
	str	r3, [sp, #28]
	add	fp, fp, #256
	bne	.L21
	ldr	r3, [sp, #56]
	add	r3, r3, #1
	str	r3, [sp, #56]
	cmp	r3, #16
	ldr	r3, [sp, #60]
	add	r3, r3, #64
	str	r3, [sp, #60]
	ldr	r3, [sp, #64]
	add	r3, r3, #32
	str	r3, [sp, #64]
	ldr	r3, [sp, #68]
	add	r3, r3, #4096
	str	r3, [sp, #68]
	bne	.L14
	movw	r7, #:lower16:.LC4
	mov	r6, #0
	ldr	r8, [sp, #92]
	ldr	r10, [sp, #88]
	add	r9, r8, #1024
	movt	r7, #:upper16:.LC4
.L22:
	mov	r5, r10
	mov	fp, r8
	mov	r4, #0
.L23:
	ldrsb	r3, [r5, #1]
	ldr	r2, [fp], #4
	str	r3, [sp]
	str	r2, [sp, #4]
	ldrsb	r3, [r5]
	mov	r2, r4
	mov	r1, r6
	add	r4, r4, #1
	mov	r0, r7
	bl	printf
	cmp	r4, #16
	add	r5, r5, #2
	bne	.L23
	add	r8, r8, #64
	cmp	r9, r8
	add	r6, r6, #1
	add	r10, r10, #32
	bne	.L22
	mov	r0, #0
	add	sp, sp, #132096
	add	sp, sp, #620
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L25:
	mvn	r3, #0
	str	r3, [sp, #8]
	b	.L15
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
	.ascii	"Block[%d][%d]: Vector: (%d, %d); Difference: %d\012"
	.ascii	"\000"
	.ident	"GCC: (GNU) 8.2.1 20180801 (Red Hat 8.2.1-2)"
	.section	.note.GNU-stack,"",%progbits
