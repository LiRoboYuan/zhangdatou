#ifndef _BSP_MODE_LIST
#define _BSP_MODE_LIST
#include "gd32f4xx.h"
#include ""

typedef struct
{
	uint8_t set_moto_target_flag;//�����г̱�־��
	int32_t moto_target_value;
	
	uint8_t adjust_moto_flag;//У׼��־��
	int32_t moto_max_route_value;
	
	
	
	
}master_mode_struct;







#endif