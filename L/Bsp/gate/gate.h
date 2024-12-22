#ifndef __GATE_H__
#define __GATE_H__
#include "main.h"

#define RS2251_A(a) if (a)	\
				gpio_bit_set(GPIOB, GPIO_PIN_15);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_15)

#define RS2251_B(a) if (a)	\
				gpio_bit_set(GPIOB, GPIO_PIN_14);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_14)
				
#define RS2251_C(a) if (a)	\
				gpio_bit_set(GPIOB, GPIO_PIN_13);\
				else		\
				gpio_bit_reset(GPIOB, GPIO_PIN_13)				
void gate_init(void);
void mcu232_run(void);
void motor3_run(void);
void motor2_run(void);
void motor1_run(void);
				
void esp32_run(void);
void rs485_run(void);
#endif
