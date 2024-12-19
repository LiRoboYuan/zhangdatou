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


#include "main.h"
#include "string.h"

#include "Motor.h"
#include <math.h>
#define PI 3.14159265358979323846
/*!
  \brief      main function
  \param[in]  none
  \param[out] none
  \retval     none
  */

//#define BLE_init


bool timer3_interrupt = true;
uint32_t time_1ms = 0;

uint8_t abc[5];

void print(double time);

int main(void)
           {
		systick_config();
			
    usart0_init(115200);
		usart2_init(115200);
	
		abc[0] = 1;
		abc[1] = 2;
		abc[3] = 3;
		motor_init();
    while(1)
    {
			time_1ms ++;			
			Check_data_from_python();
			
			if(time_1ms % 100 == 0){
//				get_curr();
//				usart_dma_send_data(USART_0_TR,(uint8_t *)&abc,sizeof(abc));
//				delay_1ms(2);
//				usart_dma_send_data(USART_2_TR,(uint8_t *)&abc,sizeof(abc));
				static double time = 0;
				time++;
				print(time/10);
			}
			
			
			delay_1ms(1);
    }	
}
float data[2];
void print(double time){
		data[0] = sin(PI * time);
		data[1] = cos(PI * time);
		usart_dma_send_data(USART_0_TR,(uint8_t *)&data,8);
		uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};
		
		usart_dma_send_data(USART_0_TR,(uint8_t *)tail,4);
}

void TIMER3_IRQHandler( void )
{
    if( SET == timer_interrupt_flag_get( TIMER3, TIMER_INT_FLAG_UP ) )
    {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear( TIMER3, TIMER_INT_FLAG_UP );

    }
}



