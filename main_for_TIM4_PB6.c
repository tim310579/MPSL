#include "stm32l476xx.h"
extern void fpu_enable();



void timer_init (TIM_TypeDef *timer);
void timer_start (TIM_TypeDef *timer);
void timer_stop (TIM_TypeDef *timer);


void timer_init (TIM_TypeDef *timer)
{
	timer->ARR = (uint32_t) 99;

	/* CH1 */
	//timer->CR1 |= TIM_CR1_DIR;
	//timer->CCMR1 &= 0xFFFFFCFF;
	timer->CCMR1 &= 0xFFFFFFFC;
	timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	timer->CCMR1 |= TIM_CCMR1_OC1PE;
	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR = TIM_EGR_UG;
	timer->CCER |= TIM_CCER_CC1E;	/* CH1 */
}
void keypad_init()
{
	//RCC->AHB2ENR   |= 0b00000000000000000000000000000010;
	//GPIOB->MODER   &= 0b11111111111011111111101010111111;
	//GPIOB->PUPDR   &= 0b11111111111111110000111111111111;
	//GPIOB->PUPDR   |= 0b00000000000000001010000000000000;
	//GPIOB->OSPEEDR &= 0b11111111111111111111111111111111;
	//GPIOB->OSPEEDR |= 0b00000000000000000000000000000000;
}
void timer_start (TIM_TypeDef *timer)
{
	timer->CR1 |= TIM_CR1_CEN;
}

void timer_stop (TIM_TypeDef *timer)
{
	timer->CR1 &= ~TIM_CR1_CEN;
}

int main (void)
{

	//keypad_init();
	TIM_TypeDef	*timer = TIM4;


	/* GPIO: set PB4 as alternate function */
	
	RCC->AHB2ENR |= 0x1 << 1;	/* enable AHB2 clock for port B */
	GPIOB->MODER  =0;
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	//GPIOB->MODER   &= 0b11111111111111111111101111111111;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;	/* PB4: AF2 (TIM3_CH1) */

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	timer_init (timer);

	fpu_enable();

	int i = 0;

	timer->PSC = (uint32_t) (4000000/50/100);	//period = 0.02sec
	float change[5] = {2.5,7.5, 10, 12.5, 15};
	for(i = 0; i < 1; i++){
		timer->CCR1 = change[i];		//change this vlaue to 2.5, 5, 7.5, 10, 12.5, 15
		timer->CR1 |= TIM_CR1_CEN;
		int j = 100000;
		while(j > 0){
			j--;
		}
	}

	return 0;
}
