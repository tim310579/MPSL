#ifndef UTILS_H_
#define UTILS_H_

// these functions are inside the asm file
extern void GPIO_init();
extern void max7219_init();
extern void max7219_send(unsigned char address, unsigned char data);
extern void Delay1sWith4MHz();

/**
 * show data on 7-seg via max7219_send
 *
 * input:
 *   data: decimal value
 *   num_digs: number of digits to show on 7-seg
 *
 * return:
 *   0: success
 *   -1: illegal data range (out of 8 digits)
 */
int display(int data, int num_digs){
	int i;
	if(data == -1){
		for(i = 1; i <= 8; i ++){
				max7219_send(i, 15);
			}
		return 0;
	}

	for(i = num_digs+1; i <= 8; i ++){
		max7219_send(i, 15);
	}
	if(num_digs == 0){

		for(i = 1; i <= 8; i++){
			max7219_send(i, 15);
		}
		return 0;
	}

		int neg = 0;
		int over = 0;
		if(data < 0){
			neg = 1;
		}
		if(data > 99999999 || data < -9999999){
			over = 1;
		}
		for (i = 1; i <= num_digs; i++){
			if (data % 10 < 0){
				max7219_send(i, -data % 10);
			}
			else{
				max7219_send(i, data % 10);
			}
			data /= 10;
		}
		if(neg == 1){
			max7219_send(num_digs, 10);	//means '-'
		}
		if(over == 1){
			for(i = 3; i <= 8; i++){
				max7219_send(i, 15);
			}
			max7219_send(2, 10);
			max7219_send(1, 1);
			return -1;
		}
		return 0;
}

#define GPIO_Pin_0  0b0000000000000001
#define GPIO_Pin_1  0b0000000000000010
#define GPIO_Pin_2  0b0000000000000100
#define GPIO_Pin_3  0b0000000000001000
#define GPIO_Pin_4  0b0000000000010000
#define GPIO_Pin_5  0b0000000000100000
#define GPIO_Pin_6  0b0000000001000000
#define GPIO_Pin_7  0b0000000010000000
#define GPIO_Pin_8  0b0000000100000000
#define GPIO_Pin_9  0b0000001000000000
#define GPIO_Pin_10 0b0000010000000000
#define GPIO_Pin_11 0b0000100000000000
#define GPIO_Pin_12 0b0001000000000000
#define GPIO_Pin_13 0b0010000000000000
#define GPIO_Pin_14 0b0100000000000000
#define GPIO_Pin_15 0b1000000000000000

int GPIO_ReadInputDataBit(GPIO_TypeDef *a, uint16_t b) {
	return a->IDR & b;
}

int displayf(float data, int num_digs)
{
	if (num_digs > 8)
		return display(-1, 2);
	if ((int) (data * 100) % 100)
		return display(data * 100, -1002 - num_digs);
	else
		return display(data, num_digs);
}

#endif /* UTILS_H_ */
