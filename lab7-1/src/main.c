#include "stm32l476xx.h"
#include "utils.h"
#include "core_cm4.h"
void SystemClock_Config(){
	RCC->CFGR = 0x00000001;	//choose HSI
	RCC->CR |= RCC_CR_HSION;// turn on HSI16 oscillator
	while((RCC->CR & RCC_CR_HSIRDY) == 0);//check HSI16 ready
	RCC->CFGR &= 0xFFFFFF0F;	//set bit 7:4 clear
	RCC->CFGR |= 11<<4; //SYSCLK divide by 16. SYSCLK = 16MHz/16 = 1Mhz
}
int t = 0;
void SysTick_Handler(void){
	/*t++;
	if(t == 1) GPIOA->BSRR = (1<<5);
	else if(t == 2) {
		GPIOA->BRR = (1<<5);
		t = 0;
	}*/
	GPIOA->ODR = ~GPIOA->ODR;
}

int main()
{
	SystemClock_Config();
	SysTick_Config(3000000);
	GPIO_init();
	while(1);
}
