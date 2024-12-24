#include "pressure.h"
#include "uart.h"
#include "Motor.h"

int pressure_state = 0;

void get_pressure(){
	static uint8_t motor_res[8];
	motor_res[0] = 0x01;
	motor_res[1] = 0x03;
	motor_res[2] = 0x00;
	motor_res[3] = 0x00;
	motor_res[4] = 0x00;
	motor_res[5] = 0x01;
	motor_res[6] = 0x84;
	motor_res[7] = 0x0A;
	usart_dma_send_data(USART_1_TR,(uint8_t *)&motor_res,sizeof(motor_res));
}

void pressure_run(){
	if(pressure_state == 1 && return_pressure() > 150){
		pressure_state = 2;
	}
	else if(pressure_state == 2 && return_pressure() < 10){
		pressure_state = 3;
	}
}
void clear_pressure_state(){
	pressure_state = 0;
}
void pressure_start(){
	pressure_state = 1;
}
int return_pressure_state(){
	if(pressure_state  == 3){
		pressure_state = 0;
		return 3;
	}
	else return pressure_state;
}

