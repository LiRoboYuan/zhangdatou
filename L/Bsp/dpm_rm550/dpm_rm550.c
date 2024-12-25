#include "dpm_rm550.h"
#include "gate.h"
#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "uart.h"
void dpm_run(int value ,float sp){
	rs485_run();
	delay_1ms(500);
  char dpm[] = "01w10=";
	char str_with_variable[50]; // 确保数组足够大以存放整个字符串
	// 将变量转换为字符串并拼接
  sprintf(str_with_variable, ":%s%d,\r\n", dpm, value);
	usart_dma_send_data(USART_2_TR,(uint8_t *)&str_with_variable,strlen(str_with_variable));
	usart_dma_send_data(USART_0_TR,(uint8_t *)&str_with_variable,strlen(str_with_variable));
	
	delay_1ms(1000);
	char dpm1[] = "01w12=";
	char str_with_variable11[50]; // 确保数组足够大以存放整个字符串
	// 将变量转换为字符串并拼接
  sprintf(str_with_variable11, ":%s%d,\r\n", dpm1, 1);
	usart_dma_send_data(USART_2_TR,(uint8_t *)&str_with_variable11,strlen(str_with_variable11));
	usart_dma_send_data(USART_0_TR,(uint8_t *)&str_with_variable11,strlen(str_with_variable11));
	delay_1ms(1000);
	
	char rm[] = "AT+RES.SP=";
	char rm_value_send[50];
	sprintf(rm_value_send, "%s%.3f/", rm, sp);
	usart_dma_send_data(USART_2_TR,(uint8_t *)&rm_value_send,strlen(rm_value_send));
	delay_1ms(1000);
	char rm_enable[] = "AT+RES.CONNECT/";//"AT+DEV.BAUDRATE=115200/";
	usart_dma_send_data(USART_2_TR,(uint8_t *)&rm_enable,strlen(rm_enable));
	delay_1ms(1000);
	
}


