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

typedef enum {
    RS232_GATE,
    MOTO1_GATE,
    MOTO2_GATE,
    MOTO3_GATE,
    ESP32_GATE,
    RS485_GATE
} COM_GATE_T;

void gate_init(void);
void com_gate_for_usart(COM_GATE_T com_gate);
#endif
