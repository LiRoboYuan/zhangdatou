#include  "User_LYH.h"

#include "74hc4051bq.h"
#include "adc.h"
#include "main.h"


#include <stdio.h>
#include <stdlib.h>



uint8_t all_adc[Y][X] ={0};

void Y_Change(int16_t a);
void X_Change(int16_t b);

void  Y_start(int16_t *a , uint8_t* adc_ruturn){
	int num_get = 0;

	int16_t get_channel;
	if(*a >= 32) *a = 0;
	get_channel = *a;


	Y_Change(get_channel);
	for(int16_t b = 0; b<8 ;b++){
		X_Change(b);
		Hc4051Delay(8*220);

		for(int16_t c = 0;c < 8;c++){
			all_adc[get_channel][8*b+c] = (uint8_t)((GetAdcValue(c)*255)/1000);
			adc_ruturn[num_get++] = all_adc[get_channel][b+c*8];
		}
	}
	*a += 1;
}
unsigned int read_register(uint32_t a) {
	if(a == GPIOA)
    return GPIOA_ODR;
	else  if(a == GPIOB)
		return GPIOB_ODR;
	else  if(a == GPIOC)
		return GPIOC_ODR;
	else
		return 0;
}
void X_Change(int16_t b){//通道切换
	unsigned int ODR = read_register(GPIOC);//读取C的寄存器值
	ODR &= 0x00001FFF;
	switch(b){
		case 1:ODR |= 0x00000000;break;//GPIOC_ODR = 0x00000000;break;//C_A(0);C_B(0);C_C(0);break;//59    0
		case 4:ODR |= 0x00008000;break;//GPIOC_ODR = 0x00008000;break;//C_A(0);C_B(0);C_C(1);break;//56    1
		case 3:ODR |= 0x00004000;break;//GPIOC_ODR = 0x00004000;break;//C_A(0);C_B(1);C_C(0);break;//57    2
		case 5:ODR |= 0x0000C000;break;//GPIOC_ODR = 0x0000C000;break;//C_A(0);C_B(1);C_C(1);break;//58    3
		case 2:ODR |= 0x00002000;break;//GPIOC_ODR = 0x00002000;break;//C_A(1);C_B(0);C_C(0);break;//60    4
		case 7:ODR |= 0x0000A000;break;//GPIOC_ODR = 0x0000A000;break;//C_A(1);C_B(0);C_C(1);break;//62    5
		case 0:ODR |= 0x00006000;break;//GPIOC_ODR = 0x00006000;break;//C_A(1);C_B(1);C_C(0);break;//63    6
		case 6:ODR |= 0x0000E000;break;//GPIOC_ODR = 0x0000E000;break;//C_A(1);C_B(1);C_C(1);break;//61    7
	}
	GPIOC_ODR = ODR;
}

void Y_Change(int16_t a){

		unsigned int ODR_gpioc = read_register(GPIOC);
		unsigned int ODR_gpiob = read_register(GPIOB);
		unsigned int ODR_gpioa = read_register(GPIOA);
		ODR_gpioa &= 0x0000E6FF;
		ODR_gpiob &= 0x00000000;
		ODR_gpioc &= 0x00000000;
		switch(a){
			case 0:  ODR_gpioa |=0x00001000;break;//31
			case 1:  ODR_gpioa |=0x00000800;break;//30
			case 2:  ODR_gpioa |=0x00000100;break;//29
			case 3:  ODR_gpioc |= 0x00000200;break;//25
			case 4:  ODR_gpioc |= 0x00000100;break;//24
			case 5:  ODR_gpioc |= 0x00000080;break;//23
			case 6:  ODR_gpioc |= 0x00000040;break;//22/
			case 7:  ODR_gpiob |= 0x00004000;break;//14
			case 8:  ODR_gpiob |= 0x00001000;break;//12
			case 9:  ODR_gpiob |= 0x00008000;break;//15
			case 10: ODR_gpiob |= 0x00002000;break;//13
			case 11: ODR_gpiob |= 0x00000800;break;//11
			case 12: ODR_gpiob |= 0x00000400;break;//10
			case 13: ODR_gpiob |= 0x00000002;break;//1
			case 14: ODR_gpioc |= 0x00000020;break;//21
			case 15: ODR_gpiob |= 0x00000004;break;//2
			case 16: ODR_gpiob |= 0x00000200;break;	//9
			case 17: ODR_gpiob |= 0x00000040;break;	//6
			case 18: ODR_gpiob |= 0x00000010;break;//4
			case 19: ODR_gpiob |= 0x00000100;break;//8
			case 20: ODR_gpiob |= 0x00000080;break;//7
			case 21: ODR_gpiob |= 0x00000020;break;//5
			case 22: ODR_gpioc |= 0x00000001;break;//16
			case 23: ODR_gpioc |= 0x00000002;break;//17
			case 24: ODR_gpioc |= 0x00000004;break;//18
			case 25: ODR_gpioc |= 0x00000008;break;//19
			case 26: ODR_gpioc |= 0x00000000;break;//3
			case 27: ODR_gpioc |= 0x00001000;break;//28
			case 28: ODR_gpioc |= 0x00000800;break;//27
			case 29: ODR_gpioc |= 0x00000400;break;//26
			case 30: ODR_gpioc |= 0x00000010;break;//20
			case 31: ODR_gpiob |= 0x00000001;break;//0

			default:break;
	}
		GPIOC_ODR = ODR_gpioc;
		GPIOB_ODR = ODR_gpiob;
		GPIOA_ODR = ODR_gpioa;
}
