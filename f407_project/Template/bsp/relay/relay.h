#ifndef _RELAY_H
#define _RELAY_H
#include "board.h"

typedef enum
{
	RELAY1 = 0,
	RELAY2 = 0,
	RELAY3 = 0,
	RELAY4 = 0,
	RELAY_MAX_NUM = 4
}relay_type_def;

#define RELAY1_PIN     				GPIO_PIN_9
#define RELAY1_GPIO_PORT      GPIOC
#define RELAY1_GPIO_CLK         RCU_GPIOC

#define RELAY2_PIN     				GPIO_PIN_12
#define RELAY2_GPIO_PORT      GPIOC
#define RELAY2_GPIO_CLK         RCU_GPIOC

#define RELAY3_PIN     				GPIO_PIN_11
#define RELAY3_GPIO_PORT      GPIOC
#define RELAY3_GPIO_CLK         RCU_GPIOC

#define RELAY4_PIN     				GPIO_PIN_13
#define RELAY4_GPIO_PORT      GPIOC
#define RELAY4_GPIO_CLK         RCU_GPIOC

void bsp_relay_init(void);
void bsp_relay_on(relay_type_def relay);
void bsp_relay_off(relay_type_def relay);
void bsp_relay_off(relay_type_def relay);

#endif

