#include "Uniaxial.h"
#include "circular_buffer.h"
#include "bsp_uart_ringbuffer.h"
#include "pressure.h"
#include "motor.h"
union Uniaxial_date_t Uniaxial_date;



void uniaxial_init(void){
	Uniaxial_date.press_resis.hand = 0xa55a;
	Uniaxial_date.press_resis.tran_type = 0xa1;
	Uniaxial_date.press_resis.date_type = 0x01;
	Uniaxial_date.press_resis.date_press = 0;
	Uniaxial_date.press_resis.date_resis = 0;
	Uniaxial_date.press_resis.date_check =0;  
}

void send_press_resis(uint16_t press, uint16_t resis ,uint16_t date_err){
	
	Uniaxial_date.press_resis.date_press = press;
	Uniaxial_date.press_resis.date_resis = resis;
	Uniaxial_date.press_resis.date_err = date_err;
	
	bsp_usart_dma_send_data(COM_UART_NUM0,(uint8_t *)&Uniaxial_date.date,sizeof(Uniaxial_date.date));
}

void demo(void){
	static int a,b = 0;
	static int16_t press = 0;
	static int16_t resis = 50;
	static int16_t err  = 50;
	press = get_now_pressure();
	err   = get_now_motor();
	send_press_resis(press,resis,err);
//	if(a == 0)
//		press++;
//	else
//		press--;
//	if(press >= 100)
//		a = 1;
//	if(press <= 0)
//		a = 0;
//	
//	
//	if(b == 0)
//		resis++;
//	else
//		resis--;
//	if(resis >= 100)
//		b = 1;
//	if(resis <= 0)
//		b = 0;
	

}