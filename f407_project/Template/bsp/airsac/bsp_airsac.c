#include "bsp_airsac.h"
#include "board.h"
#include "string.h"
#include "bsp_switch.h"
#include "stdbool.h"

lumbar_air_mode_enum g_airsac_mode = POWER_ON;
flank_air_mode_enum flank_airsac_mode = FLANK_INIT;

uint8_t flank_mode = 0;

typedef enum
{
		KEEP_MODE,
		LOOP_MODE,
}air_pattern_enum;

air_pattern_enum g_air_pattern = KEEP_MODE;

struct
{
		uint8_t airsac_mode;
		bool init_flag;
}keep_airsac_mode,flank_air_keep_mode;

struct
{
	lumbar_air_mode_enum airsac_mode[4];
	uint8_t loop_tick;
	uint8_t loop;
}loop_airsac_mode;


lin_pack_stu lin_pack = {0};
lin_pack_stu lin_pack2 = {0};

uint8_t up_charge_tick = 0;
uint8_t middle_charge_tick = 0;
uint8_t down_charge_tick = 0;

static const uint8_t air_mode_data[LUMBAR_AIR_MODE_MAX_NUM][LIN_DATA_SIZE] = {
    0X80,0X00,0XFC,0XA4,0X00,0X10,0XFF,//POWER_ON
    0X80,0X00,0XFC,0XA4,0X00,0X14,0XFF,//UP_CHARGE
    0X80,0X00,0XFC,0XA4,0X00,0X18,0XFF,//DOWN_CHARGE
    0X80,0X00,0XFC,0XA4,0X00,0X11,0XFF,//MIDDLE_CHARGE
		0x80,0x00,0xFC,0xA4,0x00,0x50,0xFF,//UP_disCHARGE
		0x80,0x00,0xFC,0xA4,0x00,0x90,0xFF,//MIDDLE_DISCHARGE
		0X80,0X00,0XFC,0XA4,0X00,0XD0,0XFF,//DOWN_DISCHARGE
		0X80,0X00,0XFC,0XA4,0X00,0X12,0XFF,//ALL_CHARGE
		0X80,0X00,0XFC,0XA4,0X00,0X12,0XFF,//ALL_DISCHARGE
		0XC0,0X00,0XFC,0XA4,0X00,0X10,0XFF,//经典模式强度1
		0XC1,0X00,0XFC,0XA4,0X00,0X10,0XFF,//经典模式强度2
		0XC2,0X00,0XFC,0XA4,0X00,0X10,0XFF,//经典模式强度3
		0XC4,0X00,0XFC,0XA4,0X00,0X10,0XFF,//标准模式强度1
		0XC5,0X00,0XFC,0XA4,0X00,0X10,0XFF,//标准模式强度2
		0XC6,0X00,0XFC,0XA4,0X00,0X10,0XFF,//标准模式强度3
		0XC8,0X00,0XFC,0XA4,0X00,0X10,0XFF,//松弛模式强度1
		0XC9,0X00,0XFC,0XA4,0X00,0X10,0XFF,//松弛模式强度2
		0XCA,0X00,0XFC,0XA4,0X00,0X10,0XFF,//松弛模式强度3
		0X80,0X00,0XFC,0XA4,0X00,0X10,0XFF,//退出按摩模式
		
};

static const uint8_t air_mode_data2[FLANK_AIR_MODE_MAX_NUM][LIN_DATA_SIZE] = {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,//INIT
		0x01,0x00,0x00,0x00,0x00,0x00,0x00,//LEFT_0%
		0x05,0x00,0x00,0x00,0x00,0x00,0x00,//LEFT_20%
		0x09,0x00,0x00,0x00,0x00,0x00,0x00,//LEFT_40&
		0x0D,0x00,0x00,0x00,0x00,0x00,0x00,//LEFT_60%
		0x11,0x00,0x00,0x00,0x00,0x00,0x00,//LEFT_80%
		0x15,0x00,0x00,0x00,0x00,0x00,0x00,//LEFT_100%
		0x02,0x00,0x00,0x00,0x00,0x00,0x00,//RIGHT_0%
		0x06,0x00,0x00,0x00,0x00,0x00,0x00,//RIGHT_20%
		0x0A,0x00,0x00,0x00,0x00,0x00,0x00,//RIGHT_40%
		0x0E,0x00,0x00,0x00,0x00,0x00,0x00,//RIGHT_60%
		0x12,0x00,0x00,0x00,0x00,0x00,0x00,//RIGHT_80%
		0x16,0x00,0x00,0x00,0x00,0x00,0x00,//RIGHT_100%
		0x03,0x00,0x00,0x00,0x00,0x00,0x00,//TWO_0%
		0x07,0x00,0x00,0x00,0x00,0x00,0x00,//TWO_20%
		0x0B,0x00,0x00,0x00,0x00,0x00,0x00,//TWO_40%
		0x0F,0x00,0x00,0x00,0x00,0x00,0x00,//TWO_60%
		0x13,0x00,0x00,0x00,0x00,0x00,0x00,//TWO_80%
		0x17,0x00,0x00,0x00,0x00,0x00,0x00,//TWO_100%
};



void airsac_thread_init(void)
{
    lin_pack.sync = 0x55;
		lin_pack2.sync = 0x55;
    lin_pack.pid 	= lin_pid_calculate(0x03);
		lin_pack2.pid = lin_pid_calculate(0x02);

    lin_init();
		
		//set_airsac_loop(UP_CHARGE,MIDDLE_CHARGE,DOWN_CHARGE);
}



void set_lumbar_airsac_mode(lumbar_air_mode_enum mode)
{
		lin_pack_stu * lin_pack_temp = &lin_pack;
    if(mode < LUMBAR_AIR_MODE_MAX_NUM)
    {
				memcpy(lin_pack_temp->data, &air_mode_data[mode], LIN_DATA_SIZE);
				lin_pack_temp->checksum = lin_checksum_calculate(lin_pack_temp->pid, lin_pack_temp->data, LIN_DATA_SIZE);
				lin_send_pack(lin_pack_temp);
    }
}


void set_flank_airsac_mode(flank_air_mode_enum mode)
{
		lin_pack_stu * lin_pack_temp = &lin_pack2;
		if(mode < FLANK_AIR_MODE_MAX_NUM)
		{
				memcpy(lin_pack_temp->data, &air_mode_data2[mode], LIN_DATA_SIZE);
				lin_pack_temp->checksum = lin_checksum_calculate(lin_pack_temp->pid, lin_pack_temp->data, LIN_DATA_SIZE);
				lin_send_pack(lin_pack_temp);
		}
}


void set_flank_airsac_keep_mode(flank_air_mode_enum mode)
{
		if(mode < FLANK_AIR_MODE_MAX_NUM)
		{
				flank_air_keep_mode.airsac_mode = mode;
				flank_air_keep_mode.init_flag = 1;
		}
}

void set_airsac_keep_mode(lumbar_air_mode_enum mode)
{
		if(mode < LUMBAR_AIR_MODE_MAX_NUM)
		{	
			if(mode == ALL_DISCHARGE)
			{
				set_airsac_loop(UP_DISCHARGE,MIDDLE_DISCHARGE,DOWN_DISCHARGE);
			}
			else
			{
				keep_airsac_mode.airsac_mode = mode;
				keep_airsac_mode.init_flag = 1;
				g_air_pattern = KEEP_MODE;
			}
		}
}


void set_airsac_loop(lumbar_air_mode_enum air_mode0, lumbar_air_mode_enum air_mode1, lumbar_air_mode_enum air_mode2)
{
		g_air_pattern = LOOP_MODE;
		loop_airsac_mode.airsac_mode[0] = air_mode0;
		loop_airsac_mode.airsac_mode[1] = air_mode1;
		loop_airsac_mode.airsac_mode[2] = air_mode2;
}

uint8_t check_air_isfull(uint8_t air_number)
{
	uint8_t rev = 0;
	switch(air_number)
	{
		case DOWN_AIR:
				if(down_charge_tick >= 200)
					rev = 1;
				break;
		case MIDDLE_AIR:
				if(middle_charge_tick >= 200)
					rev = 1;
				break;
		case UP_AIR:
				if(up_charge_tick >= 200)
					rev = 1;
				break;
	}
	return rev;
	
}

void stop_airsac_loop(void)
{
	
		g_air_pattern  = KEEP_MODE;
		keep_airsac_mode.airsac_mode = POWER_ON;
		loop_airsac_mode.loop = 0;
		loop_airsac_mode.loop_tick = 0;
		loop_airsac_mode.airsac_mode[0] = POWER_ON;
		loop_airsac_mode.airsac_mode[1] = POWER_ON;
		loop_airsac_mode.airsac_mode[2] = POWER_ON;
}

lumbar_air_mode_enum out_airsac_mode = POWER_ON;



void run_lumber_airsac_mode(void)
{
		if(g_air_pattern == KEEP_MODE)
	{	
			if(keep_airsac_mode.init_flag)
			{
				g_airsac_mode = POWER_ON;
				keep_airsac_mode.init_flag = 0;
			}
			else
			{
				g_airsac_mode = keep_airsac_mode.airsac_mode;
			}
	}
	
	
	if(g_air_pattern == LOOP_MODE)
	{
			loop_airsac_mode.loop_tick++;
			if(loop_airsac_mode.loop_tick < 3)
			{
					g_airsac_mode = POWER_ON;
			}
		  else if(loop_airsac_mode.loop_tick < 20)
			{
					g_airsac_mode = loop_airsac_mode.airsac_mode[loop_airsac_mode.loop];
					
			}
			else
			{
					loop_airsac_mode.loop = (loop_airsac_mode.loop + 1)%3; 
					loop_airsac_mode.loop_tick = 0;
			}
				
	}
	else
	{
			loop_airsac_mode.loop_tick = 0;
			loop_airsac_mode.loop = 0;
	}
	
	out_airsac_mode = g_airsac_mode;
	
	if(g_airsac_mode == UP_CHARGE)
	{
			up_charge_tick++;
			if(up_charge_tick >= 200)
			{
				up_charge_tick = 200;
				out_airsac_mode = POWER_ON;
			}
	}
	else if(g_airsac_mode == UP_DISCHARGE)
	{
			up_charge_tick = 0;
	}
	
	if(g_airsac_mode == MIDDLE_CHARGE)
	{
			middle_charge_tick++;
			if(middle_charge_tick >= 200)
			{
				middle_charge_tick = 200;
				out_airsac_mode = POWER_ON;
			}
	}
	else if(g_airsac_mode == MIDDLE_DISCHARGE)
	{
			middle_charge_tick = 0;
	}
	
	if(g_airsac_mode == DOWN_CHARGE)
	{
			down_charge_tick++;
			if(down_charge_tick >= 200)
			{
				down_charge_tick = 200;
				out_airsac_mode = POWER_ON;
			}
	}
	else if(g_airsac_mode == DOWN_DISCHARGE)
	{
			down_charge_tick = 0;
	}
	

	
	
	set_lumbar_airsac_mode(out_airsac_mode);
}

void run_flank_air_mode(void)
{
	
		if(flank_air_keep_mode.init_flag)
		{
				flank_airsac_mode = FLANK_INIT;
				flank_air_keep_mode.init_flag = 0;
		}
		else
		{
				flank_airsac_mode = flank_air_keep_mode.airsac_mode;
		}
		
		set_flank_airsac_mode(flank_airsac_mode);
}


void set_flank_mode(void)
{
	static uint8_t trickle_charge_cnt = 0;
	static uint8_t flank_mode_temp = 0;
	
	if(flank_mode)
	{
	
		if(flank_mode != flank_mode_temp)
		{
			trickle_charge_cnt = 0;
			flank_mode_temp = flank_mode;
		}
		else
		{
			trickle_charge_cnt ++;
		}
	}
	else
	{
		trickle_charge_cnt = 0;
		flank_mode_temp = 0;
	}
	
	switch(flank_mode)
	{
		case 1:
			if(trickle_charge_cnt <= 1 )
			{
				set_flank_airsac_keep_mode(LEFT_20);
			}
			else if(trickle_charge_cnt == 20)
			{
				set_flank_airsac_keep_mode(LEFT_60);
			}
			else if(trickle_charge_cnt == 40)
			{
				set_flank_airsac_keep_mode(LEFT_100);
			}
			else if(trickle_charge_cnt == 60)
			{
				trickle_charge_cnt = 0;
				flank_mode = 0;
			}
			break;
		case 2:
			if(trickle_charge_cnt <= 1)
			{
				set_flank_airsac_keep_mode(RIGHT_20);
			}
			else if(trickle_charge_cnt == 20)
			{
				set_flank_airsac_keep_mode(RIGHT_60);
			}
			else if(trickle_charge_cnt == 40)
			{
				set_flank_airsac_keep_mode(RIGHT_100);
			}
			else if(trickle_charge_cnt == 60)
			{
				trickle_charge_cnt = 0;
				flank_mode = 0;
			}
			break;
		case 3:
			if(trickle_charge_cnt <= 1)
			{
				set_flank_airsac_keep_mode(TWO_20);
			}
			else if(trickle_charge_cnt == 20)
			{
				set_flank_airsac_keep_mode(TWO_60);
			}
			else if(trickle_charge_cnt == 40)
			{
				set_flank_airsac_keep_mode(TWO_100);
			}
			else if(trickle_charge_cnt == 60)
			{
				trickle_charge_cnt = 0;
				flank_mode = 0;
			}
			break;
		default :
			trickle_charge_cnt = 0;
			flank_mode = 0;
			break;
		
	}
	
}

//可以每100ms运行一次
void airsac_thread_run(void)
{
	
	
//    uint16_t key_value = get_switch_value(SWITCH_4);

//    if (key_value > 200)
//		{
//			keep_airsac_mode = POWER_ON;
//		}

//    if (key_value > 4000)   set_lumbar_airsac_mode(UP_CHARGE);
//    else if (key_value > 3400) set_lumbar_airsac_mode(DOWN_CHARGE);
//    else if (key_value > 2600) set_lumbar_airsac_mode(MIDDLE_CHARGE);
//    else if (key_value > 1900) set_lumbar_airsac_mode(ALL_DISCHARGE);
//		else if(keep_airsac_mode != POWER_ON)	set_lumbar_airsac_mode(keep_airsac_mode);
//		else set_lumbar_airsac_mode(POWER_ON);
	
	
//	if(keep_time_airsac_mode.enable_flag == true)
//	{
//			if(keep_time_airsac_mode.time_ms--)
//			{
//					g_airsac_mode = keep_time_airsac_mode.airsac_mode;
//			}
//			else
//			{
//					keep_time_airsac_mode.enable_flag = false;
//					g_airsac_mode = POWER_ON;
//					keep_time_airsac_mode.airsac_mode = POWER_ON;
//			}
//	}

	set_flank_mode();
	run_lumber_airsac_mode();
	run_flank_air_mode();
	
	

}



