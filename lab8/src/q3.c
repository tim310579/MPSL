#include<stm32l476xx.h>
#include"GPIO_init.h"
#include<stdlib.h>
#include<string.h>

int lightMode=0;
const char *prompc = ">";

void GPIO_Init(){

	//PB6: TX(right left bottom 11), PB7: RX (left left bottom 9)

	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

	//USART
	__GPIO_Init__(GPIOB, 6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Low);
	__GPIO_Init__(GPIOB, 7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Low);

	GPIOB->AFR[0] &= ~0xFF000000;//clear
	GPIOB->AFR[0] |= 0x77000000;//set

	//button
	__GPIO_Init__(GPIOC, 13, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Medium);

	//ADC (on PC0)

	__GPIO_Init__(GPIOC, 0, GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Low);

	GPIOC->ASCR |= GPIO_ASCR_ASC0;

	//led

	__GPIO_Init__(GPIOA, 5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_Medium);

}

void USART1_Init(){
	//clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	//CR1
	USART1->CR1 &= ~(USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8);//clear
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;//set

	//CR2
	USART1->CR2 &= ~USART_CR2_STOP;//clear

	//CR3
	USART1->CR3 &= ~(USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT);//clear

	//BRR
	USART1->BRR = 0xFF & (4000000 / 115200);

	//clear
	USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

	//enable
	USART1->CR1 |= USART_CR1_UE;
}

int UART_Transmit(const uint8_t *arr, uint32_t size){
	for(int i = 0; i < size; i++){
		while (!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = arr[i];
	}
	while (!(USART1->ISR & USART_ISR_TC));
	return size;
}

int UART_Receive(uint8_t *c){
	//blocking
	while (!(USART1->ISR & USART_ISR_RXNE));
	*c = (uint8_t)USART1->RDR;
	if(!lightMode)
		UART_Transmit(c,1);

	return 0;
}

int UART_ReceiveStr(uint8_t *buffer){
	uint8_t c;
	int i=0;
	UART_Transmit((const uint8_t*)prompc,strlen(prompc));
	while(1){
		UART_Receive(&c);

		if((char)c=='\r'){
			buffer[i]=(uint8_t)'\0';
			break;
		}
		if(c==(uint8_t)127){
			i--;
		}
		else buffer[i++]=c;
	}

	return 0;
}

void TIM3_Config(){

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;

	TIM3->CR1 |= TIM_CR1_DIR;//dir count_down
	TIM3->ARR = (uint32_t)(0.5*100-1);//Reload value (0.5sec)
	TIM3->PSC = (uint32_t)39999;//Prescalser
	//freq: 4M/(39999+1) = 100 (Hz)
	TIM3->DIER |= TIM_DIER_UIE;//interrupt
	TIM3->EGR |= 1;//Reinitialize the counter

	//IRQ

	//__enable_irq();
	TIM3->SR &= ~TIM_SR_UIF;//clear bit
	NVIC_ClearPendingIRQ(TIM3_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);

}

void TIM3_IRQHandler(void){

	uint16_t adcVal;
	ADC1->CR |= ADC_CR_ADSTART;
	while(!(ADC1->ISR & ADC_ISR_EOC));
	adcVal = ADC1->DR;//read value

	char buffer[10];
	itoa((int)adcVal,buffer,10);
	strncat(buffer,"\n",1);
	UART_Transmit((uint8_t*)buffer,strlen(buffer));

	TIM3->SR &= ~TIM_SR_UIF;//clear bit
}

void ADC_Config(){

	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	/*RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;//reset
	RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;*/

	ADC1->CR &= ~ADC_CR_DEEPPWD;
	ADC1->CR |= ADC_CR_ADVREGEN;

	for(int i=0;i<300000;i++);

	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;//HCLK/2

	//ADC123_COMMON->CCR |= ADC_CCR_VREFEN;

	/*ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	ADC123_COMMON->CCR |= 0x0001;//div2
	*/

	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;//independent

	ADC123_COMMON->CCR &= ~ADC_CCR_MDMA;

	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	ADC1->CFGR &= ~ADC_CFGR_EXTSEL;
	ADC1->CFGR |= ADC_CFGR_EXTSEL_0;

	ADC1->CFGR &= ~ADC_CFGR_DMAEN;

	ADC1->CFGR &= ~ADC_CFGR_CONT;//single conversion
	//ADC1->CFGR |= ADC_CFGR_CONT;//cont conversion

	ADC1->CFGR &= ~ADC_CFGR_ALIGN;//right align

	ADC1->CFGR &= ~ADC_CFGR_RES;//12-bit res

	ADC1->SMPR1 = (0x001<<3); // cycles

	ADC1->SQR1 &= ~ADC_SQR1_SQ1;

	ADC1->SQR1 |= (ADC_SQR1_SQ1_0);//ch1
	//ADC1->SQR1 |= (ADC_SQR1_SQ1_2 | ADC_SQR1_SQ1_1);

	ADC1->SQR1 &= ~ADC_SQR1_L;//1 conversion



	//for(int i=0;i<4000000;i++);
	//calibration


	ADC1->CR &= ~ADC_CR_ADCALDIF;

	ADC1->CR |= ADC_CR_ADCAL;//start calibration

	while(ADC1->CR & ADC_CR_ADCAL);
	//int calFact = ADC1->CALFACT;

	//start ADC


	ADC1->CR |= ADC_ISR_ADRDY;
	ADC1->CR |= ADC_CR_ADEN;

	//ADC1->ISR &= ~ADC_ISR_ADRDY;

	while (!(ADC1->ISR & ADC_ISR_ADRDY));
		//ADC1->CR |= ADC_CR_ADEN;

	ADC1->CR |= ADC_CR_ADSTART;

	/*read
	while (!(ADC1->ISR & ADC_ISR_EOC));
	adcVal = ADC1->DR;
	*/

}

int main(){
	GPIO_Init();
	USART1_Init();
	ADC_Config();
	TIM3_Config();

	char *buffer = (char*)malloc(256*sizeof(char));
	const char *id = "0613457\n";
	const char *unknown = "Unknown Command\n";

	while(1){
		UART_ReceiveStr((uint8_t*)buffer);
		if(strcmp(buffer,"showid")==0){
			UART_Transmit((const uint8_t*)id,strlen(id));
		}
		else if(strcmp(buffer,"light")==0){
			lightMode = 1;
			TIM3->CR1 |= 1;//start timer
			char c;
			while(1){
				UART_Receive((uint8_t*)&c);

				if(/*lightMode && */c=='q'){
					//exit lightMode
					lightMode = 0;
					TIM3->CR1 &= ~1;//stop timer
					break;
				}

			}
		}
		else if(strcmp(buffer,"led on")==0){
			GPIOA->ODR |= (1<<5);
		}
		else if(strcmp(buffer,"led off")==0){
			GPIOA->ODR &= ~(1<<5);
		}
		else if(buffer[0]=='\0'){

		}
		else{
			UART_Transmit((const uint8_t*)unknown,strlen(unknown));
		}
	}

	free(buffer);
	return 0;
}
