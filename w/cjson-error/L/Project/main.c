/*!
  \file  main.c
  \brief ADC0 regular channel with DMA

  \version 2017-12-26, V1.0.0, firmware for GD32E10x
  */

/*
   Copyright (c) 2017, GigaDevice Semiconductor Inc.

   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
   3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software without
   specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
   OF SUCH DAMAGE.
   */
#include <stdlib.h>
#include "gd32e10x.h"
#include "systick.h"
#include <stdio.h>

#include "uart.h"
#include "timer.h"
#include "adc.h"

#include "main.h"
#include "string.h"

#include "pressure.h"
#include "Motor.h"
#include "Uniaxial.h"
#include "getJson.h"
#include "gate.h"
#include "relay.h"

bool moto_init_ok = true;
uint32_t time_1ms = 0;


int main(void)
{
    bool beep_led_flag = false;
    uint8_t cache_buffer[32] = {"wzh test"};
    systick_config();

    usart0_init(460800);
    usart1_init(460800);
    usart2_init(460800);

    gate_init();
    bsp_relay_init();
    beep_init();
    led_init();

    BEEP(0);

    com_gate_for_usart(ESP32_GATE);

    while(1)
    {
        time_1ms ++;
        if (time_1ms % 500 == 0)
        {
            beep_led_flag = !beep_led_flag;
            LED(beep_led_flag);
			BEEP(beep_led_flag);
            //RESISTANCE_TEST_COF(beep_led_flag);
            usart_dma_send_data(USART_0_TR, cache_buffer, 10);
            usart_dma_send_data(USART_2_TR, cache_buffer, 10);
            usart_dma_send_data(USART_1_TR, cache_buffer, 10);
        }
        if(time_1ms % 100 == 0)
        {
            //Check_data_from_python();
            //get_err();

        }
        if(time_1ms % 500 == 0)
        {
            //getJsonTask();
        }

        delay_1ms(1);
    }
}


void TIMER3_IRQHandler( void )
{
    if( SET == timer_interrupt_flag_get( TIMER3, TIMER_INT_FLAG_UP ) )
    {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear( TIMER3, TIMER_INT_FLAG_UP );

    }
}


