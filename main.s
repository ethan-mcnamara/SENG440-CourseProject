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
	@ args = 0, pretend = 0, frame = 131072
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	sub	sp, sp, #131072
	mov	r0, sp
	add	r1, sp, #65536
	bl	process_frame
	mov	r5, #16
	mov	r0, #254
.L14:
	add	ip, r0, #5
	sxtb	ip, ip
	cmp	ip, #16
	movge	ip, #16
	sxtb	r4, r0
	bic	r4, r4, r4, asr #31
	cmp	r4, ip
	movlt	lr, #254
	bge	.L20
.L18:
	add	r3, lr, #5
	sxtb	r3, r3
	cmp	r3, #16
	movlt	r1, r3
	movge	r1, #16
	sxtb	r3, lr
	bic	r3, r3, r3, asr #31
	cmp	r3, r1
	bge	.L15
	smlabb	r2, r4, r5, r1
	smlabb	r6, ip, r5, r1
	sub	r3, r3, r1
	lsl	r1, r3, #8
	add	r3, sp, #65536
	add	r2, r3, r2, lsl #8
	add	r6, r3, r6, lsl #8
.L17:
	add	r3, r2, r1
.L16:
	add	r3, r3, #256
	cmp	r2, r3
	bne	.L16
	add	r2, r2, #4096
	cmp	r6, r2
	bne	.L17
.L15:
	add	lr, lr, #1
	uxtb	lr, lr
	cmp	lr, #14
	bne	.L18
.L20:
	add	r0, r0, #1
	uxtb	r0, r0
	cmp	r0, #14
	bne	.L14
	mov	r0, #0
	add	sp, sp, #131072
	@ sp needed
	pop	{r4, r5, r6, pc}
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
	.ident	"GCC: (GNU) 8.2.1 20180801 (Red Hat 8.2.1-2)"
	.section	.note.GNU-stack,"",%progbits
