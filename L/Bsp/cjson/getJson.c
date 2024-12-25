#include "getJson.h"
#include "uart.h"
#include "circular_buffer.h"
#include <stdlib.h>
#include "main.h"
#include "cJSON.h"
#include "string.h"
#include "priv_malloc.h"

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
	int result = 0;  // 0 ��ʾ�ɹ�������ֵ��ʾ����
//	data_count = CircBuf_GetUsedSize(&USART0_RxCBuf);
	if (data_count > 0) {
//		read_size = CircBuf_Pop(&USART0_RxCBuf, uart_rx_buffer, data_count);
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
	int result = -1;  // 1 ��ʾ�ɹ�������ֵ��ʾ����
	uint16_t get_size = 0;
	uint16_t uart_get_num = usart_get_rx_data_count(USART_0_TR);

	if(uart_get_num > 0){
		get_size = usart_recv(USART_0_TR, uart_rx_buffer, uart_get_num);
//		printf("\n uart_get_num %d",uart_get_num);
		cJSON *cjson = NULL , *motor_mode = NULL;
		cjson = cJSON_Parse((const char*)uart_rx_buffer);
		
		memset(uart_rx_buffer,0x00,1024);
		if (cjson != NULL) {
			// ��ȡ "motor_mode" �ֶ�
			cJSON *motor_mode = cJSON_GetObjectItem(cjson, "motor_mode");
			jsondata.motor_mode = atoi(motor_mode->valuestring);
			switch(jsondata.motor_mode)
			{
				case 0:result=jsondata.motor_mode;break;//ʧ�ܵ��
				case 1:result=jsondata.motor_mode;break;//ʹ�ܵ��
				case 2:result=jsondata.motor_mode;break;//����ѹͷ�����Σ�
				case 3:
					result=jsondata.motor_mode;
					cJSON *run_location  = cJSON_GetObjectItem(cjson, "run_location");
					jsondata.run_location = atoi(run_location->valuestring);
					break;//�ƶ�ƽ̨
				case 5:
					result=jsondata.motor_mode;
					break;
				case 6:
					result=jsondata.motor_mode;
					cJSON *run_test_num = cJSON_GetObjectItem(cjson, "run_test_num");
//					jsondata.run_test_num = atoi(run_test_num->valuestring);
				
					cJSON *test_point_total = cJSON_GetObjectItem(cjson, "test_point_config");
					test_point_total = cJSON_GetObjectItem(test_point_total, "test_point_total");
					cJSON *position = cJSON_GetObjectItem(test_point_total, "position");
					int position_count = cJSON_GetArraySize(position);
					jsondata.run_test_num = position_count;
					jsondata.test_lactiong = (int *)board_malloc(position_count * sizeof(int));
					for (int i = 0; i < position_count; i++) {
					cJSON *item = cJSON_GetArrayItem(position, i);
					if (cJSON_IsNumber(item)) {
							// ��ÿ�����ִ洢�� position_array ������
							jsondata.test_lactiong[i] = item->valueint;
							printf("position[%d] = %d\n", i, jsondata.test_lactiong[i]);
					} else {
							printf("Error: 'position' item %d is not a number.\n", i);
					}
			}
					break;
//				case 6:
//					result=jsondata.motor_mode;
//					cJSON *run_test_num = cJSON_GetObjectItem(cjson, "run_test_num");
//					cJSON *test_location = cJSON_GetObjectItem(cjson, "test_location");
//					jsondata.run_test_num = atoi(run_test_num->valuestring);
//					jsondata.test_lactiong = (int *)board_malloc(jsondata.run_test_num * sizeof(int));
//					if (!jsondata.test_lactiong) 
//						printf("Memory allocation failed!\n");
//					char key[50];
//					for (int i = 0; i < jsondata.run_test_num; ++i) {
//						// �������
//						snprintf(key, sizeof(key), "test_location%d", i + 1);
//						// ��ȡ��Ӧ�� JSON ����
//						cJSON *location_item = cJSON_GetObjectItem(test_location, key);
//						if (location_item && cJSON_IsString(location_item)) {
//								// ���ַ���ת��Ϊ�������洢
//								jsondata.test_lactiong[i] = atoi(location_item->valuestring);
//						} else {
//								printf("Key %s not found or is not a valid string!\n", key);
//								jsondata.test_lactiong[i] = 0; // Ĭ��ֵΪ 0
//						}
//					}	
//					break;//������������
			}
			// �����ڴ�
			cJSON_Delete(cjson);
		} 
		else {
				printf("\nError: JSON parsing failed  %d\n",get_size);
		}
	}
	return result;
}

void clean_test_location(void){
	board_free(jsondata.test_lactiong);
}

int *get_test_location(void){
	
	return jsondata.test_lactiong;
}
int get_run_test_num(void){
	return	jsondata.run_test_num;
}
//int get_run_test_laction(void){
//	return	*jsondata.test_lactiong;
//}
int get_run_location(void){
	
	return jsondata.run_location;
}
