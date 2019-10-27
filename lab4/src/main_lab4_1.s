
	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	arr: .byte 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x72, 0x7F, 0x7B, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47
.text

	.global main
	.equ RCC_AHB2ENR, 		0x4002104C

	.equ GPIOA_MODER, 		0x48000000
	.equ GPIOA_OTYPER, 		0x48000004
	.equ GPIOA_OSPEEDR, 	0x48000008
	.equ GPIOA_PUPDR, 		0x4800000C
	.equ GPIOA_ODR, 		0x48000014

	.equ GPIO_BSRR,  	    0x48000018
	.equ GPIO_BRR,			0x48000028

	.equ DECODE_MODE,		0x09
	.equ SHUTDOWN,			0x0C
	.equ INTENSITY,			0x0A
	.equ SCAN_LIMIT, 		0x0B
	.equ DISPLAY_TEST, 		0x0F

	.equ DIN, 	0x20 //PA5
	.equ CS, 	0x40 //PA6
	.equ CLK, 	0x80 //PA7


main:
	BL GPIO_init
	BL MAX7219_init
	B Display0toF

GPIO_init:
	movs 	r0, #0x1
	ldr		r1, =RCC_AHB2ENR
	str		r0, [r1]

	movs	r0, #0x5400
	ldr  	r1, =GPIOA_MODER
	ldr		r2, [r1]
	and		r2, #0xFFFF03FF
	orrs	r2, r2,r0
	str		r2, [r1]

	movs	r0, #0xA800
	ldr		r1, =GPIOA_OSPEEDR
	str  	r0, [r1]

	ldr		r1, =GPIOA_ODR

	BX LR

Display0toF:
	//TODO: Display 0 to F at first digit on 7-SEG LED. Display one
	mov		r0, 0x1
	ldr		r2, =arr
inner:
	ldrb	r1, [r2], #1
	bl		MAX7219Send
	ldr		r3, =2500000
	b		Delay
Delay_0:
	cmp		r1, 0x47
	bne		inner
	b		Display0toF
MAX7219Send:
	//input parameter: r0 is ADDRESS , r1 is DATA

	push	{r0,r1,r2,r3,r4,r5,r6,r7,r8,lr}
	lsl		r0, r0, #8
	add    	r0, r1

	ldr  	r2, =CS
	ldr  	r3, =DIN
	ldr  	r4, =CLK
	ldr	  	r5, =GPIO_BSRR
	ldr  	r6, =GPIO_BRR

	mov 	r8, #1
	lsl		r8, r8, #16
MAX7219_loop:
	lsr		r8, r8, #1
	cmp		r8, #0
	beq		MAX_end
	tst     r0, r8
	beq		bit_not_set
	str		r3, [r5]
	str		r4, [r6]
	str		r4, [r5]
	b		MAX7219_loop

bit_not_set:
	str		r3, [r6]
	str		r4, [r6]
	str		r4, [r5]
	b		MAX7219_loop

MAX_end:
	str		r2, [r6]
	str		r2, [r5]
	pop  	{r0,r1,r2,r3,r4,r5,r6,r7,r8,pc}
	BX LR

MAX7219_init:
	//TODO: Initialize max7219 registers

	push	{r0,r1,r2,lr}

	ldr 	r0, =DECODE_MODE
	ldr		r1, =0x0
	bl		MAX7219Send

	ldr 	r0, =INTENSITY
	ldr		r1, =0x7
	bl		MAX7219Send

	ldr 	r0, =SHUTDOWN
	ldr		r1, =0x1
	bl		MAX7219Send

	ldr 	r0, =SCAN_LIMIT
	ldr		r1, =0x0
	bl		MAX7219Send

	ldr 	r0, =DISPLAY_TEST
	ldr		r1, =0x0
	bl		MAX7219Send

	pop		{r0,r1,r2,pc}
	BX LR

Delay:
	subs 	r3, #4
	beq		Delay_0
	b		Delay

