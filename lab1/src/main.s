.syntax unified
	.cpu cortex-m4
	.thumb

.data
	result: .byte 0
.text
	.global main
.equ X, 0x55BB
.equ Y, 0xAA55
hamm:
	//TODO
	mov R7, #0
	EOR R3, R0, R1
	b count
count:
	cmp R3, #0
	beq L
	AND R5, R3, #1
	LSR R3, R3, 1
	cmp R5, #1
	beq countadd
	b count
countadd:
	add R7, #1
	b count
main:
	//b main2
	//b main3
	ldr R0, =#X //This code will cause assemble error. Why? And how
	ldr R1, =#Y
	ldr R2, =result

	bl hamm
L:
	str R7, [R2]

	b L
	//b main2
