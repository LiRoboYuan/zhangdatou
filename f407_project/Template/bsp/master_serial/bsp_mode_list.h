#ifndef _BSP_MODE_LIST
#define _BSP_MODE_LIST
#include "gd32f4xx.h"
#include ""

typedef struct
{
	uint8_t set_moto_target_flag;//设置行程标志；
	int32_t moto_target_value;
	
	uint8_t adjust_moto_flag;//校准标志；
	int32_t moto_max_route_value;
	
	
	
	
}master_mode_struct;







#endif