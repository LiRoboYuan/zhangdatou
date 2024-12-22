#include "relay.h"


void bsp_relay_init(){
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7);
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_15);
}

void beep_init(){
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);
	
	
}