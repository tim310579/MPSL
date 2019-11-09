extern void GPIO_init();
extern void max7219_init();
extern void max7219_send(unsigned char address, unsigned char data);

int display(int data, int num_digs) {
	int i;
	int neg = 0;
	int over = 0;
	if(data < 0){
		neg = 1;
	}
	if(data > 99999999 || data < -99999999){
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
int main() {
	int student_id = 616027;
	GPIO_init();
	max7219_init();
	display(student_id, 8);
}
