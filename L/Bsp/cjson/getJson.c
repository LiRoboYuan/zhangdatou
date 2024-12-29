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
cJSON *Json_send = NULL; // 全局变量

int iwaaa=0;
int cJSON_demo(void){
	uint16_t data_count = 0;
	uint16_t read_size = 0;
	int result = 0;  // 0 表示成功，其他值表示错误
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
	int result = -1;  // 1 表示成功，其他值表示错误
	uint16_t get_size = 0;
	uint16_t uart_get_num = usart_get_rx_data_count(USART_0_TR);

	if(uart_get_num > 0){
		get_size = usart_recv(USART_0_TR, uart_rx_buffer, uart_get_num);
//		printf("\n uart_get_num %d",uart_get_num);
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
					break;//移动平台
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
					jsondata.test_lactiong = (int *)mymalloc(0,position_count * sizeof(int));
					for (int i = 0; i < position_count; i++) {
						cJSON *item = cJSON_GetArrayItem(position, i);
						if (cJSON_IsNumber(item)) {
								// 将每个数字存储到 position_array 数组中
								jsondata.test_lactiong[i] = item->valueint;
								printf("position[%d] = %d\n", i, jsondata.test_lactiong[i]);
						} else {
								printf("Error: 'position' item %d is not a number.\n", i);
						}
					}
					
					cJSON *pressure = cJSON_GetObjectItem(test_point_total, "pressure");
					jsondata.run_test_pressure_num = cJSON_GetArraySize(pressure);
					jsondata.test_pressure = (int *)mymalloc(0,jsondata.run_test_pressure_num * sizeof(int));
					for(int i = 0; i < jsondata.run_test_pressure_num; i++) {
						cJSON * pre = cJSON_GetArrayItem(pressure, i);
						if (cJSON_IsNumber(pre)) {
							jsondata.test_pressure[i] = pre->valueint;
							printf("test_pressure[%d] = %d\n", i, jsondata.test_pressure[i]);
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
//						// 构造键名
//						snprintf(key, sizeof(key), "test_location%d", i + 1);
//						// 获取对应的 JSON 对象
//						cJSON *location_item = cJSON_GetObjectItem(test_location, key);
//						if (location_item && cJSON_IsString(location_item)) {
//								// 将字符串转换为整数并存储
//								jsondata.test_lactiong[i] = atoi(location_item->valuestring);
//						} else {
//								printf("Key %s not found or is not a valid string!\n", key);
//								jsondata.test_lactiong[i] = 0; // 默认值为 0
//						}
//					}	
//					break;//启动连续测试
			}
			// 清理内存
			cJSON_Delete(cjson);
		} 
		else {
				printf("\nError: JSON parsing failed  %d\n",get_size);
		}
	}
	return result;
}

void clean_test_location(void){
	myfree(1,jsondata.test_lactiong);
	myfree(1,jsondata.test_pressure);
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
int get_run_test_pressure_num(){
	return jsondata.run_test_pressure_num;
}

void sendJsonTask(int Mode,int test_now,int press,uint32_t resis){
	
	if(Mode == 0){
		if (Json_send != NULL) {
            cJSON_Delete(Json_send);  // 先清空之前的 root 对象
    }
		Json_send = cJSON_CreateObject();
		
		for(int i=0;i < jsondata.run_test_num;i++){
			 char test_name[10];
			 snprintf(test_name, sizeof(test_name), "test%d", i+1);
			 // 为每个测试项创建一个子对象
			 cJSON *test_obj = cJSON_CreateObject();
			 // 向每个 test 对象添加空数组 pressure 和 resis
			 // 创建 pressure 和 resis 数组，并初始化为包含 n 个 0
		 	 cJSON *pressure_array = cJSON_CreateArray();
			 cJSON *resis_array = cJSON_CreateArray();
			
			// 向 pressure_array 和 resis_array 数组添加 n 个 0
			 for (int i = 0; i < get_run_test_pressure_num(); i++) {
					cJSON_AddItemToArray(pressure_array, cJSON_CreateNumber(0));  // 添加 0 到 pressure 数组
					cJSON_AddItemToArray(resis_array, cJSON_CreateNumber(0));     // 添加 0 到 resis 数组
			 }
			
			// 将数组添加到 test_obj 对象中
			 cJSON_AddItemToObject(test_obj, "pressure", pressure_array);
			 cJSON_AddItemToObject(test_obj, "resis", resis_array);
    
			 // 将这个 test 对象添加到根对象中，键名为 testX
			 cJSON_AddItemToObject(Json_send, test_name, test_obj);
		}
		return;
	}
	if(Mode == 10){
		char *json_string = cJSON_PrintUnformatted(Json_send);
		// 通过串口发送数据
		if (json_string != NULL) {
			printf("%s", json_string);  // 使用 printf 输出 JSON 字符串
			cJSON_free(json_string);    // 释放由 cJSON_PrintUnformatted() 分配的内存
		}
		cJSON_Delete(Json_send);  // 释放 root 对象及其所有子对象的内存
		Json_send = NULL;         // 显式设置 Json_send 为 NULL
		return;
	}
	
	if (Json_send == NULL) {
		return;
	}
	else if(Mode == 1) {
		char test_name2[50];
		snprintf(test_name2, sizeof(test_name2), "test%d", test_now);
//		printf("test_name: %s\n", test_name2); 
		cJSON *test_obj = cJSON_GetObjectItem(Json_send, test_name2);
		// 获取 test1 下的 pressure 数组
		
		cJSON *pressure_array = cJSON_GetObjectItem(test_obj, "pressure");
		cJSON *resis_array = cJSON_GetObjectItem(test_obj, "resis");	
//		if(test_obj == NULL){
//			printf("test_obj == NULL");
//		}
//		if(pressure_array == NULL){
//			printf("pressure_array == NULL");
//		}
		
		for(int i=0;i<get_run_test_pressure_num();i++){
			cJSON *item_use = cJSON_GetArrayItem(pressure_array, i);
			cJSON *item_use2 = cJSON_GetArrayItem(resis_array, i);
			if(press == jsondata.test_pressure[i] && item_use->valuedouble == 0 && item_use2->valuedouble == 0){
				// 创建新的数据项
//				printf("%d\n",jsondata.test_pressure[i]);
				cJSON *new_press = cJSON_CreateNumber(jsondata.test_pressure[i]);
				printf("%u\n",resis);
				cJSON_DeleteItemFromArray(pressure_array, i);
				cJSON_InsertItemInArray(pressure_array, i, new_press);
				
				cJSON *new_resis = cJSON_CreateNumber(resis);
				cJSON_DeleteItemFromArray(resis_array, i);
				cJSON_InsertItemInArray(resis_array, i, new_resis);
				
			}
		}
	}	
}
