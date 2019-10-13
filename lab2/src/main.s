.syntax unified
	.cpu cortex-m4
	.thumb

.data
	result: .word 0
	max_size: .word 0
.text
		.global main
	m: .word 0x5E
	n: .word 0x60
	//m: .word 36
	//n: .word 24
GCD:
	//TODO: Implement your GCD function
	ldr r0, [sp]	//load stack (a,b) to r0, r1
	ldr r1, [sp, #4]

	cmp r0, #0
	beq retb	//a == 0 -> return b
	cmp r1, #0
	beq reta		//b == 0, ret a
	movs r4, #1
	movs r5, #1
	and r4, r0, #1	//r4==1, ->a%2 != 0
	and r5, r1, #1	//r5==1, ->b%2 != 0
	orr r6, r4, r5	//r6==0, ->(a % 2 == 0 && b % 2 == 0)
	cmp r6, #0
	beq a_s_1_b_s_1		//means a >> 1, b >> 1
	cmp r4, #0		//a%2 == 0
	beq shift_a
	cmp r5, #0		//b%2 == 0
	beq shift_b

	B else		//else  return GCD(abs(a - b), min(a, b));
retb:
	movs r2, r1		//r2 is the answer of gcd(0, b)
	bx lr
reta:
	movs r2, r0
	bx lr
a_s_1_b_s_1:	//return 2 * GCD(a >> 1, b >> 1)
	asr r0, r0, 1
	asr r1, r1, 1
	push {r0, r1, lr}
	adds r3, r3, #12
	bl GCD
	movs r7, #2
	mul r2, r2, r7		//gcd*2
	pop {r0, r1, pc}	//the last pop to pc will goto main:line93

shift_a:
	asr r0, r0, 1	//a >> 1
	push {r0, r1, lr}
	adds r3, r3, #12
	bl GCD
	pop {r0, r1, pc}
shift_b:
	asr r1, r1, 1	//b >> 1
	push {r0, r1, lr}
	adds r3, r3, #12
	bl GCD
	pop {r0, r1, pc}
else:
	cmp r0, r1
	bgt abs_a_b
	cmp r0, r1
	ble abs_b_a
abs_a_b:	//a>b
	sub r0, r0, r1	//abs(a-b) = a-b
	movs r1, r1		//min(a,b) = b
	b else_ret
abs_b_a:
	sub r1, r1, r0	//abs(a-b) = b-a
	movs r0, r0		//min = a
	b else_ret
else_ret:
	push {r0, r1, lr}
	adds r3, r3, #12
	bl GCD
	pop {r0, r1, pc}
main:
// r0 = m, r1 = n
	ldr r0, =#m
	ldr r1, =#n
	ldr r0, [r0]
	ldr r1, [r1]
	movs r3, #0		//r3 -> stack size = 0
	//movs r11, lr
	push {r0, r1, lr}
	adds r3, r3, #12	//stack size+12
	BL GCD
	ldr r4, =result		//result address -> r4
	str r2, [r4]
	ldr r5, =max_size	//max_size address -> r5
	str r3, [r5]
	pop {r0, r1, r11}
	b fin
// get return val and store into result

fin:
	b fin
