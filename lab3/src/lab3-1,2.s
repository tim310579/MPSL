
	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	leds: .byte 0
	direction: .byte 0
	.align
.text
	.global main
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIOB_MODER, 0x48000400
	.equ GPIOB_OTYPER, 0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 0x4800040C
	.equ GPIOB_ODR, 0x48000414
	.equ GPIOB_BSRR, 0x48000418

	.equ GPIOC_MODER, 0x48000800
	.equ GPIOC_OTYPER, 0x48000804
	.equ GPIOC_OSPEEDR, 0x48000808
	.equ GPIOC_PUPDR, 0x4800080C
	.equ GPIOC_IDR, 0x48000810
	.equ GPIOC_BSRR, 0x48000818

main:
	BL		GPIO_init
	MOVS	R1, #12
	LDR		R0, =leds
	STRB	R1, [R0]

Loop:
	//TODO: Write the display pattern into leds variable
	BL		DisplayLED
	BL		Delay
	B		Loop

GPIO_init:
	//TODO: Initial LED GPIO pins as output
	//Enable AHB2 clock for GPIOB
	movs	r0, #0x6
	ldr		r1, =RCC_AHB2ENR
	str		r0, [r1]

	//Set PB3456 as output mode
	movs	r0, #0x1540
	ldr		r1, =GPIOB_MODER
	ldr		r2, [r1]
	and 	r2, #0xFFFFC03F //Mask MODERS
	orrs	r2, r2, r0
	str		r2, [r1]

	//Set PB3456 as Pull-up output
	movs	r0, #0x1540
	ldr		r1, =GPIOB_PUPDR
	ldr		r2, [r1]
	and 	r2, #0xFFFFC03F //Mask
	orrs	r2, r2, r0
	str		r2, [r1]

	//Set PB3456 as high speed mode
	movs	r0, #0x2A80
	ldr		r1, =GPIOB_OSPEEDR
	strh	r0, [r1]

	//set pc13 as input mode
	ldr		r0, =GPIOC_MODER
	ldr		r1, [r0]
	and 	r1, #0xF3FFFFFF //Mask MODERS
	str		r1, [r0]

	//set pc13 as pull-up
	ldr		r0, =GPIOC_PUPDR
	ldr		r1, [r0]
	and		r1, #0xF3FFFFFF
	orrs	r1, #0x4000000
	str		r1, [r0]

	BX LR

DisplayLED:
	//TODO: Display LED by leds
	ldr		r1, =leds
	ldrb	r0, [r1]

	mvn		r2, r0
	ldr		r1, =GPIOB_ODR
	strh	r2, [r1]

	ldr		r2, =direction
	ldr		r3, [r2]
	cmp		r3, #0
	beq		shift_l
	cmp		r3, #1
	beq		shift_r

shift_l:
	lsl		r0, r0, #1
	ldr		r1, =leds
	strb    r0, [r1]

	cmp		r0, #192
	beq		change_1
	bx		lr
shift_r:
	lsr		r0, r0, #1
	ldr		r1, =leds
	strb    r0, [r1]

	cmp		r0, #12
	beq		change_0
	bx		lr
change_1:
	mov		r3, #1
	str		r3, [r2]
	bx lr
change_0:
	mov		r3, #0
	str		r3, [r2]
	bx lr
Delay:
	//TODO: Write a delay 1 sec function
	ldr		r4, =#10
L3:
	ldr		r5, =#100000
Delay0:
	ldr		r0, =GPIOC_IDR
	ldrh	r1, [r0]
	and		r1, #0x2000
	cmp		r1, #0
	beq		stop
	subs	r5, #1
	bne		Delay0
resume:
	ldr		r3, =#40000
Delay2:
	subs	r3, #1
	bne		Delay2
	subs	r4, #1
	bne		L3

	BX		LR

stop:
	ldr		r3, =#40000
Delay1:
	subs	r3, #1
	bne		Delay1
stop0:
	ldr		r0, =GPIOC_IDR
	ldrh	r1, [r0]
	and		r1, #0x2000
	cmp		r1, #0
	beq		resume
	b		stop0


