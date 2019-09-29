.syntax unified
	.cpu cortex-m4
	.thumb

.data
	arr1: .byte 0x19, 0x34, 0x14, 0x32, 0x52, 0x23, 0x61, 0x29
	arr2: .byte 0x18, 0x17, 0x33, 0x16, 0xFA, 0x20, 0x55, 0xAC
.text
	.global main3
do_sort:
	//TODO out loop from here
	mov r3, #0			//r3 and r3+1 means the position to compare
	bl inner_loop
	sub r5, r5, #1		//r5 means the times to do swap
	cmp r5, #0
	bgt do_sort
	b main3_2
inner_loop:
	cmp r3, r5
	bne compare
	bx lr
compare:
	ldrb r1, [r0, r3]
	add r3, r3, #1
	ldrb r2, [r0, r3]
	cmp r1, r2
	bgt swap
	b inner_loop

swap:
	add r4, r3, #-1
	strb r1, [r0, r3]	//swap their position
	strb r2, [r0, r4]
	b inner_loop
main3:

	mov r5, #7
	ldr r0, =arr1
	bl do_sort
main3_2:

	mov r5, #7
	ldr r0, =arr2

	bl do_sort
L3:
	ldrb r8, [r0, #8]
	ldrb r9, [r0, #9]
	b L3
