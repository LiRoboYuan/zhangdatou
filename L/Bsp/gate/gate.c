#include "gate.h"

void gate_init(void){
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	gpio_bit_set(GPIOB, GPIO_PIN_12);
}


void mcu232_run(void){
	RS2251_A(0);
	RS2251_B(0);
	RS2251_C(0);
}
void motor3_run(void){
	RS2251_A(1);
	RS2251_B(0);
	RS2251_C(0);
}
void motor2_run(void){
	RS2251_A(0);
	RS2251_B(1);
	RS2251_C(0);
}
void motor1_run(void){
	RS2251_A(1);
	RS2251_B(1);
	RS2251_C(0);
}
void esp32_run(void){
	RS2251_A(0);
	RS2251_B(0);
	RS2251_C(1);
}
void rs485_run(void){
	RS2251_A(1);
	RS2251_B(0);
	RS2251_C(1);
}