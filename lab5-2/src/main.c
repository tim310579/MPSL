#include "stm32L476xx.h"
#include "utils.h"



#define X0 GPIO_Pin_8	//PA8,9,10,12 as output mode
#define X1 GPIO_Pin_9
#define X2 GPIO_Pin_10
#define X3 GPIO_Pin_12
#define Y0 GPIO_Pin_5	//Set PB5,6,7,9 as INPUT mode
#define Y1 GPIO_Pin_6
#define Y2 GPIO_Pin_7
#define Y3 GPIO_Pin_9

unsigned int x_pin = {X0, X1, X2, X3};
unsigned int y_pin = {Y0, Y1, Y2, Y3};

// initial keypad gpio pin, X as output and Y as input

void keypad_init()
{
	// SET keypad gpio OUTPUT //
	RCC->AHB2ENR = RCC->AHB2ENR|0x2;
	//Set PA8,9,10,12 as output mode
	GPIOA->MODER= GPIOA->MODER&0xFDD5FFFF;
	//set PA8,9,10,12 is Pull-up output
	GPIOA->PUPDR=GPIOA->PUPDR|0x1150000;
	//Set PA8,9,10,12 as medium speed mode
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x1150000;
	//Set PA8,9,10,12 as high
	GPIOA->ODR=GPIOA->ODR|10111<<8;
	// SET keypad gpio INPUT //

	//Set PB5,6,7,9 as INPUT mode
	GPIOB->MODER=GPIOB->MODER&0xFFF303FF;
	//set PB5,6,7,9 is Pull-down input
	GPIOB->PUPDR=GPIOB->PUPDR|0x8A800;
	//Set PB5,6,7,9 as medium speed mode
	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x45400;

}

/**
 * scan keypad value
 *
 * return:
 *   >=0: key pressed value
 *   -1: no key press
 */
char keypad_scan()
{
	GPIOA->BSRR = X0;
	GPIOA->BRR = X1;
	GPIOA->BRR = X2;
	GPIOA->BRR = X3;

	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 1;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 4;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 7;
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 15;

	GPIOA->BRR = X0;
	GPIOA->BSRR = X1;
	GPIOA->BRR = X2;
	GPIOA->BRR = X3;

	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 2;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 5;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 8;
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 0;

	GPIOA->BRR = X0;
	GPIOA->BRR = X1;
	GPIOA->BSRR = X2;
	GPIOA->BRR = X3;

	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 3;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 6;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 9;
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 14;

	GPIOA->BRR = X0;
	GPIOA->BRR = X1;
	GPIOA->BRR = X2;
	GPIOA->BSRR = X3;

	if (GPIO_ReadInputDataBit(GPIOB, Y0))	return 10;
	if (GPIO_ReadInputDataBit(GPIOB, Y1))	return 11;
	if (GPIO_ReadInputDataBit(GPIOB, Y2))	return 12;
	if (GPIO_ReadInputDataBit(GPIOB, Y3))	return 13;

	return -1;
}

int main()
{
	GPIO_init();
	max7219_init();
	keypad_init();
	while(1){
		int input = keypad_scan();
			if(input == 255){
				display(input, 0);
			}
			else if (input >= 10){
				display(input, 2);
			}
			else if (input >= 0) {
				display(input, 1);
			}

	}
}
