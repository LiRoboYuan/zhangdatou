#ifndef __GETJSON_H__
#define __GETJSON_H__
#include "board.h"

//{
//    "motor_mode":"run/off/on/test",0,1,2,3
//    "location_num":"num"
//    "run_location": "int"
//    "test_location": {
//        "1": "int"
//        "2": "int"
//        "3": "int"
//        "4": "int"
//        "5": "int"
//        "6": "int"
//        "7": "int"
//        "8": "int"

//    }
//}

typedef struct
{
	int motor_mode;
	int relay_flag;
	int location_num;
	int run_location;
	int *test_lacation;
}json_data_t;

int get_Json_data(void);
int cJSON_demo(void);
void aid_paser_json_demo(uint8_t *json_buffer,
        uint16_t json_size);
#endif

