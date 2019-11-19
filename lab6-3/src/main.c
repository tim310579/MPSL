#include "stm32l476xx.h"
#include "utils.h"
//#include "core_cm4.h"

#define DO 261.6
#define RE 293.7
#define MI 329.6
#define FA 349.2
#define SO 392.0
#define LA 440.0
#define SI 493.9
#define HI_DO 523.3

float freq = -1;
int curr = -2, prev = -3, check = -4;
int duty_cycle = 50;


#define X0 GPIO_Pin_0	//pc0,1,2,3 output
#define X1 GPIO_Pin_1
#define X2 GPIO_Pin_2
#define X3 GPIO_Pin_3
#define Y0 GPIO_Pin_6	//pb4,5,6,7
#define Y1 GPIO_Pin_5
#define Y2 GPIO_Pin_4
#define Y3 GPIO_Pin_7

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
void timer_init()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// enable TIM2 timer clock
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0;
	// select AF1 for PB3 (PB3 is TIM2_CH2)
	TIM2->CR1 |= TIM_CR1_DIR;
	// counter used as downcounter
	TIM2->CR1 |= TIM_CR1_ARPE;
	// enable auto-reload preload (buffer TIM2_ARR)
	TIM2->ARR = (uint32_t) 100;
	// auto-reload prescaler value
	TIM2->CCMR1 &= 0xFFFFFCFF;
	// select compare 2 (channel 2 is configured as output)
	TIM2->CCMR1 |= 0b110000000000000;
	// set output compare 2 mode to PWM mode 1
	TIM2->CCMR1 |= 0b10000000000;
	// enable output compare 2 preload register on TIM2_CCR2
	TIM2->CCER |= TIM_CCER_CC2E;
	// enable compare 2 output
	TIM2->EGR = TIM_EGR_UG;
	// re-initialize the counter and generates an update of the registers
}

void timer_config()
{
	TIM2->PSC = (uint32_t) (4000000 / freq / 100);
	// prescaler value
	TIM2->CCR2 = duty_cycle;
	// compare 2 preload value
}

void keypad_ctrl()
{
	while (1)
	{
		check = keypad_scan();
		switch (check)
		{
		case 1:
			freq = DO;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 2:
			freq = RE;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 3:
			freq = MI;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 4:
			freq = FA;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 5:
			freq = SO;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 6:
			freq = LA;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 7:
			freq = SI;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;
		case 8:
			freq = HI_DO;
			timer_config();
			TIM2->CR1 |= TIM_CR1_CEN;
			break;

		default:
			TIM2->CR1 &= ~TIM_CR1_CEN;
			freq = -1;
			break;
		}
	}
}

int main()
{
	fpu_enable();
	GPIO_init();
	keypad_init();
	timer_init();
	keypad_ctrl();
}
