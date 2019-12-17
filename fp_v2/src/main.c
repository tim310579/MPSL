#include "stm32l476xx.h"
extern void fpu_enable();


void keypad_init()
{
	RCC->AHB2ENR   |= 0b00000000000000000000000000000110;
	GPIOB->MODER   &= 0b11111111111011111010101010111111;
	GPIOB->PUPDR   &= 0b11111111111111111111111111111111;
	GPIOB->PUPDR   |= 0b00000000000000000000000000000000;
	GPIOB->OSPEEDR &= 0b11111111111111111111111111111111;
	GPIOB->OSPEEDR |= 0b00000000000000000000000000000000;
}
void timer_init_pb2(TIM_TypeDef *timer)
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// enable TIM2 timer clock
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0;
	// select AF1 for PB3 (PB3 is TIM2_CH2)
	timer->CR1 |= TIM_CR1_DIR;
	// counter used as downcounter
	timer->CR1 |= TIM_CR1_ARPE;
	// enable auto-reload preload (buffer TIM2_ARR)
	timer->ARR = (uint32_t) 99;
	// auto-reload prescaler value
	timer->CCMR1 &= 0xFFFFFCFF;
	// select compare 2 (channel 2 is configured as output)
	timer->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
	// set output compare 2 mode to PWM mode 1
	timer->CCMR1 |= TIM_CCMR1_OC2PE;
	// enable output compare 2 preload register on TIM2_CCR2
	timer->CCER |= TIM_CCER_CC2E;
	// enable compare 2 output
	timer->EGR = TIM_EGR_UG;
	// re-initialize the counter and generates an update of the registers
}


void timer_init_pb4 (TIM_TypeDef *timer)
{
	RCC->AHB2ENR |= 0x1 << 1;	/* enable AHB2 clock for port B */
	GPIOB->MODER |= GPIO_MODER_MODE4_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL4_1;	/* PB4: AF2 (TIM3_CH1) */

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	timer->ARR = (uint32_t) 99;
	timer->CR1 |= TIM_CR1_DIR;
	/* CH1 */
	//timer->CCR1 = 50;
	timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	timer->CCMR1 |= TIM_CCMR1_OC1PE;
	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR = TIM_EGR_UG;
	timer->CCER |= TIM_CCER_CC1E;	/* CH1 */
}
void timer_init_pb5 (TIM_TypeDef *timer)
{
	RCC->AHB2ENR |= 0x1 << 1;	/* enable AHB2 clock for port B */
	GPIOB->MODER |= GPIO_MODER_MODE5_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL5_1;	/* PB5: AF2 (TIM3_CH2) */

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	timer->ARR = (uint32_t) 99;
	timer->CR1 |= TIM_CR1_DIR;
	/* CH2 */
	//timer->CCR1 = 50;
	timer->CCMR1 &= 0xFFFFFCFF;
	timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
	timer->CCMR1 |= TIM_CCMR1_OC2PE;
	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR = TIM_EGR_UG;
	timer->CCER |= TIM_CCER_CC2E;	/* CH2 */
}
void timer_init_pb6 (TIM_TypeDef *timer)
{
	RCC->AHB2ENR |= 0x1 << 1;	/* enable AHB2 clock for port B */
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;	/* PB6: AF2 (TIM4_CH1) */

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	timer->ARR = (uint32_t) 99;
	timer->CR1 |= TIM_CR1_DIR;
	/* CH1 */
	//timer->CCR1 = 50;
	timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	timer->CCMR1 |= TIM_CCMR1_OC1PE;
	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR = TIM_EGR_UG;
	timer->CCER |= TIM_CCER_CC1E;	/* CH1 */
}
void timer_init_pb7 (TIM_TypeDef *timer)
{
	RCC->AHB2ENR |= 0x1 << 1;	/* enable AHB2 clock for port B */
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_1;	/* PB7: AF2 (TIM4_CH2) */

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	timer->ARR = (uint32_t) 99;
	timer->CR1 |= TIM_CR1_DIR;
	/* CH2 */
	//timer->CCR1 = 50;
	timer->CCMR1 &= 0xFFFFFCFF;
	timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
	timer->CCMR1 |= TIM_CCMR1_OC2PE;
	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR = TIM_EGR_UG;
	timer->CCER |= TIM_CCER_CC2E;	/* CH2 */
}
void SystemClock_Config(){
	RCC->CFGR = 0x00000001;	//choose HSI
	RCC->CR |= RCC_CR_HSION;// turn on HSI16 oscillator
	while((RCC->CR & RCC_CR_HSIRDY) == 0);//check HSI16 ready
	RCC->CFGR &= 0xFFFFFF0F;	//set bit 7:4 clear
	RCC->CFGR |= 11<<4; //SYSCLK divide by 16. SYSCLK = 16MHz/16 = 1Mhz
}
int main()
{
	SystemClock_Config();
	fpu_enable();
	keypad_init();
	TIM_TypeDef	*timer = TIM4;
	//TIM_TypeDef	*timer = TIM3;
	//TIM_TypeDef	*timer = TIM2;
	//gpio_init();
	//timer_init_pb2(timer);		//for pb3 tim2 ch2

	//timer_init_pb4(timer);		// for pb4 tim3 ch1

	//timer_init_pb5(timer);		// for pb5 tim3 ch2

	//timer_init_pb6(timer);		// for pb6 tim4 ch1

	timer_init_pb7(timer);		// for pb7 tim4 ch2
		//TIM2->PSC = (uint32_t) (4000000/50/100);	//period = 0.02sec
		timer->PSC = (uint32_t) (1000000/50/100);	//period = 0.02sec
		float duty = 10;
		A:
		//for(i = 0; i < 1; i++){
			// ** for pb3 ch2
			//timer->CCR2 = duty;		//change this value to 2.5, 5, 7.5, 10, 12.5, 15
			//timer->CR1 |= TIM_CR1_CEN;
			//***********************************************

			// ** for pb4 ch1
			timer->CCR1 = duty;		//change this vlaue to 2.5, 5, 7.5, 10, 12.5, 15
			timer->CR1 |= TIM_CR1_CEN;
			//***********************************************

			// ** for pb5 ch2
			//timer->CCR2 = duty;		//change this vlaue to 2.5, 5, 7.5, 10, 12.5, 15
			//timer->CR1 |= TIM_CR1_CEN;

			int j = 100000;
			while(j > 0){
				j--;
			}
			timer->CR1 &= ~TIM_CR1_CEN;
			j = 100000;
			while(j > 0){
				j--;
			}
			goto A;

		//}

}
