#ifndef __ADC_h
#define __ADC_h

#include "gd32e10x.h"
#include "string.h"
#include "systick.h"
#include "stdbool.h"

#define cumulative_num 8
#define adc_channel_num 8

extern uint16_t  adc_v[adc_channel_num];

void rcu_config(void);
void gpio_config(void);
void dma_config(void);
void adc_config(void);
void adc_init(void);
void read_adc_value(void);
uint16_t GetAdcValue(uint8_t index);
#endif

