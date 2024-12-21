#ifndef _SWITCH_H
#define _SWITCH_H
#include "gd32f4xx.h"
#include "board.h"
#define RCU_SWITCH		RCU_GPIOA

#define PORT_SWITCH0	GPIOA
#define GPIO_SWITCH0	GPIO_PIN_3

#define PORT_SWITCH1	GPIOA
#define GPIO_SWITCH1	GPIO_PIN_4

#define PORT_SWITCH2	GPIOA
#define GPIO_SWITCH2	GPIO_PIN_5

#define	PORT_SWITCH3	GPIOA
#define GPIO_SWITCH3	GPIO_PIN_6

#define PORT_SWITCH4	GPIOA
#define GPIO_SWITCH4	GPIO_PIN_7

#define SWITCH_ADC_RCU			RCU_ADC0
#define SWITCH_ADC					ADC0
#define SWITCH0_ADC_CHANNEL	ADC_CHANNEL_3
#define SWITCH1_ADC_CHANNEL	ADC_CHANNEL_4
#define SWITCH2_ADC_CHANNEL ADC_CHANNEL_5
#define SWITCH3_ADC_CHANNEL ADC_CHANNEL_6
#define SWITCH4_ADC_CHANNEL ADC_CHANNEL_7

#define ENABLE_SWITCH_DMA_ADC

typedef enum {
    SWITCH_0 = 3,
    SWITCH_1,
    SWITCH_2,
    SWITCH_3,
    SWITCH_4,
} SWITCH_NUM;

typedef struct	{
		uint8_t switch_down_flag;
		int32_t switch_target;
}switch_moto_par_struct;
extern switch_moto_par_struct switch_moto_par[4];


void switch_thread_init(void);
void switch_thread_run(void);
uint16_t get_switch_value(SWITCH_NUM num);

#endif
