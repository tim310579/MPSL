.syntax unified
	.cpu cortex-m4
	.thumb
.data
	ans: .asciz "01123581321345589144233377610987159725844181676510946177112865746368750251213931964183178115142298320401346269217830935245785702887922746514930352241578173908816963245986:1"
	len: .byte 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x4, 0x5, 0x5, 0x5, 0x5, 0x5, 0x6, 0x6, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x8, 0x8, 0x8, 0x8, 0x2

.text
	.global main
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

	.equ GPIOC_MODER, 0x48000800
	.equ GPIOA_BSRR, 0x48000018 // set bit
	.equ GPIOA_BRR, 0x48000028 // clear bit

	.equ GPIOC_IDR, 0x48000810	//user button

main:
	mov r9, 0x0	//long press
	mov r11, 0x1
	mov r12, 0x1
	bl GPIO_init
	bl MAX_init

GPIO_init:
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

	ldr r1, =GPIOC_MODER
	ldr r2, [r1]
	and r2, 0xF3FFFFFF
	str r2, [r1]

	ldr r10, =GPIOC_IDR	//button
	bx lr


MAX_send:
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

MAX_send_loop:
	mov r8, 0x1
	lsl r8, r8, r7
	str r4, [r6] //clk -> 0
	tst r0, r8 // ands but discard result
	beq MAX_send_clear
	str r3, [r5] // din -> 1
	b MAX_send_check

MAX_send_clear:
	str r3, [r6] // din -> 0

MAX_send_check:
	str r4, [r5] // clk -> 1
	subs r7, 0x1
	bge MAX_send_loop
	str r2, [r6] // cs -> 0
	str r2, [r5] // cs -> 1
	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, pc}

MAX_init:
	push {r0, r1, r2, lr}

	ldr r0, =#DECODE_MODE
	ldr r1, =#0xFF
	bl MAX_send

	ldr r0, =#DISPLAY_TEST
	ldr r1, =#0x0
	bl MAX_send

	ldr r0, =#SCAN_LIMIT
	ldr r1, =0x7
	bl MAX_send

	ldr r0, =#INTENSITY
	ldr r1, =#0xA
	bl MAX_send

	ldr r0, =#SHUT_DOWN
	ldr r1, =#0x1
	bl MAX_send

	pop {r0, r1, r2, r7}

	//b button_init
display_ans:
	ldr r2, =len
	ldr r3, =ans
	b loop_init

loop_init:	//init 0-7
	mov r0, 0x1
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x2
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x3
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x4
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x5
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x6
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x7
	mov r1, 0x0F
	bl MAX_send

	mov r0, 0x8
	mov r1, 0x0F
	bl MAX_send

	ldrb r0, [r2]

loop_inner:
	ldrb r1, [r3]
	sub r1, 0x30
	bl MAX_send

	subs r0, 0x1
	add r3, 0x1
	bne loop_inner

loop:
	ldr r0, =len
	add r0, 40
	cmp r2, r0
	it eq
	bleq loop_last	//if r0==r2->b loop_last
	add r2, 0x1

	mov r9, 0x0
	mov r11, 0x1
	mov r12, 0x1
	bl button_init

	b loop_init

loop_last:
	sub  r2, 0x1
	sub  r3, 0x2
	bx   lr
button_init:
	ldr r0, =4000000 //set delay
	movs r0, r0
	b button_delay

button_delay:
	beq button_init	//re init
	ldr r1, =0b11111111111111111
	ands r1, r0
	beq button_check //30ms->branch
	subs r0, 0x8
	b button_delay

button_check:
	ldrh r1, [r10]
	lsr r1, 13
	mov  r4, 1
	ands r1, r4
	beq  button_plus

	cmp  r9, 31
	bge  main
	mov  r9, 0
	cmp  r1, r11
	mov  r11, r1
	beq button_confirm
	subs r0, 8
	b    button_delay

loop_debounce:


button_plus:

	add r9, 1
	cmp r1, r11
	mov r11, r1
	beq button_confirm
	subs r0, 8
	b button_delay
button_confirm:
	subs r1, r11, r12
	cmp  r1, 1
	mov  r12, r11
	beq  button_end

	subs r0, 8
	b    button_delay

button_end:
	bx   lr
