#ifndef UTILS_H_
#define UTILS_H_
#include "stm32l476xx.h"
// these functions are inside the asm file
extern void GPIO_init();
extern void max7219_init();
extern void max7219_send(unsigned char address, unsigned char data);
extern void Delay1sWith4MHz();
extern void fpu_enable();

/**
 * show data on 7-seg via max7219_send
 *
 * input:
 *   data: decimal value
 *   num_digs: number of digits to show on 7-seg
 *
 * return:
 *   0: success
 *   -1: illegal data range (out of 8 digits)
 */
int display(int data, int num_digs){
	int i;
	if(data == -1){
		for(i = 1; i <= 8; i ++){
				max7219_send(i, 15);
			}
		return 0;
	}

	for(i = num_digs+1; i <= 8; i ++){
		max7219_send(i, 15);
	}
	if(num_digs == 0){

		for(i = 1; i <= 8; i++){
			max7219_send(i, 15);
		}
		return 0;
	}

		int neg = 0;
		int over = 0;
		if(data < 0){
			neg = 1;
		}
		if(data > 99999999 || data < -9999999){
			over = 1;
		}
		for (i = 1; i <= num_digs; i++){
			if (data % 10 < 0){
				max7219_send(i, -data % 10);
			}
			else{
				max7219_send(i, data % 10);
			}
			data /= 10;
		}
		if(neg == 1){
			max7219_send(num_digs, 10);	//means '-'
		}
		if(over == 1){
			for(i = 3; i <= 8; i++){
				max7219_send(i, 15);
			}
			max7219_send(2, 10);
			max7219_send(1, 1);
			return -1;
		}
	return 0;
}
#define X0 GPIO_Pin_0	//pc0,1,2,3 output
#define X1 GPIO_Pin_1
#define X2 GPIO_Pin_2
#define X3 GPIO_Pin_3
#define Y0 GPIO_Pin_6	//pb4,5,6,7
#define Y1 GPIO_Pin_5
#define Y2 GPIO_Pin_4
#define Y3 GPIO_Pin_7

#define GPIO_Pin_0  0b0000000000000001
#define GPIO_Pin_1  0b0000000000000010
#define GPIO_Pin_2  0b0000000000000100
#define GPIO_Pin_3  0b0000000000001000
#define GPIO_Pin_4  0b0000000000010000
#define GPIO_Pin_5  0b0000000000100000
#define GPIO_Pin_6  0b0000000001000000
#define GPIO_Pin_7  0b0000000010000000
#define GPIO_Pin_8  0b0000000100000000
#define GPIO_Pin_9  0b0000001000000000
#define GPIO_Pin_10 0b0000010000000000
#define GPIO_Pin_11 0b0000100000000000
#define GPIO_Pin_12 0b0001000000000000
#define GPIO_Pin_13 0b0010000000000000
#define GPIO_Pin_14 0b0100000000000000
#define GPIO_Pin_15 0b1000000000000000
/**
 * initialize keypad GPIO pin, X as output and Y as input
 */
void keypad_init()
{
	RCC->AHB2ENR   |= 0b00000000000000000000000000000110;

	GPIOC->MODER   &= 0b11111111111111111111111100000000;
	GPIOC->MODER   |= 0b00000000000000000000000001010101;
	GPIOC->PUPDR   &= 0b11111111111111111111111100000000;
	GPIOC->PUPDR   |= 0b00000000000000000000000001010101;
	GPIOC->OSPEEDR &= 0b11111111111111111111111100000000;
	GPIOC->OSPEEDR |= 0b00000000000000000000000001010101;
	GPIOC->ODR     |= 0b00000000000000000000000000001111;

	GPIOB->MODER   &= 0b11111111111111110000000011111111;
	GPIOB->PUPDR   &= 0b11111111111111110000000011111111;
	GPIOB->PUPDR   |= 0b00000000000000001010101000000000;
	GPIOB->OSPEEDR &= 0b11111111111111110000000011111111;
	GPIOB->OSPEEDR |= 0b00000000000000000101010100000000;
}

/**
 * scan keypad value
 *
 * return:
 *   >=0: key press value
 *   -1: no key press
 */
signed char keypad_scan()
{
	GPIOC->ODR = X0;
			if (GPIOB->IDR & Y0)	return 1;
			if (GPIOB->IDR & Y1)	return 4;
			if (GPIOB->IDR & Y2)	return 7;
			if (GPIOB->IDR & Y3)	return 15;/*
			if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 1;
			if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 4;
			if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 7;
			if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 15;*/

			//GPIOC->BRR = X0;
			//GPIOC->BSRR = X1;
			//GPIOC->BRR = X2;
			//GPIOC->BRR = X3;
			GPIOC->ODR = X1;
			if (GPIOB->IDR & Y0)	return 2;
			if (GPIOB->IDR & Y1)	return 5;
			if (GPIOB->IDR & Y2)	return 8;
			if (GPIOB->IDR & Y3)	return 0;/*
			if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 2;
			if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 5;
			if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 8;
			if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 0;*/

			//GPIOC->BRR = X0;
			//GPIOC->BRR = X1;
			//GPIOC->BSRR = X2;
			//GPIOC->BRR = X3;
			GPIOC->ODR = X2;
			if (GPIOB->IDR & Y0)	return 3;
			if (GPIOB->IDR & Y1)	return 6;
			if (GPIOB->IDR & Y2)	return 9;
			if (GPIOB->IDR & Y3)	return 14;/*
			if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 3;
			if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 6;
			if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 9;
			if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 14;*/

			//GPIOC->BRR = X0;
			//GPIOC->BRR = X1;
			//GPIOC->BRR = X2;
		//	GPIOC->BSRR = X3;
			GPIOC->ODR = X3;
			if (GPIOB->IDR & Y0)	return 10;
			if (GPIOB->IDR & Y1)	return 11;
			if (GPIOB->IDR & Y2)	return 12;
			if (GPIOB->IDR & Y3)	return 13;/*
			if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 10;
			if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 11;
			if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 12;
			if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 13;*/

			return -1;
}


int GPIO_ReadInputDataBit(GPIO_TypeDef *a, uint16_t b) {
	return a->IDR & b;
}

int displayf(float data, int num_digs)
{
	if (num_digs > 8)
		return display(-1, 2);
	if ((int) (data * 100) % 100)
		return display(data * 100, -1002 - num_digs);
	else
		return display(data, num_digs);
}

#endif /* UTILS_H_ */
