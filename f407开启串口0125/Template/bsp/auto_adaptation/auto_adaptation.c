#include "auto_adaptation.h"
#include "bsp_mx.sensor.h"
#include "bsp_master_serial.h"
#include "bsp_airsac.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef enum
{
	AIR_TOO_BIG,
	AIR_TOO_SMALL,
	AIR_NORMAL,
	AIR_NODE
}air_staus_enum;


struct 
{
	unsigned int down;
	unsigned int middle;
	unsigned int up;
}air_max_value;

struct
{
	unsigned int down;
	unsigned int middle;
	unsigned int up;
}air_min_value;

struct
{
		bool enable_flag;//使能标志
		mode_shit_enum mode;//强度挡位
		unsigned int run_tick;//运行时间
		unsigned char air_number_loop;//气囊号循环
		unsigned char air_tick[3];//三个气囊消抖
		air_staus_enum air_staus[3];//三个气囊状态
		lumbar_air_mode_enum air_out_mode[3];
}back_auto_adaptaiton;



sensor_data_struct g_back_sensor_data;
sensor_data_struct g_cushion_sensor_data;
sensor_data_struct g_16x16_sensor_data;

//

unsigned char seated_tick = 0;//入座检测消抖
unsigned char sitting_tick = 0;//坐姿识别消抖
unsigned char back_auto_tick = 0;//靠背消抖

unsigned int g_start_back__total_value = 0;//
unsigned short g_start_back_pressure_number = 0;



bool g_seated_flag = false;//入座标志
sitting_position_enum g_sitting_position = NONE_BACK;//坐姿
bool check_seated_enable_flag = true;




//入座检测
bool check_seated(sensor_data_struct* cushion_senor_data)
{
		if(cushion_senor_data->total_value[0] >= 800 && cushion_senor_data->pressure_number[0] >= 200)
			return true;
		else
			return false;
}

//坐姿检测
sitting_position_enum check_sitting_position(sensor_data_struct* cushion_sensor_data,sensor_data_struct * back_sensor_data)
{
		if(back_sensor_data->total_value[0] <= 100 || back_sensor_data->pressure_number[0] <= 20)
		{
				return NONE_BACK;
		}
		
		if(cushion_sensor_data->centre.y <= 10)
		{
				return T00_FAR_AHEAD;
		}
		else if(cushion_sensor_data->centre.y <= 15 && fabsf(cushion_sensor_data->centre.x - cushion_sensor_data->centroid.x) >= 0.5 
						&& fabsf(cushion_sensor_data->centre.y - cushion_sensor_data->centroid.y) >= 0.5 && abs((int)cushion_sensor_data->total_value[1] - (int)cushion_sensor_data->total_value[2]) >= 500
						&& abs((int)cushion_sensor_data->pressure_number[1] - (int)cushion_sensor_data->pressure_number[2]) >= 50)
		{
				return CROSS_LEGGED;
		}
		else if(fabsf(cushion_sensor_data->centre.x - (float)9.5) >= 5)
		{
				return UNEVENNESS;
		}
		else
		{
				
				return NORMAL_POSITION;
		}
		
}

void check_air_staus(sensor_data_struct* back_sensor_data, air_staus_enum * air_staus)
{
		unsigned int total_value_temp = g_back_sensor_data.total_value[0]/ 100;
	
		switch(back_auto_adaptaiton.mode)
		{
				case MODE_WEAK:
						air_min_value.down = 1*total_value_temp;
						air_max_value.down = air_min_value.down + 3*total_value_temp;
						air_min_value.middle = 12*total_value_temp;
						air_max_value.middle = air_min_value.middle + 4*total_value_temp;
						air_min_value.up = 19*total_value_temp;
						air_max_value.up = air_min_value.up + 4*total_value_temp;
						break;
				case MODE_MEDIUM:
						air_min_value.down = 3*total_value_temp;
						air_max_value.down = air_min_value.down + 4*total_value_temp;
						air_min_value.middle = 15*total_value_temp;
						air_max_value.middle = air_min_value.middle + 4*total_value_temp;
						air_min_value.up = 22*total_value_temp;
						air_max_value.up = air_min_value.up + 4*total_value_temp;
						break;
				case MODE_STRONG:
						air_min_value.down = 5*total_value_temp;
						air_max_value.down = air_min_value.down + 4*total_value_temp;
						air_min_value.middle = 16*total_value_temp;
						air_max_value.middle = air_min_value.middle + 4*total_value_temp;
						air_min_value.up = 23*total_value_temp;
						air_max_value.up = air_min_value.up + 4*total_value_temp;
						break;
				
		}
		
//		air_min_value.down =  2 * total_value_temp;
//		air_min_value.middle =13* total_value_temp;
//		air_min_value.up = 15 * total_value_temp;
//	
//		air_max_value.down = 6 *total_value_temp;
//		air_max_value.middle =17 * total_value_temp;
//		air_max_value.up = 19 * total_value_temp;
//	
		//下气囊
		if(g_back_sensor_data.total_value[1] <= air_min_value.down)
		{
				air_staus[0] = AIR_TOO_SMALL;
		}
		else if(g_back_sensor_data.total_value[1] >= air_max_value.down)
		{
				air_staus[0] = AIR_TOO_BIG;
		}
		else
		{
				air_staus[0] = AIR_NORMAL;
		}
		//中气囊
		if(g_back_sensor_data.total_value[2] <= air_min_value.middle)
		{
				air_staus[1] = AIR_TOO_SMALL;
		}
		else if(g_back_sensor_data.total_value[2] >= air_max_value.middle)
		{
				air_staus[1] = AIR_TOO_BIG;
		}
		else
		{
				air_staus[1] = AIR_NORMAL;
		}
		//上气囊
		if(g_back_sensor_data.total_value[3] <= air_min_value.up)
		{
				air_staus[2] = AIR_TOO_SMALL;
		}
		else if(g_back_sensor_data.total_value[3] >= air_max_value.up )
		{
				air_staus[2] = AIR_TOO_BIG;
		}
		else
		{
				air_staus[2] = AIR_NORMAL;
		}
}

void set_auto_adaptation_air_mode(air_staus_enum *air_staus)
{
		lumbar_air_mode_enum air_mode[3];
		
		if(air_staus[0] == AIR_TOO_BIG)
		{
			air_mode[0] = DOWN_DISCHARGE;
		}
		else if(air_staus[0] == AIR_TOO_SMALL)
		{
			air_mode[0] = DOWN_CHARGE;
		}
		else
		{
			air_mode[0] = POWER_ON;
		}
		
		if(air_staus[1] == AIR_TOO_BIG)
		{
			air_mode[1] = MIDDLE_DISCHARGE;
		}
		else if(air_staus[1] == AIR_TOO_SMALL)
		{
			air_mode[1] = MIDDLE_CHARGE;
		}
		else
		{
			air_mode[1] = POWER_ON;
		}
		
		if(air_staus[2] == AIR_TOO_BIG)
		{
			air_mode[2] = UP_DISCHARGE;
		}
		else if(air_staus[2] == AIR_TOO_SMALL)
		{
			air_mode[2] = UP_CHARGE;
		}
		else
		{
			air_mode[2] = POWER_ON;
		}
		
		
		for(unsigned char i = 0; i < 3; i++)
		{
				back_auto_adaptaiton.air_out_mode[i] = air_mode[i];
		}
		
		
		//set_airsac_loop(air_mode[0], air_mode[1], air_mode[2]);
		
}




void back_auto_adaptation_enable(bool enable_flag)
{
	if(enable_flag)
	{
			back_auto_adaptaiton.enable_flag = 1;
			set_airsac_keep_mode(POWER_ON);
			//stop_airsac_loop();
			back_auto_adaptaiton.run_tick = 0;
			back_auto_adaptaiton.air_number_loop = 0;
			for(unsigned char i = 0; i < 3; i++)
			{
					back_auto_adaptaiton.air_tick[i] = 0;
					back_auto_adaptaiton.air_staus[i] = AIR_NODE;
			}
	}
	else
	{
			back_auto_adaptaiton.enable_flag = 0;
			back_auto_adaptaiton.run_tick = 0;
			back_auto_adaptaiton.air_number_loop = 0;
			for(unsigned char i = 0; i < 3; i++)
			{
		 			back_auto_adaptaiton.air_tick[i] = 0;
					back_auto_adaptaiton.air_staus[i] = AIR_NODE;
			}
			//stop_airsac_loop();
			set_airsac_keep_mode(POWER_ON);
	}	
}

 bool check_auto_adaptation_enable_flag(void)
{
		return back_auto_adaptaiton.enable_flag;
}

void set_back_auto_adaptation_mode_shit(mode_shit_enum mode_shit)
{
		back_auto_adaptaiton.mode = mode_shit;
}

//100ms运行一次
void run_auto_adaptation(void)
{
		bool current_seated_flag = false;
		sitting_position_enum current_sitting_position = NONE_BACK;
		air_staus_enum current_air_staus[3];
		if(check_seated_enable_flag)
		{
				g_cushion_sensor_data.centroid = calculateCentroid((uint8_t*)g_cushion_sensor_data.buffer, AY_NUM, AX_NUM);
				g_cushion_sensor_data.centre = calculateCentre((uint8_t*)g_cushion_sensor_data.buffer, AY_NUM, AX_NUM);
				g_cushion_sensor_data.total_value[0] = calculateTotal((uint8_t*)g_cushion_sensor_data.buffer, 0, 0, AY_NUM, AX_NUM, AY_NUM, AX_NUM, &g_cushion_sensor_data.pressure_number[0]);
				g_cushion_sensor_data.total_value[1] = calculateTotal((uint8_t*)g_cushion_sensor_data.buffer, 0, 0, AY_NUM/2, AX_NUM/2, AY_NUM, AX_NUM, &g_cushion_sensor_data.pressure_number[1]);
				g_cushion_sensor_data.total_value[2] = calculateTotal((uint8_t*)g_cushion_sensor_data.buffer,  AY_NUM/2, 0, AY_NUM, AX_NUM/2, AY_NUM, AX_NUM, &g_cushion_sensor_data.pressure_number[2]);
				current_seated_flag = check_seated(&g_cushion_sensor_data);
				g_back_sensor_data.centroid = calculateCentroid((uint8_t*)g_back_sensor_data.buffer, AY_NUM, AX_NUM-8);
				g_back_sensor_data.centre = calculateCentre((uint8_t*)g_back_sensor_data.buffer, AY_NUM, AX_NUM-8);
				g_back_sensor_data.total_value[1] = calculateTotal((uint8_t *)g_back_sensor_data.buffer, 8, 3, AY_NUM-8, 7, AY_NUM, AX_NUM, &g_back_sensor_data.pressure_number[1]);
				g_back_sensor_data.total_value[2] = calculateTotal((uint8_t *)g_back_sensor_data.buffer, 8, 10, AY_NUM-8, 14, AY_NUM, AX_NUM, &g_back_sensor_data.pressure_number[2]);
				g_back_sensor_data.total_value[3] = calculateTotal((uint8_t *)g_back_sensor_data.buffer, 0, 15, AY_NUM-8, 19, AY_NUM, AX_NUM, &g_back_sensor_data.pressure_number[3]);
				if(current_seated_flag != g_seated_flag)
				{
						seated_tick++;
						if(seated_tick >= 30)
						{
								g_seated_flag = current_seated_flag;
								send_staus_pack(0x25, g_seated_flag);
								sitting_tick = 0;
								
						}
				}
				else
				{
						seated_tick = 0;
				}
				
				if(g_seated_flag == true)
				{
						
						g_back_sensor_data.total_value[0] = calculateTotal((uint8_t*)g_back_sensor_data.buffer, 0, 0, AY_NUM, AX_NUM, AY_NUM, AX_NUM, &g_back_sensor_data.pressure_number[0]);
						current_sitting_position = check_sitting_position(&g_cushion_sensor_data, &g_back_sensor_data);
						if(current_sitting_position != g_sitting_position)
						{
								sitting_tick++;
								if(sitting_tick >= 10)
								{
										g_sitting_position = current_sitting_position;
										send_staus_pack(0x26, g_sitting_position);
										sitting_tick = 0;
								}
						}
						else
						{
								sitting_tick = 0;
						}
						
				}
				else
				{
						sitting_tick = 0;
						g_sitting_position  = NONE_BACK;
				}
		}
		
		if(back_auto_adaptaiton.enable_flag)
		{	
				back_auto_adaptaiton.run_tick++;
				if(g_sitting_position == NORMAL_POSITION)
				{	
						check_air_staus(&g_back_sensor_data, current_air_staus);//检测气囊状态
						
						//消抖
						for(unsigned char i = 0; i < 3; i++)
						{
								if(back_auto_adaptaiton.air_staus[i] != current_air_staus[i])
								{
										back_auto_adaptaiton.air_tick[i]++;
										if(back_auto_adaptaiton.air_tick[i] >= 15)
										{
												back_auto_adaptaiton.air_staus[i] = current_air_staus[i];
												set_auto_adaptation_air_mode(back_auto_adaptaiton.air_staus);
										}
								}
								else
								{
										back_auto_adaptaiton.air_tick[i] = 0;
								}
						}
						
						//每1.5s切换一次气囊
						if(back_auto_adaptaiton.run_tick % 15 == 0)
						{
								unsigned char i = 3;
								lumbar_air_mode_enum air_mode_temp = POWER_ON;
								while(i--)
								{
										if(back_auto_adaptaiton.air_staus[back_auto_adaptaiton.air_number_loop] != AIR_NORMAL)//如果气囊不等于正常直接运行该气囊，否则切换下一个气囊
										{
												i = 0;
												air_mode_temp = back_auto_adaptaiton.air_out_mode[back_auto_adaptaiton.air_number_loop];
										}			
										back_auto_adaptaiton.air_number_loop = (back_auto_adaptaiton.air_number_loop + 1)%3;
								}
								
								set_airsac_keep_mode(air_mode_temp);
								
								
						}
						
						
						if((back_auto_adaptaiton.air_staus[0] == AIR_NORMAL || check_air_isfull(DOWN_AIR) == 1) &&
							(back_auto_adaptaiton.air_staus[1] == AIR_NORMAL || check_air_isfull(MIDDLE_AIR) == 1) &&
							(back_auto_adaptaiton.air_staus[2] == AIR_NORMAL || check_air_isfull(UP_AIR) == 1))
						{
								send_staus_pack(0x27, back_auto_adaptaiton.enable_flag);
								back_auto_adaptation_enable(false);
						}
							
				}
				else
				{
						back_auto_adaptation_enable(false);
						send_staus_pack(0x27, back_auto_adaptaiton.enable_flag);
				}
				
				if(back_auto_adaptaiton.run_tick >= 600)
				{
						back_auto_adaptaiton.run_tick = 0;
						back_auto_adaptation_enable(false);
						send_staus_pack(0x27, back_auto_adaptaiton.enable_flag);
				}
		}
		else
		{
//				if(airsac_start_flag)
//				{
//					stop_airsac_loop();
//					airsac_start_flag = 0;
//				}
				//set_airsac_loop(UP_DISCHARGE,MIDDLE_DISCHARGE,DOWN_DISCHARGE);
				
		}
						
		
}


