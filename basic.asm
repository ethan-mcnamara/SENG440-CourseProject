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
	@ args = 0, pretend = 0, frame = 132952
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r5, r6, fp, lr}
	add	fp, sp, #16
	sub	sp, sp, #132096
	sub	sp, sp, #868
	sub	r3, fp, #131072
	sub	r3, r3, #20
	str	r0, [r3, #-1876]
	sub	r3, fp, #131072
	sub	r3, r3, #20
	str	r1, [r3, #-1880]
	sub	r2, fp, #131072
	sub	r2, r2, #20
	sub	r2, r2, #336
	sub	r3, fp, #65536
	sub	r3, r3, #20
	sub	r3, r3, #336
	mov	r1, r2
	mov	r0, r3
	bl	process_frame
	mov	r4, #0
	mov	r5, #0
	mvn	r6, #0
	mov	r3, #0
	strb	r3, [fp, #-21]
	mov	r3, #0
	strb	r3, [fp, #-22]
	mov	r3, #0
	str	r3, [fp, #-28]
	b	.L16
.L38:
	mov	r3, #0
	str	r3, [fp, #-32]
	b	.L17
.L37:
	ldr	r3, [fp, #-28]
	uxtb	r3, r3
	sub	r3, r3, #2
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_zero
	mov	r3, r0
	str	r3, [fp, #-36]
	b	.L18
.L36:
	ldr	r3, [fp, #-32]
	uxtb	r3, r3
	sub	r3, r3, #2
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_zero
	mov	r3, r0
	str	r3, [fp, #-40]
	b	.L19
.L35:
	mov	r4, #0
	mov	r3, #1
	str	r3, [fp, #-44]
	b	.L20
.L33:
	mov	r5, #0
	sub	r3, fp, #65536
	sub	r3, r3, #20
	mov	r1, r3
	ldr	r3, [fp, #-28]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-32]
	add	r3, r2, r3
	lsl	r2, r3, #4
	ldr	r3, [fp, #-44]
	add	r3, r2, r3
	lsl	r3, r3, #4
	add	r3, r1, r3
	sub	r3, r3, #336
	vld1.64	{d16-d17}, [r3:64]
	vstr	d16, [fp, #-68]
	vstr	d17, [fp, #-60]
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r1, r3
	ldr	r3, [fp, #-36]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-40]
	add	r3, r2, r3
	lsl	r2, r3, #4
	ldr	r3, [fp, #-44]
	add	r3, r2, r3
	lsl	r3, r3, #4
	add	r3, r1, r3
	sub	r3, r3, #336
	vld1.64	{d16-d17}, [r3:64]
	vstr	d16, [fp, #-84]
	vstr	d17, [fp, #-76]
	vldr	d16, [fp, #-68]
	vldr	d17, [fp, #-60]
	vstr	d16, [fp, #-340]
	vstr	d17, [fp, #-332]
	vldr	d16, [fp, #-84]
	vldr	d17, [fp, #-76]
	vstr	d16, [fp, #-356]
	vstr	d17, [fp, #-348]
	vldr	d16, [fp, #-340]
	vldr	d17, [fp, #-332]
	vldr	d18, [fp, #-356]
	vldr	d19, [fp, #-348]
	vabd.u8	q8, q8, q9
	vstr	d16, [fp, #-100]
	vstr	d17, [fp, #-92]
	vldr	d16, [fp, #-100]
	vldr	d17, [fp, #-92]
	vstr	d16, [fp, #-324]
	vstr	d17, [fp, #-316]
	vldr	d16, [fp, #-324]
	vldr	d17, [fp, #-316]
	vmov	d16, d17  @ v8qi
	vstr	d16, [fp, #-108]
	vldr	d16, [fp, #-100]
	vldr	d17, [fp, #-92]
	vstr	d16, [fp, #-308]
	vstr	d17, [fp, #-300]
	vldr	d16, [fp, #-308]
	vldr	d17, [fp, #-300]
	vstr	d16, [fp, #-116]
	vldr	d16, [fp, #-108]
	vstr	d16, [fp, #-284]
	vldr	d16, [fp, #-116]
	vstr	d16, [fp, #-292]
	vldr	d16, [fp, #-284]
	vldr	d17, [fp, #-292]
	vaddl.u8	q8, d16, d17
	vstr	d16, [fp, #-132]
	vstr	d17, [fp, #-124]
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-276]
	vstr	d17, [fp, #-268]
	vldr	d16, [fp, #-276]
	vldr	d17, [fp, #-268]
	vmov.u16	r3, d16[0]
	uxth	r3, r3
	add	r4, r4, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-260]
	vstr	d17, [fp, #-252]
	vldr	d16, [fp, #-260]
	vldr	d17, [fp, #-252]
	vmov.u16	r3, d16[1]
	uxth	r3, r3
	add	r5, r5, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-244]
	vstr	d17, [fp, #-236]
	vldr	d16, [fp, #-244]
	vldr	d17, [fp, #-236]
	vmov.u16	r3, d16[2]
	uxth	r3, r3
	add	r4, r4, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-228]
	vstr	d17, [fp, #-220]
	vldr	d16, [fp, #-228]
	vldr	d17, [fp, #-220]
	vmov.u16	r3, d16[3]
	uxth	r3, r3
	add	r5, r5, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-212]
	vstr	d17, [fp, #-204]
	vldr	d16, [fp, #-212]
	vldr	d17, [fp, #-204]
	vmov.u16	r3, d17[0]
	uxth	r3, r3
	add	r4, r4, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-196]
	vstr	d17, [fp, #-188]
	vldr	d16, [fp, #-196]
	vldr	d17, [fp, #-188]
	vmov.u16	r3, d17[1]
	uxth	r3, r3
	add	r5, r5, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-180]
	vstr	d17, [fp, #-172]
	vldr	d16, [fp, #-180]
	vldr	d17, [fp, #-172]
	vmov.u16	r3, d17[2]
	uxth	r3, r3
	add	r4, r4, r3
	vldr	d16, [fp, #-132]
	vldr	d17, [fp, #-124]
	vstr	d16, [fp, #-164]
	vstr	d17, [fp, #-156]
	vldr	d16, [fp, #-164]
	vldr	d17, [fp, #-156]
	vmov.u16	r3, d17[3]
	uxth	r3, r3
	add	r5, r5, r3
	add	r4, r4, r5
	ldr	r3, [fp, #-44]
	add	r3, r3, #1
	str	r3, [fp, #-44]
.L20:
	ldr	r3, [fp, #-44]
	cmp	r3, #15
	ble	.L33
	cmp	r6, r4
	bls	.L34
	mov	r6, r4
	ldr	r3, [fp, #-40]
	uxtb	r2, r3
	ldr	r3, [fp, #-32]
	uxtb	r3, r3
	sub	r3, r2, r3
	strb	r3, [fp, #-21]
	ldr	r3, [fp, #-28]
	uxtb	r2, r3
	ldr	r3, [fp, #-36]
	uxtb	r3, r3
	sub	r3, r2, r3
	strb	r3, [fp, #-22]
.L34:
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
.L19:
	ldr	r3, [fp, #-32]
	uxtb	r3, r3
	add	r3, r3, #3
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_max
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-40]
	cmp	r3, r2
	blt	.L35
	ldr	r3, [fp, #-36]
	add	r3, r3, #1
	str	r3, [fp, #-36]
.L18:
	ldr	r3, [fp, #-28]
	uxtb	r3, r3
	add	r3, r3, #3
	uxtb	r3, r3
	sxtb	r3, r3
	mov	r0, r3
	bl	comp_max
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-36]
	cmp	r3, r2
	blt	.L36
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r1, r3
	ldr	r3, [fp, #-28]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-32]
	add	r3, r2, r3
	lsl	r3, r3, #2
	add	r3, r1, r3
	str	r6, [r3, #-1360]
	ldrsb	r1, [fp, #-21]
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r0, r3
	ldr	r3, [fp, #-28]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-32]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r0, r3
	mov	r2, r1
	strb	r2, [r3, #-1872]
	ldrsb	r1, [fp, #-22]
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r0, r3
	ldr	r3, [fp, #-28]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-32]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r0, r3
	mov	r2, r1
	strb	r2, [r3, #-1871]
	mvn	r6, #0
	ldr	r3, [fp, #-32]
	add	r3, r3, #1
	str	r3, [fp, #-32]
.L17:
	ldr	r3, [fp, #-32]
	cmp	r3, #15
	ble	.L37
	ldr	r3, [fp, #-28]
	add	r3, r3, #1
	str	r3, [fp, #-28]
.L16:
	ldr	r3, [fp, #-28]
	cmp	r3, #15
	ble	.L38
	mov	r3, #0
	str	r3, [fp, #-48]
	b	.L39
.L42:
	mov	r3, #0
	str	r3, [fp, #-52]
	b	.L40
.L41:
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r1, r3
	ldr	r3, [fp, #-48]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-52]
	add	r3, r2, r3
	lsl	r3, r3, #2
	add	r3, r1, r3
	ldr	r3, [r3, #-1360]
	str	r3, [fp, #-136]
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r1, r3
	ldr	r3, [fp, #-48]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-52]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r1, r3
	sub	r3, r3, #1872
	ldrsb	r3, [r3]
	str	r3, [fp, #-140]
	sub	r3, fp, #131072
	sub	r3, r3, #20
	mov	r1, r3
	ldr	r3, [fp, #-48]
	lsl	r2, r3, #4
	ldr	r3, [fp, #-52]
	add	r3, r2, r3
	lsl	r3, r3, #1
	add	r3, r1, r3
	sub	r3, r3, #1856
	sub	r3, r3, #15
	ldrsb	r3, [r3]
	str	r3, [fp, #-144]
	ldr	r3, [fp, #-136]
	str	r3, [sp, #4]
	ldr	r3, [fp, #-144]
	str	r3, [sp]
	ldr	r3, [fp, #-140]
	ldr	r2, [fp, #-52]
	ldr	r1, [fp, #-48]
	movw	r0, #:lower16:.LC4
	movt	r0, #:upper16:.LC4
	bl	printf
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	str	r3, [fp, #-52]
.L40:
	ldr	r3, [fp, #-52]
	cmp	r3, #15
	ble	.L41
	ldr	r3, [fp, #-48]
	add	r3, r3, #1
	str	r3, [fp, #-48]
.L39:
	ldr	r3, [fp, #-48]
	cmp	r3, #15
	ble	.L42
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #16
	@ sp needed
	pop	{r4, r5, r6, fp, pc}
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180801 (Red Hat 8.2.1-2)"
	.section	.note.GNU-stack,"",%progbits
