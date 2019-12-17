#include<stm32l476xx.h>
#include"GPIO_init.h"
#include<stdlib.h>
#include<string.h>

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

}

void IRQ_Init(){
	//PC13
	SYSCFG->EXTICR[3] &= 0x7707;
	SYSCFG->EXTICR[3] |= 0x0020;

	EXTI->RTSR1&=~(1<<13);
	EXTI->FTSR1|=(1<<13);

	EXTI->PR1|=(1<<13);
	EXTI->IMR1|=(1<<13);

	//NVIC_SetPriority(EXTI15_10_IRQn,5);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
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
	//baud rate 9600
	//USART1->BRR = 0xFFFF & (4000000 / 9600);

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

void EXTI15_10_IRQHandler(void){ //user button

	if(!(GPIOC->IDR&(1<<13))){
		const char *str = "Hello World!\n";
		UART_Transmit((const uint8_t*)str, strlen(str));
	}
	EXTI->PR1|=(1<<13);
}

int UART_Receive(uint8_t *c){
	//blocking
	while (!(USART1->ISR & USART_ISR_RXNE));
	*c = (uint8_t)USART1->RDR;
	UART_Transmit(c,1);
	return 0;
}

int UART_ReceiveStr(uint8_t *buffer){
	uint8_t c;
	int i=0;
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

int main(){
	GPIO_Init();
	USART1_Init();
	IRQ_Init();

	//const char *str = "Hello World!\n";
	/*while(1)
		UART_Transmit((const uint8_t*)str, strlen(str));*/
	char *buffer = (char*)malloc(256*sizeof(char));
	while(1)UART_ReceiveStr((uint8_t*)buffer);


	return 0;
}
