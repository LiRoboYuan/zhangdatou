#include "relay.h"
void bsp_relay_init(){
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7);
//	gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
//	rcu_periph_clock_enable(RCU_GPIOA);
//	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_15);
	KEY1(1);
}

void relay_run(int num){
	static int flag = 0;
	flag--;
	if(num == 1){
		KEY1(0);
		flag = 2;
		return;
	}
	if(flag == 0){
		KEY1(1);
	}
}

void beep_init(){
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);
}

void led_init()
{
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_13);
}
