#include "pressure.h"
#include "uart.h"

void get_pressure(){
	static uint8_t abc[8];
	abc[0] = 0x01;
	abc[1] = 0x03;
	abc[2] = 0x00;
	abc[3] = 0x00;
	abc[4] = 0x00;
	abc[5] = 0x01;
	abc[6] = 0x84;
	abc[7] = 0x0A;
	usart_dma_send_data(USART_1_TR,(uint8_t *)&abc,sizeof(abc));
}