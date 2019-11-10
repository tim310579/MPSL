#include "stm32L476xx.h"
#include "utils.h"



#define X0 GPIO_Pin_8	//PA8,9,10,12 as output mode
#define X1 GPIO_Pin_9
#define X2 GPIO_Pin_10
#define X3 GPIO_Pin_12
#define Y0 GPIO_Pin_5	//Set PB5,6,7,9 as INPUT mode
#define Y1 GPIO_Pin_6
#define Y2 GPIO_Pin_7
#define Y3 GPIO_Pin_9

unsigned int x_pin = {X0, X1, X2, X3};
unsigned int y_pin = {Y0, Y1, Y2, Y3};

// initial keypad gpio pin, X as output and Y as input

void keypad_init()
{
	// SET keypad gpio OUTPUT //
	RCC->AHB2ENR = RCC->AHB2ENR|0x2;
	//Set PA8,9,10,12 as output mode
	GPIOA->MODER= GPIOA->MODER&0xFDD5FFFF;
	//set PA8,9,10,12 is Pull-up output
	GPIOA->PUPDR=GPIOA->PUPDR|0x1150000;
	//Set PA8,9,10,12 as medium speed mode
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x1150000;
	//Set PA8,9,10,12 as high
	GPIOA->ODR=GPIOA->ODR|10111<<8;
	// SET keypad gpio INPUT //

	//Set PB5,6,7,9 as INPUT mode
	GPIOB->MODER=GPIOB->MODER&0xFFF303FF;
	//set PB5,6,7,9 is Pull-down input
	GPIOB->PUPDR=GPIOB->PUPDR|0x8A800;
	//Set PB5,6,7,9 as medium speed mode
	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x45400;

}

/**
 * scan keypad value
 *
 * return:
 *   >=0: key pressed value
 *   -1: no key press
 */
int flag = -1;
int ret;
signed char keypad_scan()
{
		//ret = -1;
		int cnt = 0;
		int loc = -1;
		int pre_ret = ret;
		GPIOA->BSRR = X0;
		GPIOA->BRR = X1;
		GPIOA->BRR = X2;
		GPIOA->BRR = X3;

		if (GPIO_ReadInputDataBit(GPIOB, Y0)){
			ret = 1;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y1)){
			ret = 4;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y2)){
			ret = 7;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y3)){
			ret = 15;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}

		GPIOA->BRR = X0;
		GPIOA->BSRR = X1;
		GPIOA->BRR = X2;
		GPIOA->BRR = X3;

		if (GPIO_ReadInputDataBit(GPIOB, Y0)){
			ret = 2;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y1)){
			ret = 5;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y2)){
			ret = 8;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y3)){
			ret = 0;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}

		GPIOA->BRR = X0;
		GPIOA->BRR = X1;
		GPIOA->BSRR = X2;
		GPIOA->BRR = X3;

		if (GPIO_ReadInputDataBit(GPIOB, Y0)){
			ret = 3;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y1)){
			ret = 6;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y2)){
			ret = 9;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y3)){
			ret = 14;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}

		GPIOA->BRR = X0;
		GPIOA->BRR = X1;
		GPIOA->BRR = X2;
		GPIOA->BSRR = X3;

		if (GPIO_ReadInputDataBit(GPIOB, Y0)){
			ret = 10;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y1)){
			ret = 11;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y2)){
			ret = 12;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}
		if (GPIO_ReadInputDataBit(GPIOB, Y3)){
			ret = 13;
			cnt++;
			if(pre_ret != ret)	loc = ret;
		}

		if(cnt == 0) return -1;
		else if(cnt == 1) return ret;
		else return loc;
}

void clear_to_0(int a[14]){
	int i;
	for(i = 0; i < 14; i++){
		a[i] = 0;
	}
	return ;
}
int main()
{
	GPIO_init();
	max7219_init();
	keypad_init();
	int choose[14];
	int input = -1;
	int sum = 0;
	int not_blank = 0;
	clear_to_0(choose);
	int tmp_pre = -1;
		while(1){
			sum = 0;
			int input2 = input;	//previous

			input = keypad_scan();
			//clear_to_0(choose);
			//choose[input2] = 1;
			if(input2 == input && input != -1){	// just wait for next

				choose[input] = 1;
				//choose[tmp_pre] = 1;
				//sum = input;
			}
			else if(input2 == input && input == -1){
				clear_to_0(choose);		//no press
			}
			else if(input2 != input && input == -1){
				clear_to_0(choose);
				//choose[input2] = 1;
			}
			else if(input2 != input){

				choose[input] = 1;
				choose[input2] = 1;
			}

			else{
				clear_to_0(choose);
			}
			int i = 0;
			not_blank = 0;
			for(i = 0; i < 14; i++){
				if(choose[i] == 1) {
					sum += i;
					not_blank = 1;	//not_blank==1->display some num
				}
			}
			int dig = 0;
			if(sum > 9) dig = 2;
			else if(sum >= 0) dig = 1;

			if(not_blank == 1)	display(sum, dig);
			else	display(sum, 0);
		}
}
