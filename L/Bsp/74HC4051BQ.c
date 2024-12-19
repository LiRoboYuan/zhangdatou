#include "74hc4051bq.h"
#include "adc.h"
#include "math.h"
#include "string.h"
#include <stdio.h>


Hc4051AdcValue adc_raw_value;
#define ADC_DELAY_TIME (7 * 220) //9 ns * 120


void Divider_Resistance_Selection(uint8_t data);
void Hc4051Delay(uint32_t num)
{
    while(num --);
}

/*
功能：Init
输入：none
*/ 
void Hc4051IoInit(void)
{
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_GPIOB);
		rcu_periph_clock_enable(RCU_GPIOC);
		rcu_periph_clock_enable(RCU_AF);
		gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15);
		gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_ALL);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_ALL);
//		AX31(0);AX30(0);AX29(0);AX25(0);AX24(0);AX23(0);AX22(0);AX14(0);AX12(0);AX15(0);AX13(0);AX11(0);AX10(0);AX1(0);AX21(0);AX2(0);AX0(0);AX20(0);AX26(0);AX27(0); AX28(0);AX3(0);AX19(0);AX18(0);AX17(0);AX16(0);AX5(0);AX7(0);AX8(0); AX4(0);AX6(0); AX9(0);
	
		GREEN_LED(0);
}


