#include "stm32l476xx.h"
#include "utils.h"

int money = 0;
void IRQ_Init(){
	//PC8-11
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[2] &= 0x0000;
	SYSCFG->EXTICR[2] |= 0x0022;

	EXTI->RTSR1 |=(3<<8);
	EXTI->FTSR1 &=~(3<<8);

	EXTI->PR1|=(3<<8);
	EXTI->IMR1|=(3<<8);

	//NVIC_SetPriority(EXTI15_10_IRQn,5);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI9_5_IRQHandler(void){ //user button

	if((GPIOC->IDR&(1<<8))){
		money+=5;
	}
	if((GPIOC->IDR&(1<<9))){
		money+=10;
	}
	EXTI->PR1|=(3<<8);
	//EXTI->PR1|=(1<<9);
}

int main()
{
	A:
	money = 0;
	gpio_init();
	max7219_init();
	display(0, 1);
	keypad_init();
	fpu_enable();
	PB_timer_init();
	display(0, 1);

	ray_init();

	IRQ_Init();
	coin:
	while(1){
		display(money, cal_len(money));
		if(keypad_scan() != -1) break;
	}

	int choose = keypad_scan();
	int rem = 0;
	if(choose == 1){
		if(money < 5) {
			display(87, 2);
			goto coin;
		}
		rem = money - 5;
		display(rem, cal_len(rem));
	}
	else if(choose == 2){
		if(money < 20) {
			display(87, 2);
			goto coin;
		}
		rem = money - 20;
		display(rem, cal_len(rem));
	}
	else if(choose == 3){
		if(money < 30) {
			display(87, 2);
			goto coin;
		}
		rem = money - 30;
		display(rem, cal_len(rem));
	}

	//TIM_TypeDef	*timer = TIM2;

	//TIM_TypeDef	*timer = TIM3;



	float duty = 1;
	float time = 440000;
	if(choose == 1){
		TIM_TypeDef	*timer = TIM3;
		timer->PSC = (uint32_t) (4000000/50/100);	//period = 0.02sec
		timer_init_pb4(timer);	// for pb4 tim3 ch1
		GPIOB->MODER &= GPIO_MODER_MODE4_1;	//disable others


		timer->CCR1 = duty/2;
		timer->CR1 |= TIM_CR1_CEN;
		int j = time;		//spin 360', one cycle
		while(j > 0){
			j--;
		}
		timer->CR1 &= ~TIM_CR1_CEN;
	}

	else if(choose == 2){		//second candy
		TIM_TypeDef	*timer = TIM3;
		timer->PSC = (uint32_t) (4000000/50/100);	//period = 0.02sec
		timer_init_pb5(timer);	// for pb5 tim3 ch2
		GPIOB->MODER &= GPIO_MODER_MODE5_1;	//disable others
		timer->CCR2 = duty;
		timer->CR1 |= TIM_CR1_CEN;
		int j = 250000;		//spin 360', one cycle

		while(j > 0){
			j--;
		}

		timer->CR1 &= ~TIM_CR1_CEN;
	}
	else if(choose == 3){
		TIM_TypeDef	*timer = TIM4;
		timer->PSC = (uint32_t) (4000000/50/100);	//period = 0.02sec
		timer_init_pb6(timer);	// for pb6 tim4 ch1
		GPIOB->MODER &= GPIO_MODER_MODE6_1;	//disable others
		timer->CCR1 = duty/2;
		timer->CR1 |= TIM_CR1_CEN;
		int j = time;		//spin 360', one cycle
		while(j > 0){
			j--;
		}
		timer->CR1 &= ~TIM_CR1_CEN;
	}
	//timer_init_pb3(timer);		//for pb3 tim2 ch2

	//timer_init_pb4(timer);		// for pb4 tim3 ch1

	//timer_init_pb5(timer);		// for pb5 tim3 ch2

	//timer_init_pb6(timer);		// for pb6 tim4 ch1

	//timer_init_pb7(timer);		// for pb7 tim4 ch2

	//timer_init_pb8(timer);		// for pb8 tim4 ch3

	//timer_init_pb9(timer);		// for pb9 tim4 ch4

		// ** for pb3 tim2 ch2
		//timer->CCR2 = duty;
		//***********************************************

		// ** for pb4 tim3 ch1
		//timer->CCR1 = duty;
		//***********************************************

		// ** for pb5 tim3 ch2
		//timer->CCR2 = duty;
		//*********************************************

		// ** for pb6 tim4 ch1
		//timer->CCR1 = duty;
		//**********************************************

		// ** for pb7 tim4 ch2
		//timer->CCR2 = duty;
		//*********************************************

		// ** for pb8 tim4 ch3
		//timer->CCR3 = duty;
		//*********************************************

		// ** for pb9 tim4 ch3
		//timer->CCR4 = duty;
		//*********************************************

		//timer->CR1 |= TIM_CR1_CEN;



		goto A;


}
