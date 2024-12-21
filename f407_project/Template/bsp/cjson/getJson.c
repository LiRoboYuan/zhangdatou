#include "getJson.h"
#include "bsp_uart.h"
#include "circular_buffer.h"
#include "bsp_uart_ringbuffer.h"
#include "board.h"
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
	
	data_count = uart_rx_ringbuffer_get_data_count(COM_UART_NUM0);
	
	if (data_count > 0) {
		read_size = uart_rx_ringbuffer_pop_data(COM_UART_NUM0, uart_rx_buffer, data_count);
		
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
	int result = 0;  // 1 表示成功，其他值表示错误
	uint16_t get_size = 0;
	uint16_t uart_get_num = uart_rx_ringbuffer_get_data_count(COM_UART_NUM0);
	memset(uart_rx_buffer, 0x00, sizeof(uart_rx_buffer));

	if(uart_get_num > 0){
		get_size = uart_rx_ringbuffer_pop_data(COM_UART_NUM0, uart_rx_buffer, uart_get_num);
		uart_rx_buffer[uart_get_num] = NULL;
//		printf("%s\n",uart_rx_buffer);
		cJSON *cjson = NULL , *motor_mode = NULL;
		cjson = cJSON_Parse((const char*)uart_rx_buffer);
		if (cjson != NULL) {
			// 获取 "motor_mode" 字段
			cJSON *motor_mode = cJSON_GetObjectItem(cjson, "motor_mode");
			cJSON *relay_flag = cJSON_GetObjectItem(cjson, "relay_flag");
			cJSON *location_num = cJSON_GetObjectItem(cjson, "location_num");
			cJSON *run_location = cJSON_GetObjectItem(cjson, "run_location");
			cJSON *test_location = cJSON_GetObjectItem(cjson, "test_location");
			
			jsondata.motor_mode = atoi(motor_mode->valuestring);
			jsondata.relay_flag = atoi(relay_flag->valuestring);
			jsondata.location_num = atoi(location_num->valuestring);
			jsondata.run_location = atoi(run_location->valuestring);
			int item_count = cJSON_GetArraySize(test_location);
			//printf("\njsondata.motor_mode=%d,jsondata.relay_flag = %d",jsondata.motor_mode,jsondata.relay_flag);
//			printf("\njsondata.location_num = %d",jsondata.location_num);
//			printf("\njsondata.run_location = %d",jsondata.run_location);
			printf("\ntest_location = %d",item_count);
			// 清理内存
			cJSON_Delete(cjson);
		} 
		else {
				printf("\nError: JSON parsing failed  %d\n",get_size);
		}
	}
	
	return result;
}

