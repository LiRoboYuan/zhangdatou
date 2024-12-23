#include "getJson.h"
#include "uart.h"
#include "circular_buffer.h"
#include <stdlib.h>
#include "main.h"
#include "cJSON.h"
#include "string.h"

uint8_t uart_rx_buffer[1024];
int read_sizE = 0;
uint8_t json_buffer_test[] = "{\"seq\":2,\"vendor\":[{\"sid\":\"123456\",\"data\":{\"key\":\"88888888\",\"ssid\":\"HS\"}}]}";
uint8_t json_buffer_test1[] ="{\"motor_mode\":\"1\",\"relay_flag\":\"2\",\"location_num\":\"4\",\"run_location\":\"5\",\"run_location1\":\"5\","
"\"test_location\":{\"location1\":\"1111\",\"location2\":\"1\",\"location3\":\"11111\",\"location4\":\"11111441141\"}}";


json_data_t jsondata;

int iwaaa=0;
int cJSON_demo(void){
	uint16_t data_count = 0;
	uint16_t read_size = 0;
	int result = 0;  // 0 表示成功，其他值表示错误
	
	data_count = CircBuf_GetUsedSize(&USART0_RxCBuf);
	
	if (data_count > 0) {
		read_size = CircBuf_Pop(&USART0_RxCBuf, uart_rx_buffer, data_count);
		
		cJSON *cjson = NULL, *single_json = NULL;
		
		cjson = cJSON_Parse((const char*)uart_rx_buffer);
		if( cjson != NULL){
			
			single_json = cJSON_GetObjectItem(cjson, "items");
			
			single_json = cJSON_GetArrayItem(single_json, 0);
			single_json = cJSON_GetObjectItem(single_json, "CMD");
			int abcd = atoi(single_json->valuestring);
			printf("%s:json size %d  %s\n", __FUNCTION__, data_count, single_json->valuestring);
			
//			uart_rx_ringbuffer_clean(COM_UART_NUM0);
		}
		cJSON_Delete(cjson);
	}		
	return result;
	
}


int get_Json_data(void){
	int result = -1;  // 1 表示成功，其他值表示错误
	uint16_t get_size = 0;
	
	uint16_t uart_get_num = CircBuf_GetUsedSize(&USART0_RxCBuf);
//	memset(uart_rx_buffer, 0x00, sizeof(uart_rx_buffer));
	if(uart_get_num > 0){
		get_size = CircBuf_Pop(&USART0_RxCBuf, uart_rx_buffer, uart_get_num);
		printf("%s\n",(const char*)uart_rx_buffer);
		cJSON *cjson = NULL , *motor_mode = NULL;
		cjson = cJSON_Parse((const char*)uart_rx_buffer);
		memset(uart_rx_buffer,0x00,1024);
		if (cjson != NULL) {
			// 获取 "motor_mode" 字段
			cJSON *motor_mode = cJSON_GetObjectItem(cjson, "motor_mode");
			jsondata.motor_mode = atoi(motor_mode->valuestring);
			switch(jsondata.motor_mode)
			{
				case 0:result=jsondata.motor_mode;break;//失能电机
				case 1:result=jsondata.motor_mode;break;//使能电机
				case 2:result=jsondata.motor_mode;break;//下落压头（单次）
				case 3:
					result=jsondata.motor_mode;
					cJSON *run_location  = cJSON_GetObjectItem(cjson, "run_location");
					jsondata.run_location = atoi(run_location->valuestring);
					printf("\n jsondata.run_location = %d",jsondata.run_location);
					break;//移动平台
				case 4:
					result=jsondata.motor_mode;
					cJSON *test_lactiong = cJSON_GetObjectItem(cjson, "test_lactiong");
					jsondata.test_lactiong = atoi(test_lactiong->valuestring);
					printf("\n jsondata.test_lactiong = %d",jsondata.test_lactiong);
					break;//记住一个点位
				case 5:result=jsondata.motor_mode;break;//删去一个点位
				case 6:
					result=jsondata.motor_mode;
					cJSON *run_test_num = cJSON_GetObjectItem(cjson, "run_test_num");
					jsondata.run_test_num = atoi(run_test_num->valuestring);
					printf("\n jsondata.run_test_num = %d",jsondata.run_test_num);
					break;//启动连续测试
			}

			printf("\n result = %d",result);
			// 清理内存
			cJSON_Delete(cjson);
		} 
		else {
				printf("\nError: JSON parsing failed  %d\n",get_size);
		}
	}

	return result;
}
int get_run_test_num(void){
	return	jsondata.run_test_num;
}
int get_run_test_laction(void){
	return	jsondata.test_lactiong;
}
int get_run_location(void){
	
	return jsondata.run_location;
}
