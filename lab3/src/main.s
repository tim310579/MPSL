	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	password: .byte 0b1011
	Press: .byte 0
	.align

.text
	.global main
	.equ RCC_AHB2ENR  , 0x4002104C
	.equ GPIOB_MODER  , 0x48000400
	.equ GPIOB_OTYPER , 0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR  , 0x4800040C
	.equ GPIOB_ODR    , 0x48000414

	.equ GPIOC_MODER  , 0x48000800
	.equ GPIOC_PUPDR  , 0x4800080C
	.equ GPIOC_IDR    , 0x48000810

main:
	b    GPIO_init

GPIO_init:
	mov  r0, 0b110
	ldr  r1, =RCC_AHB2ENR
	str  r0, [r1]

	mov  r0, 0b01010101000000	//MODER3,4,5,6->01->output mode
	ldr  r1, =GPIOB_MODER
	ldr  r2, [r1]
	and  r2, 0xFFFFC03F
	orr  r2, r0
	str  r2, [r1]
	//movs r7, r2

	ldr  r1, =GPIOC_MODER
	ldr  r0, [r1]
	ldr  r2, =0xF3FFFF00
	and  r0, r2
	str  r0, [r1]

	mov  r0, 0b10101010000000	//OSPEEDER3,4,5,6 ->10 ->fast speed
	ldr  r1, =GPIOB_OSPEEDR
	str  r0, [r1]

	ldr  r1, =GPIOC_PUPDR
	ldr  r0, [r1]	//r0 = 0x0000 0000(reset)
	ldr  r2, =0b01010101

	and  r0, 0xFFFFFF00
	orr  r0, r2
	str  r0, [r1]

	ldr  r10, =GPIOB_ODR // leds
	ldr  r11, =GPIOC_IDR // user button
	ldr  r12, =GPIOC_IDR // dip switch

	mov  r0, 0b11111111
	strh r0, [r10]
	b    button_init

button_init:
	movs r0, 0
	b    button

button:
	beq  button_restart
	ldr  r1, =0b1111111111111111
	ands r1, r0, r1
	cmp  r1, #0
	beq  button_check	//r1 and r0->0 --> button check when r1 -> 0x0000 ->check
	subs r0, 8													//(0x10000/8)->period
	b    button

button_check:
	ldrh r1, [r11]	//if button not pressed, r1->0x2005, if pressed, r1->0x5
	//movs r5, r1	//
	lsr  r1, 13		//if button not pressed, r1 shift will be 0b1, else -> 0b0
	cmp  r1, #0
	beq	 read_switch
	subs r0, 8
	b    button

button_restart:
	ldr  r0, =4000000	//cycle to wait press button
	movs r0, r0		//update r0 state
	b    button

read_switch:
	ldrh r1, [r12]

	and  r1, 0b1111
	eor  r1, 0b1111

	ldr  r0, =password
	ldr  r0, [r0]
	cmp  r0, r1
	mov  r1, 0b11111111
	mov  r2, 0b0
	beq  leds_3		//r0 == r1 ->correct
	b    leds_1

leds_3:
	eor  r1, 0b11111111
	strh r1, [r10]		//r1 == 0->light, 0b1111 1111->dark
	add  r2, 0b1
	cmp  r2, 6
	beq  button_init
	ldr  r0, =1500000	//clock
	movs r0, r0
	b    leds_3_blink

leds_3_blink:
	beq  leds_3	//r0 == 0,->light->dark or d->l
	subs r0, 4
	b    leds_3_blink

leds_1:
	eor  r1, 0b11111111		//like above
	strh r1, [r10]
	add  r2, 0b1

	cmp  r2, 2		//sprinkle one times
	beq  button_init

	ldr  r0, =2000000
	movs r0, r0
	b    leds_1_blink

leds_1_blink:
	beq  leds_1
	subs r0, 4
	b    leds_1_blink
