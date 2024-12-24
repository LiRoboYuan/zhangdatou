#include "dpm_rm550.h"
#include "gate.h"
#include <stdio.h>
#include "systick.h"
#include "uart.h"
void dpm_run(int value){
	rs485_run();
	delay_1ms(500);
  static char str[] = "01w10=";
	static char str_with_variable[50]; // 确保数组足够大以存放整个字符串
	// 将变量转换为字符串并拼接
  sprintf(str_with_variable, "%s%d\r\n", str, value);
	usart_dma_send_data(USART_2_TR,(uint8_t *)&str_with_variable,sizeof(str_with_variable));
	usart_dma_send_data(USART_0_TR,(uint8_t *)&str_with_variable,sizeof(str_with_variable));
	
	delay_1ms(1000);
	static char str1[] = "01w12=";
	static char str_with_variable11[50]; // 确保数组足够大以存放整个字符串
	// 将变量转换为字符串并拼接
  sprintf(str_with_variable11, "%s%d\r\n", str1, 1);
	usart_dma_send_data(USART_2_TR,(uint8_t *)&str_with_variable11,sizeof(str_with_variable11));
	usart_dma_send_data(USART_0_TR,(uint8_t *)&str_with_variable11,sizeof(str_with_variable11));
	delay_1ms(1000);

}


