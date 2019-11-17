#include "stm32l476xx.h"
#include "utils.h"
//#include "core_cm4.h"

int plln = 16, pllm = 7, prescaler = 9;

void delay_1s(){
	int i = 4000000;
	while(i > 0){
		i -= 4;
	}
}
void SystemClock_Config()
{
		RCC->CFGR = 0x00000000;
		RCC->CR &= 0xFEFFFFFF;	//set bit24 = 0, pll off
		while (RCC->CR & 0x02000000);	//wait until pllRDY id cleared, wait for unlock
		RCC->PLLCFGR = 0x01000001;
		// PLLCLK output enable
		// MSI clock selected as PLL, PLLSAI1 and PLLSAI2 clock entry
		RCC->PLLCFGR |= plln << 8;	//pllcfgr 8:15 is plln
		RCC->PLLCFGR |= pllm << 4;	//pllcfgr 4:6 is pllm

		RCC->CR |= 0x01000000;	//pllon =1, again
		while (!(RCC->CR & 0x02000000));	//wait for lock
		RCC->CFGR = 0x00000003;	// system clock switch: PLL selected as system clock
		RCC->CFGR |= prescaler << 4;	// AHB prescaler: SYSCLK divided by 4
		//System clock = f(PLL clock input) × (PLLN / PLLM) / PLLR(default==2)
}

int main(){

	SystemClock_Config();
	GPIO_init();


	int status[5] = {1, 6, 10, 16, 40};
	int state = status[0];
	//int prev_btn = 1, curr_btn = 1;
	int i = 0;
	while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) != 0);
	while(1) {
				switch (state)
				{
				case 1:
					plln = 16;
					pllm = 7;	//pllm = 8
					prescaler = 9;	//clc / 4
					//sys clk = f(default =4M)*plln(16)/pllm(8)/pllr(default=2)
					//sys clk / 4(pre == 9) = 1M
					break;
				case 6:
					plln = 24;
					pllm = 7;	//pllm = 8
					prescaler = 0;	//clc no change
					//4*24/8/2
					break;
				case 10:
					plln = 40;
					pllm = 7;	//pllm = 8
					prescaler = 0;

					break;
				case 16:
					plln = 64;
					pllm = 7;	//pllm = 8
					prescaler = 0;

					break;
				case 40:
					plln = 20;
					pllm = 0;	//pllm = 1
					prescaler = 0;

					break;
				default:
					break;
				}
				SystemClock_Config();
				//state = state == 40 ? 1 : state + 1;
				i = (i+1)%5;
				state = status[i];

			GPIOA->BRR = (1 << 5);
			//Delay1sWith4MHz();
			delay_1s();
			GPIOA->BSRR = (1 << 5);
			delay_1s();
			//Delay1sWith4MHz();
			//prev_btn = curr_btn;
			//curr_btn = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
	}
}


