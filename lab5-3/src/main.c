#include "stm32L476xx.h"
#include "utils.h"



#define X0 GPIO_Pin_0	//PC 0,1,2,3 output
#define X1 GPIO_Pin_1
#define X2 GPIO_Pin_2
#define X3 GPIO_Pin_3	//0b  1000 0000 0000
#define Y0 GPIO_Pin_6	//Set PB6,5,4,3 as INPUT mode
#define Y1 GPIO_Pin_5
#define Y2 GPIO_Pin_4
#define Y3 GPIO_Pin_3

//unsigned int x_pin[4] = {X0, X1, X2, X3};
//unsigned int y_pin = {Y0, Y1, Y2, Y3};

// initial keypad gpio pin, X as output and Y as input

void keypad_init()
{

	RCC->AHB2ENR = RCC->AHB2ENR|0x6;
	GPIOC->MODER   &= 0b11111111111111111111111100000000;
	GPIOC->MODER   |= 0b00000000000000000000000001010101;
	GPIOC->PUPDR   &= 0b11111111111111111111111100000000;
	GPIOC->PUPDR   |= 0b00000000000000000000000001010101;
	GPIOC->OSPEEDR &= 0b11111111111111111111111100000000;
	GPIOC->OSPEEDR |= 0b00000000000000000000000001010101;
	GPIOC->ODR     |= 0b00000000000000000000000000001111;

	GPIOB->MODER   &= 0b11111111111111111100000000111111;
	GPIOB->PUPDR   &= 0b11111111111111111100000000111111;
	GPIOB->PUPDR   |= 0b00000000000000000010101010000000;
	GPIOB->OSPEEDR &= 0b11111111111111111100000000111111;
	GPIOB->OSPEEDR |= 0b00000000000000000001010101000000;
}

/**
 * scan keypad value
 *
 * return:
 *   >=0: key pressed value
 *   -1: no key press
 */

signed char keypad_scan()
{
     int ret = -1;
		//GPIOC->BSRR = X0;
		//GPIOC->BRR = X1;
		//GPIOC->BRR = X2;
		//GPIOC->BRR = X3;
		GPIOC->ODR = X0;
		if (GPIOB->IDR & Y0)	ret = 1;
		if (GPIOB->IDR & Y1)	ret = 4;
		if (GPIOB->IDR & Y2)	ret = 7;
		if (GPIOB->IDR & Y3)	ret = 15;/*
		if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 1;
		if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 4;
		if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 7;
		if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 15;*/

		//GPIOC->BRR = X0;
		//GPIOC->BSRR = X1;
		//GPIOC->BRR = X2;
		//GPIOC->BRR = X3;
		GPIOC->ODR = X1;
		if (GPIOB->IDR & Y0)	ret = 2;
		if (GPIOB->IDR & Y1)	ret = 5;
		if (GPIOB->IDR & Y2)	ret = 8;
		if (GPIOB->IDR & Y3)	ret = 0;/*
		if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 2;
		if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 5;
		if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 8;
		if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 0;*/

		//GPIOC->BRR = X0;
		//GPIOC->BRR = X1;
		//GPIOC->BSRR = X2;
		//GPIOC->BRR = X3;
		GPIOC->ODR = X2;
		if (GPIOB->IDR & Y0)	ret = 3;
		if (GPIOB->IDR & Y1)	ret = 6;
		if (GPIOB->IDR & Y2)	ret = 9;
		if (GPIOB->IDR & Y3)	ret = 14;/*
		if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 3;
		if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 6;
		if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 9;
		if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 14;*/

		//GPIOC->BRR = X0;
		//GPIOC->BRR = X1;
		//GPIOC->BRR = X2;
	//	GPIOC->BSRR = X3;
		GPIOC->ODR = X3;
		if (GPIOB->IDR & Y0)	ret = 10;
		if (GPIOB->IDR & Y1)	ret = 11;
		if (GPIOB->IDR & Y2)	ret = 12;
		if (GPIOB->IDR & Y3)	ret = 13;/*
		if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 10;
		if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 11;
		if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 12;
		if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 13;*/

		return ret;
}
signed char keypad_scan_back(){

    int ret = -1;
	//GPIOC->BRR = X0;
	//GPIOC->BRR = X1;
	//GPIOC->BRR = X2;
	//GPIOC->BSRR = X3;
	GPIOC->ODR = X3;
	if (GPIOB->IDR & Y3)	ret = 13;
	if (GPIOB->IDR & Y2)	ret = 12;
	if (GPIOB->IDR & Y1)	ret = 11;
	if (GPIOB->IDR & Y0)	ret = 10;/*
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 13;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 12;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 11;
	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 10;*/

	//GPIOC->BRR = X0;
//	GPIOC->BRR = X1;
	//GPIOC->BSRR = X2;
	//GPIOC->BRR = X3;
	GPIOC->ODR = X2;
	if (GPIOB->IDR & Y3)	ret = 14;
	if (GPIOB->IDR & Y2)	ret = 9;
	if (GPIOB->IDR & Y1)	ret = 6;
	if (GPIOB->IDR & Y0)	ret = 3;/*
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 14;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 9;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 6;
	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 3;*/

	//GPIOC->BRR = X0;
	//GPIOC->BSRR = X1;
	//GPIOC->BRR = X2;
	//GPIOC->BRR = X3;
	GPIOC->ODR = X1;
	if (GPIOB->IDR & Y3)	ret = 0;
	if (GPIOB->IDR & Y2)	ret = 8;
	if (GPIOB->IDR & Y1)	ret = 5;
	if (GPIOB->IDR & Y0)	ret = 2;/*
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 0;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 8;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 5;
	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 2;*/

	//GPIOC->BSRR = X0;
	//GPIOC->BRR = X1;
	//GPIOC->BRR = X2;
	//GPIOC->BRR = X3;
	GPIOC->ODR = X0;
	if (GPIOB->IDR & Y3)	ret = 15;
	if (GPIOB->IDR & Y2)	ret = 7;
	if (GPIOB->IDR & Y1)	ret = 4;
	if (GPIOB->IDR & Y0)	ret = 1;/*
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 15;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 7;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 4;
	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 1;*/

	return ret;
}

int main()
{
	GPIO_init();
	max7219_init();
	keypad_init();
	//int choose[14];
	int input = -1;
	int tmp_input = -1;
	int sum = 0;

	//clear_to_0(choose);

		while(1){
			//int cnt = 0;

			//sum = -1;
			//int input2 = input;	//previous
			//clear_to_0(choose);

			input = keypad_scan();
			tmp_input = keypad_scan_back();
			sum = -1;
			if(input == tmp_input && input != -1)	{
				//choose[input] = 1;
				//cnt = 1;
				sum = input;

			}

			if(input != tmp_input && input != -1 && tmp_input != -1) {
				//choose[input] = 1;
				//choose[tmp_input] = 1;
				//cnt = 2;
				sum = input + tmp_input;
			}


			int dig = 0;
			if(sum > 9 && sum < 100) dig = 2;
			else if(sum >= 0) dig = 1;

			if(dig > 0)	display(sum, dig);
			else	display(sum, 0);
		}
}
