#ifndef _RELAY_H
#define _RELAY_H
#include "main.h"

#define KEY1(a) if(a) \
				gpio_bit_set(GPIOB, GPIO_PIN_7);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_7)
#define KEY2(a) if(a) \
				gpio_bit_set(GPIOB, GPIO_PIN_6);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_6)
#define KEY3(a) if(a) \
				gpio_bit_set(GPIOB, GPIO_PIN_4);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_4)
#define KEY4(a) if(a) \
				gpio_bit_set(GPIOB, GPIO_PIN_3);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_3)
#define KEY5(a) if(a) \
				gpio_bit_set(GPIOA, GPIO_PIN_15);\
				else		\
				gpio_bit_reset(GPIOA, GPIO_PIN_15)

#define RESISTANCE_TEST_COF(a) if(a) \
                gpio_bit_set(GPIOB, GPIO_PIN_12);\
                else\
                gpio_bit_reset(GPIOB, GPIO_PIN_12)

#define BEEP(a) if(a) \
				gpio_bit_set(GPIOB, GPIO_PIN_8);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_8)

#define LED(a) if(a) \
				gpio_bit_set(GPIOC, GPIO_PIN_13);\
				else		\
				gpio_bit_reset(GPIOC, GPIO_PIN_13)


void bsp_relay_init(void);
void beep_init(void);
void led_init(void);

#endif

