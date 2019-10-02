.syntax unified
	.cpu cortex-m4
	.thumb

.text
.global main2
	.equ N, -87
	.equ OVER, 0x80000000
fib:
	//TODO
	mov r2, #1
	mov r3, #1
	b fact
fact:
	sub r1, r1, #1
	cmp r1, #1
	beq L2
	add r4, r2, r3
	mov r5, #OVER
	and r5, r5, r4
	cmp r5, #0
	bne N_2
	mov r2, r3
	mov r3, r4
	b fact
main2:
	movs R0, #N
	movs R1, R0
	cmp r0, #100
	bgt N_1
	cmp r0, #0
	ble N_1
	bl fib
L2:
	b finish
N_1:
	mov r4, #-1		//-1 means N > 100 or N <= 0
	b finish
N_2:
	mov r4, #-2
finish:
	b finish
	//b main3
