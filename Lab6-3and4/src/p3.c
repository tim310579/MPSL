#include "stm32l476xx.h"
#include "utils.h"

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
	TIM2->ARR = (uint32_t) 99;
	// auto-reload prescaler value
	TIM2->CCMR1 &= 0xFFFFFCFF;
	// select compare 2 (channel 2 is configured as output)
	TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
	// set output compare 2 mode to PWM mode 1
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	// enable output compare 2 preload register on TIM2_CCR2
	TIM2->CCER |= TIM_CCER_CC2E;
	// enable compare 2 output
	TIM2->EGR = TIM_EGR_UG;
	// re-initialize the counter and generates an update of the registers
}


void keypad_init()
{
	RCC->AHB2ENR   |= 0b00000000000000000000000000000110;
	GPIOB->MODER   &= 0b11111111111011111111111110111111;
	GPIOB->PUPDR   &= 0b11111111111111110000000011111111;
	GPIOB->PUPDR   |= 0b00000000000000001010101000000000;
	GPIOB->OSPEEDR &= 0b11111111111111110000000011111111;
	GPIOB->OSPEEDR |= 0b00000000000000000101010100000000;
}

int main()
{
	fpu_enable();

	keypad_init();
	//gpio_init();

	timer_init();

		int i = 0;
		TIM2->PSC = (uint32_t) (4000000/50/100);	//period = 0.02sec
				//¿é¥X¯ßªi¶g´Á = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIMxCLK
		float change[5] = {2.5, 7.5, 10, 12.5, 15};
		for(i = 0; i < 5; i++){
			TIM2->CCR2 = change[i];		//change this vlaue to 2.5, 5, 7.5, 10, 12.5, 15
			TIM2->CR1 |= TIM_CR1_CEN;
			int j = 100000;
			while(j > 0){
				j--;
			}


		}

}
