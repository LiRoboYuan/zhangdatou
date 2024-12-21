#ifndef _AUTO_ADAPTATION_H
#define _AUTO_ADAPTATION_H
#include "board.h"
#include "bsp_mx.sensor.h"
#include "auto_adaptation_math.h"
#include <stdbool.h>

typedef struct
{
		uint8_t buffer[AX_NUM][AY_NUM];
		PointF centroid;
		PointF centre;
		unsigned int total_value[4];
		unsigned short pressure_number[4];
}sensor_data_struct;

typedef enum
{
		NORMAL_POSITION,//正常
		T00_FAR_AHEAD,//太靠前
		UNEVENNESS,//左右不均
		CROSS_LEGGED,//二郎腿
		NONE_BACK//没有靠背
}sitting_position_enum;

typedef enum
{
	MODE_WEAK,
	MODE_MEDIUM,
	MODE_STRONG
}mode_shit_enum;


//typedef enum
//{
//	LOIN_TOO_BIG,
//	LOIN_TOO_SMALL,
//	BACK_TOO_BIG
//	 	
//}back_staus_enum;


extern sensor_data_struct g_back_sensor_data;
extern sensor_data_struct g_cushion_sensor_data;
extern sensor_data_struct g_16x16_sensor_data;
extern bool g_seated_flag;//入座标志
extern sitting_position_enum g_sitting_position;//坐姿

extern unsigned int g_start_back__total_value;
extern unsigned short g_start_back_pressure_number;

void run_auto_adaptation(void);
void back_auto_adaptation_enable(bool enable_flag);
void set_back_auto_adaptation_mode_shit(mode_shit_enum mode_shit);
bool check_auto_adaptation_enable_flag(void);

#endif