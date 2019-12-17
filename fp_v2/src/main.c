#include "stm32l476xx.h"
#include "utils.h"

int main()
{
	A:
	gpio_init();
	max7219_init();
	keypad_init();
	SystemClock_Config();
	fpu_enable();
	PB_timer_init();
	display(0, 1);
	while(keypad_scan() == -1);
	int choose = keypad_scan();
	if(choose == 1){
		display(5, cal_len(5));
	}
	else if(choose == 2){
		display(20, cal_len(20));
	}
	else if(choose == 3){
		display(30, cal_len(30));
	}

	//TIM_TypeDef	*timer = TIM2;

	//TIM_TypeDef	*timer = TIM3;



	float duty = 10;
	if(choose == 1){
		TIM_TypeDef	*timer = TIM3;
		timer->PSC = (uint32_t) (1000000/50/100);	//period = 0.02sec
		timer_init_pb4(timer);	// for pb4 tim3 ch1
		GPIOB->MODER &= GPIO_MODER_MODE4_1;	//disable others
		timer->CCR1 = duty;
		timer->CR1 |= TIM_CR1_CEN;
		int j = 110000;		//spin 360', one cycle
		while(j > 0){
			j--;
		}
		timer->CR1 &= ~TIM_CR1_CEN;
	}

	else if(choose == 2){		//second candy
		TIM_TypeDef	*timer = TIM3;
		timer->PSC = (uint32_t) (1000000/50/100);	//period = 0.02sec
		timer_init_pb5(timer);	// for pb5 tim3 ch2
		GPIOB->MODER &= GPIO_MODER_MODE5_1;	//disable others
		timer->CCR2 = duty;
		timer->CR1 |= TIM_CR1_CEN;
		int j = 110000;		//spin 360', one cycle
		while(j > 0){
			j--;
		}
		timer->CR1 &= ~TIM_CR1_CEN;
	}
	else if(choose == 3){
		TIM_TypeDef	*timer = TIM4;
		timer->PSC = (uint32_t) (1000000/50/100);	//period = 0.02sec
		timer_init_pb6(timer);	// for pb6 tim4 ch1
		GPIOB->MODER &= GPIO_MODER_MODE6_1;	//disable others
		timer->CCR1 = duty;
		timer->CR1 |= TIM_CR1_CEN;
		int j = 110000;		//spin 360', one cycle
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

		int delay = 110000;

		while(delay > 0){
			delay--;
		}

		goto A;


}
