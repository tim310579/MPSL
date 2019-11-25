#include "stm32l476xx.h"
#include "utils.h"
#include "core_cm4.h"


#define NVIC_ISER 0xE000E100 // interrupt set-enable registers
#define NVIC_ICPR 0xE000E280 // interrupt clear-pending registers
#define NVIC_IPR  0xE000E400 // interrupt priority registers
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

unsigned int total, total_check, len;
char set[14];
int rem = 0;
int plln = 40, pllm = 7, prescaler = 0; // 10 MHz SYSCLK
int prev_btn = 1, curr_btn = 1;
int state = 0;
int cal_len(int n)
{
	int sum = 0;
	while (n > 0)
	{
		n /= 10;
		sum++;
	}
	return sum;
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
	TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
	// set output compare 2 mode to PWM mode 1
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	// enable output compare 2 preload register on TIM2_CCR2
	TIM2->CCER |= TIM_CCER_CC2E;
	// enable compare 2 output
	TIM2->EGR = TIM_EGR_UG;
	// re-initialize the counter and generates an update of the registers
}

void timer_config()
{
	TIM2->PSC = (uint32_t) (400000 / freq / 100);
	// prescaler value
	TIM2->CCR2 = duty_cycle;
	// compare 2 preload value
}

void start_systick_timer()
{
	SysTick->CTRL |= 0x00000001;
	state = 1;
}

void stop_systick_timer()
{
	SysTick->CTRL &= 0xFFFFFFFE;
	state = 0;
}

#define SHPR3 0xE000ED20

void SysTick_UserConfig()
{
	SysTick->CTRL |= 0x00000004;
	SysTick->LOAD = 10000000; // 1.0 second
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x00000002;
	uint32_t *ptr;
	ptr = (uint32_t *) SHPR3;
	*ptr = *ptr | 0xF0000000;
}

void SysTick_Handler()
{

	if(total > 0 && state == 0) {
		total --;
		display(total, cal_len(total));
	}
	if(total == 0 && state == 0){
		display(0, 1);
		freq = DO;
		timer_config();
		TIM2->CR1 |= TIM_CR1_CEN;
		//while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==1);

	}
}


void SystemClock_Config()
{
	RCC->CFGR = 0x00000001;	//choose HSI
	RCC->CR |= RCC_CR_HSION;// turn on HSI16 oscillator
	while((RCC->CR & RCC_CR_HSIRDY) == 0);//check HSI16 ready
	RCC->CFGR &= 0xFFFFFF0F;	//set bit 7:4 clear
	RCC->CFGR |= 11<<4; //SYSCLK divide by 16. SYSCLK = 16MHz/16 = 1Mhz
}

void exti_init2()
{
	// setup SYSCFG
	SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PC;

	// setup EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	EXTI->PR1 |= EXTI_PR1_PIF13;

	// enable interrupts
	asm("cpsie i;");

	// setup NVIC
	// EXTI15_10_IRQn = 40
	uint32_t *ptr;
	ptr = (uint32_t *) NVIC_IPR;
	ptr[10] = 0x00000010;
	ptr = (uint32_t *) NVIC_ICPR;
	ptr[1] = 0x00000100;
	ptr = (uint32_t *) NVIC_ISER;
	ptr[1] = 0x00000100;
}

void EXTI15_10_IRQHandler()
{
	uint32_t *ptr;
	ptr = (uint32_t *) NVIC_ICPR;
	ptr[1] = 0x00000100;
	EXTI->PR1 |= EXTI_PR1_PIF13;
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==0){
		stop_systick_timer();
	}
}

int main()
{
	A:
	fpu_enable();
	SystemClock_Config();
	//SysTick_UserConfig();
	SysTick_Config(1000000);
	GPIO_init();
	max7219_init();
	keypad_init();
	timer_init();
	exti_init2();
	//TIM2->CR1 &= ~TIM_CR1_CEN;
	freq = -1;

	len = 0;
	int cnt = 0;
	int input = -1, prev_input = -1;
	//set_clear();
	total = -1;
	while(keypad_scan() == -1);
	total = keypad_scan();
	//display(total, cal_len(total));
	total++;
	state = 0;
	while (1){
		//if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==0){
			//state = 1;

			//goto A;
		//}
	}

}
