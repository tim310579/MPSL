	.syntax unified
	.cpu cortex-m4
	.thumb

.text

	.global GPIO_init
	.global max7219_init
	.global max7219_send
	.global Delay1sWith4MHz

	.equ RCC_AHB2ENR, 0x4002104C

	.equ DECODE_MODE, 0x09
	.equ INTENSITY, 0x0A
	.equ SCAN_LIMIT, 0x0B
	.equ SHUT_DOWN, 0x0C
	.equ DISPLAY_TEST, 0x0F

	.equ DATA, 0x20 //PA5
	.equ LOAD, 0x40 //PA6
	.equ CLOCK, 0x80 //PA7

	.equ GPIOA_MODER, 0x48000000
	.equ GPIOA_OTYPER, 0x48000004
	.equ GPIOA_SPEEDER, 0x48000008

	.equ GPIOB_MODER, 0x48000400
	.equ GPIOC_MODER, 0x48000800
	.equ GPIOA_BSRR, 0x48000018 // set bit
	.equ GPIOA_BRR, 0x48000028 // clear bit

	.equ GPIOC_IDR, 0x48000810	//user button

GPIO_init:
	push {r0, r1, r2, lr}

	movs r0, #5
	ldr r1, =RCC_AHB2ENR
	str r0, [r1]

	ldr r1, =GPIOA_MODER
	ldr r2, [r1]
	and r2, 0xFFFF03FF	//set PA5,6,7
	orr r2, 0x5400
	str r2, [r1]

	ldr r1, =GPIOA_OTYPER
	ldr r2, [r1]
	and r2, 0xFFFFFF1F
	str r2, [r1]

	ldr r1, =GPIOA_SPEEDER
	ldr r2, [r1]
	and r2, 0xFFFF03FF
	orr r2, 0x5400
	str r2, [r1]

	ldr  r1, =GPIOB_MODER @ GPIOB_MODER
	ldr  r2, [r1]
	and  r2, 0b11111111111111111111111100111111
	orr  r2, 0b00000000000000000000000010000000 @ alternate function mode for PB3
	str  r2, [r1]

	ldr r1, =GPIOC_MODER
	ldr r2, [r1]
	and r2, 0xF3FFFFFF
	str r2, [r1]

	pop  {r0, r1, r2, pc}
max7219_init:
	push {r0, r1, lr}

	ldr r0, =#DECODE_MODE
	ldr r1, =#0xFF
	bl max7219_send

	ldr r0, =#DISPLAY_TEST
	ldr r1, =#0x0
	bl max7219_send

	ldr r0, =#SCAN_LIMIT
	ldr r1, =0x7
	bl max7219_send

	ldr r0, =#INTENSITY
	ldr r1, =#0xA
	bl max7219_send

	ldr r0, =#SHUT_DOWN
	ldr r1, =#0x1
	bl max7219_send

	pop {r0, r1, pc}
max7219_send:
	push {r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}	//r0->address
	lsl r0, 0x8
	add r0, r1
	ldr r1, =GPIOA_MODER
	ldr r2, =LOAD
	ldr r3, =DATA
	ldr r4, =CLOCK
	ldr r5, =GPIOA_BSRR
	ldr r6, =GPIOA_BRR
	ldr r7, =0x0F

max7219_send_loop:
	mov r8, 0x1
	lsl r8, r8, r7
	str r4, [r6] //clk -> 0
	tst r0, r8 // ands but discard result
	beq max7219_send_clear
	str r3, [r5] // din -> 1
	b max7219_send_check

max7219_send_clear:
	str r3, [r6] // din -> 0

max7219_send_check:
	str r4, [r5] // clk -> 1
	subs r7, 0x1
	bge max7219_send_loop
	str r2, [r6] // cs -> 0
	str r2, [r5] // cs -> 1
	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, pc}
Delay1sWith4MHz:
	push {r0, lr}
	ldr r0, =1000000
	movs r0, r0
Delay1sWith4MHz_loop:
	beq delay_end
	subs r0, r0, 1
	b Delay1sWith4MHz_loop
delay_end:
	pop {r0, pc}
