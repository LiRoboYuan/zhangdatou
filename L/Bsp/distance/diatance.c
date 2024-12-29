#include "diatance.h"
#include "uart.h"
#include "gate.h"
void distance_init(void){
	motor2_run();
	delay_1ms(10);
	static uint8_t c[8];
	c[0] = 0x50;
	c[1] = 0x06;
	c[2] = 0x00;
	c[3] = 0x00;
	c[4] = 0x00;
	c[5] = 0x01;
	c[6] = 0x45;
	c[7] = 0x8b;
	usart_dma_send_data(USART_2_TR,(uint8_t *)&c,sizeof(c));
	delay_1ms(1);
	
}


