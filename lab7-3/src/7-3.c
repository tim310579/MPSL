#include "stm32l476xx.h"
#include "utils.h"
#include "core_cm4.h"


#define DO 261.6
#define RE 293.7
#define MI 329.6
#define FA 349.2
#define SO 392.0
#define LA 440.0
#define SI 493.9
#define HI_DO 523.3

float freq = -1;
int duty_cycle = 50;
//float star[14] = {DO, DO, SO, SO, LA, LA, SO, FA, FA, MI, MI, RE, RE, DO};
unsigned int total;
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
int cnt = 0;
void SysTick_Handler()
{
	//cnt++;
	//cnt %= 14;
	if(state == 1) return;
	if(total > 0 && state == 0) {
		total --;
		display(total, cal_len(total));
	}
	if(total == 0 && state == 0){
		display(0, 1);
		freq = LA;
		timer_config();
		TIM2->CR1 |= TIM_CR1_CEN;

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

int main(void){
	A:
	fpu_enable();
	SystemClock_Config();
	SysTick_Config(1000000);
	GPIO_init();
	max7219_init();
	keypad_init();
	timer_init();
	freq = -1;
	state = 0;
	total = -1;
	while(keypad_scan() == -1);
	total = keypad_scan();
	//display(total, cal_len(total));
	total++;

	while (1){
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==0){
			state = 1;
			TIM2->CR1 &= ~TIM_CR1_CEN;
			freq = -1;
			//total = 0;
			goto A;
		}
	}
}
