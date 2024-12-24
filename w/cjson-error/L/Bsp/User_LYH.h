#ifndef __USER_LYH_h
#define __USER_LYH_h

#include "main.h"

#define X 64
#define Y 32

#define GPIOC_ODR *(unsigned int*)(GPIO_BASE + 0x00000800U + 0x0C)
#define GPIOB_ODR *(unsigned int*)(GPIO_BASE + 0x00000400U + 0x0C)
#define GPIOA_ODR *(unsigned int*)(GPIO_BASE + 0x00000000U + 0x0C)
extern void Y_start(int16_t *a , uint8_t* adc_ruturn);
#endif

