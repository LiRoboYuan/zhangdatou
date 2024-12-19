#include "moto_control.h"
#include "height_weight.h"
#include "board.h"

uint8_t moto_thread_enable = 0;
uint8_t moto_start_flag[MOTO_MAX_NUM] = {0};
uint8_t height_weight_moto_finish_flag = 0x00;
int32_t g_moto_target[4] = {0}; 
int32_t max_route_value[MOTO_MAX_NUM] = {0,0,0,0};
int16_t moto_physics_value[MOTO_MAX_NUM] = {HEIGHT_PHYSICS_VALUE,FRONT_PHYSICS_VALUE,TILT_PHYSICS_VALUE,BACK_PHYSICS_VALUE};
adjust_mode_struct adjust_mode[MOTO_MAX_NUM] = {{0,ADJUST_MAX_ROUTE,NEGATIVE,40000,0,0},
																								{0,ADJUST_MAX_ROUTE,NEGATIVE,40000,0,0},
																								{0,ADJUST_MAX_ROUTE,NEGATIVE,40000,0,0},
																								{0,ADJUST_MAX_ROUTE,NEGATIVE,40000,0,0},
																								};

static Enum_State g_estate[MOTO_MAX_NUM] = {E_IDLE,E_IDLE,E_IDLE,E_IDLE};
static Enum_Event g_event[MOTO_MAX_NUM] = {IDLE_EVENT};

static void idle_funtion(MOTO_NUM moto_num);
static void switch_funtion(MOTO_NUM moto_num);
static void master_serial_funtion(MOTO_NUM moto_num);
static void adjust_funtion(MOTO_NUM moto_num);
static void height_weight_funtion(MOTO_NUM moto_num);

void(*moto_handle[])(MOTO_NUM moto_num) = {
	idle_funtion,
	switch_funtion,
	master_serial_funtion,
	adjust_funtion,
	height_weight_funtion
};
																																															
																								
void adjust_mode_init(void)
{
		adjust_mode[MOTO_1].zero_direction = NEGATIVE;
		adjust_mode[MOTO_2].zero_direction = NEGATIVE;
		adjust_mode[MOTO_3].zero_direction = NEGATIVE;
		adjust_mode[MOTO_4].zero_direction = NEGATIVE;
		adjust_mode[MOTO_1].target = 40000;
		adjust_mode[MOTO_2].target = 40000;
		adjust_mode[MOTO_3].target = 40000;
		adjust_mode[MOTO_4].target = 40000;
		
		adjust_mode[MOTO_1].state = ADJUST_MAX_ROUTE;
		adjust_mode[MOTO_2].state = ADJUST_MAX_ROUTE;
		adjust_mode[MOTO_3].state = ADJUST_MAX_ROUTE;
		adjust_mode[MOTO_4].state = ADJUST_MAX_ROUTE;
		
}

void moto_control_init(void)
{
		
}



void set_moto_thread_status(uint8_t enable)
{
    moto_thread_enable = enable;
    if (!enable)
    {
        set_moto_control(MOTO_4, 0);
        set_moto_control(MOTO_2, 0);
				set_moto_control(MOTO_3, 0);
        set_moto_control(MOTO_1, 0);
				
    }
}

void set_zero_direction(MOTO_NUM moto_num, zero_direction_enum zero_direction)
{
		adjust_mode[moto_num].zero_direction = zero_direction;
}


void set_all_zero_direction(void)
{
		adjust_mode[MOTO_1].zero_direction = NEGATIVE;
		adjust_mode[MOTO_2].zero_direction = NEGATIVE;
		adjust_mode[MOTO_3].zero_direction = NEGATIVE;
		adjust_mode[MOTO_4].zero_direction = NEGATIVE;
	
}

void set_moto_adjust_mode(MOTO_NUM moto_num)
{
		adjust_mode[moto_num].start_flag = 0x01;
}


int32_t get_max_route_value(MOTO_NUM moto_num)
{
		return max_route_value[moto_num];
}




void moto_PID_thread_run(void)
{
    if (moto_thread_enable)
    {
        motor_pid_4.f_cal_pid(&motor_pid_4, get_encoder_currentCount(MOTO_D));
        motor_pid_2.f_cal_pid(&motor_pid_2, get_encoder_currentCount(MOTO_B));
				motor_pid_3.f_cal_pid(&motor_pid_3, get_encoder_currentCount(MOTO_C));
				motor_pid_1.f_cal_pid(&motor_pid_1, get_encoder_currentCount(MOTO_A));
			
				
				
				if(check_moto_status(MOTO_4, motor_pid_4.output, get_encoder_currentCount(MOTO_D))!=RUN)
				{
						set_moto_control(MOTO_4, 0);
						//set_moto_target(MOTO_4,get_encoder_currentCount(MOTO_D));
						//moto_stop_flag[MOTO_4] = 1;
				}
				else 
				{
						set_moto_control(MOTO_4, motor_pid_4.output);
				}
				
				if(check_moto_status(MOTO_2, motor_pid_2.output, get_encoder_currentCount(MOTO_B))!=RUN)
				{
						
						set_moto_control(MOTO_2, 0);
						//set_moto_target(MOTO_2,get_encoder_currentCount(MOTO_B));
						//moto_stop_flag[MOTO_2] = 1;
				}
				else
						set_moto_control(MOTO_2, motor_pid_2.output);
				
				if(check_moto_status(MOTO_3, motor_pid_3.output, get_encoder_currentCount(MOTO_C))!=RUN)
				{
						set_moto_control(MOTO_3, 0);
						//set_moto_target(MOTO_3,get_encoder_currentCount(MOTO_C));
						//moto_stop_flag[MOTO_3] = 1;
				}
				else
						set_moto_control(MOTO_3, motor_pid_3.output);
				
				if(check_moto_status(MOTO_1, motor_pid_1.output, get_encoder_currentCount(MOTO_A))!=RUN)
				{
						set_moto_control(MOTO_1, 0);
						//set_moto_target(MOTO_1,get_encoder_currentCount(MOTO_A));
						//moto_stop_flag[MOTO_1] = 1;
				}
				else
						set_moto_control(MOTO_1, motor_pid_1.output);
				
  
        
    } else {
        set_moto_control(MOTO_4, 0);
        set_moto_control(MOTO_2, 0);
				set_moto_control(MOTO_3, 0);
        set_moto_control(MOTO_1, 0);
    }
}






//清除其他状态
void clear_other_event_status(Enum_Event event,MOTO_NUM moto_num)
{
	switch(event)
	{
		case SWITCH_EVENT: 
					height_weight_par.event_status = DISABLE;
					adjust_mode[moto_num].start_flag = 0;
					master_serial_moto_par[moto_num].set_moto_flag = 0;
					break;
		case MASTER_EVENT: 
					height_weight_par.event_status = DISABLE;
					adjust_mode[moto_num].start_flag = 0;
					break;
		case ADJUST_EVENT:
					height_weight_par.event_status = DISABLE;
					break;
		case HEIGHT_WEIGHT_EVENT:
					break;
		case	IDLE_EVENT:
					height_weight_par.event_status = DISABLE;
					adjust_mode[moto_num].start_flag = 0;
					master_serial_moto_par[moto_num].set_moto_flag = 0;
					break;
		default:break;
	}
}

//检测事件
void check_event(void)
{
	uint8_t height_weight_mask = 0;
	for(MOTO_NUM moto_num = MOTO_1; moto_num < MOTO_MAX_NUM; moto_num++)
	{
		if(switch_moto_par[moto_num].switch_down_flag)
		{
				g_event[moto_num] = SWITCH_EVENT;
		}
		else if(master_serial_moto_par[moto_num].stop_flag)
		{
				g_event[moto_num] = IDLE_EVENT;
				master_serial_moto_par[moto_num].stop_flag = 0;
		}
		else if(master_serial_moto_par[moto_num].set_moto_flag)
		{
				g_event[moto_num] = MASTER_EVENT;
		}
		else if(adjust_mode[moto_num].start_flag)
		{
				g_event[moto_num] = ADJUST_EVENT;
		}
		else if(height_weight_par.event_status)
		{
				g_event[moto_num] = HEIGHT_WEIGHT_EVENT;
				height_weight_mask++;
		}
		else
		{
				g_event[moto_num] = IDLE_EVENT;
		}
	}
	
	if(height_weight_mask < 4 && height_weight_mask > 0)
	{
			for(MOTO_NUM moto_num = MOTO_1; moto_num < MOTO_MAX_NUM; moto_num++)
			{
					if(g_event[moto_num] == HEIGHT_WEIGHT_EVENT)
							g_event[moto_num] = IDLE_EVENT;
			}
	}
	
	for(MOTO_NUM moto_num = MOTO_1; moto_num < MOTO_MAX_NUM; moto_num++)
	{
			clear_other_event_status(g_event[moto_num],moto_num);
	}
}



//空闲时的函数
void idle_funtion(MOTO_NUM moto_num)
{
		Enum_Event event = IDLE_EVENT;
		event = g_event[moto_num];
		if(event > 0)
		{
				moto_start_flag[moto_num] = 1;
		}	
		
		switch((uint8_t)event)
		{
				case SWITCH_EVENT:
							g_moto_target[moto_num] = switch_moto_par[moto_num].switch_target;
							break;
				case MASTER_EVENT:
							g_moto_target[moto_num] = master_serial_moto_par[moto_num].target;
							break;
				case ADJUST_EVENT:
							g_moto_target[moto_num] = adjust_mode[moto_num].target;
							break;
				case HEIGHT_WEIGHT_EVENT:
							g_moto_target[moto_num] = height_weight_par.target_out[moto_num];
							break;
				case IDLE_EVENT:
							g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
							break;
				default:
							g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
		}
		
		g_estate[moto_num] = (uint8_t)event;
}


//按键事件函数
void switch_funtion(MOTO_NUM moto_num)
{
		Enum_Event event = IDLE_EVENT;
		event = g_event[moto_num];
		static uint64_t current_tick[MOTO_MAX_NUM] = {0};
		static uint8_t send_stop_flag[MOTO_MAX_NUM] = {0};
		
		if(event != SWITCH_EVENT)
		{
				g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
				g_estate[moto_num] = E_IDLE;
		}
		else
		{
				if(get_moto_run_status(moto_num) != RUN)
				{
						g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
						if(send_stop_flag[moto_num] == 0)
						{
							send_moto_position_pack(moto_num,get_encoder_currentCount((encoder_num_enum)moto_num));
							send_stop_flag[moto_num] = 1;
						}
						
				}
				else
				{
						if(get_system_tick()-current_tick[moto_num] >= 500)
						{
								current_tick[moto_num] = get_system_tick();
								send_moto_position_pack(moto_num,get_encoder_currentCount((encoder_num_enum)moto_num));
						}
						g_moto_target[moto_num] = switch_moto_par[moto_num].switch_target;
						send_stop_flag[moto_num] = 0;
				}
		}
	
}

//主串口函数
void master_serial_funtion(MOTO_NUM moto_num)
{
		Enum_Event event = IDLE_EVENT;
		event = g_event[moto_num];
		int16_t current_encoder = 0;
		if(event != MASTER_EVENT)
		{
				send_err_pack();
				g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
				g_estate[moto_num] = E_IDLE;
				
		}
		else
		{
				if(get_moto_run_status(moto_num) != RUN)
				{
						current_encoder = get_encoder_currentCount((encoder_num_enum)moto_num);
						if(master_serial_moto_par[moto_num].set_moto_flag == 1)
							send_encoder_current_pack(moto_num,current_encoder);
						else if(master_serial_moto_par[moto_num].set_moto_flag == 2)
						{
							send_moto_position_pack(moto_num,current_encoder);
						}
				
						g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
						g_estate[moto_num] = E_IDLE;
						master_serial_moto_par[moto_num].set_moto_flag = 0;
						
				}
				else
				{
						g_moto_target[moto_num] = master_serial_moto_par[moto_num].target;
				}
		}
}


//校验函数
void adjust_funtion(MOTO_NUM moto_num)
{
		Enum_Event event = IDLE_EVENT;
		event = g_event[moto_num];
		if(event != ADJUST_EVENT)
		{
				send_moto_staus_pack(0x05, moto_num, 0);
				g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
				adjust_mode[moto_num].state = ADJUST_MAX_ROUTE;
				g_estate[moto_num] = E_IDLE;
		}
		else
		{
				if(get_moto_run_status(moto_num) != RUN)
				{
						if(adjust_mode[moto_num].state == ADJUST_MAX_ROUTE)
						{
								adjust_mode[moto_num].state = ADJUST_ZERO;
								adjust_mode[moto_num].max_route_value = get_encoder_currentCount((encoder_num_enum)moto_num);
								moto_start_flag[moto_num] = 1;
								g_moto_target[moto_num] = -adjust_mode[moto_num].target;
						}
						else
						{
								adjust_mode[moto_num].state = ADJUST_MAX_ROUTE;
								adjust_mode[moto_num].zero_value = get_encoder_currentCount((encoder_num_enum)moto_num);
								max_route_value[moto_num] = adjust_mode[moto_num].max_route_value - adjust_mode[moto_num].zero_value;
								lock_encoder_currentCount((encoder_num_enum)moto_num);
								set_encoder_currentCount((encoder_num_enum)moto_num,0);
								unlock_encoder_currentCount((encoder_num_enum)moto_num);
								adjust_mode[moto_num].start_flag =  0;
								send_moto_staus_pack(0x05, moto_num, 1);
								g_estate[moto_num] = E_IDLE;
						}
				}
				else
				{
						if(adjust_mode[moto_num].state == ADJUST_MAX_ROUTE)
						{
								g_moto_target[moto_num] = adjust_mode[moto_num].target;
						}
						else
						{
								g_moto_target[moto_num] = -adjust_mode[moto_num].target;
						}
				}
		}
}

//身高体重适应函数
void height_weight_funtion(MOTO_NUM moto_num)
{
		Enum_Event event = IDLE_EVENT;
		event = g_event[moto_num];
		if(event != HEIGHT_WEIGHT_EVENT)
		{
				send_err_pack();
				g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
				g_estate[moto_num] = E_IDLE;
		}
		else
		{
				if(get_moto_run_status(moto_num) != RUN)
				{
						height_weight_moto_finish_flag |= 0x01<<moto_num;
						g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
						
				}
				else
				{
						g_moto_target[moto_num] = height_weight_par.target_out[moto_num];
				}
				
				if(height_weight_moto_finish_flag == 0x0f)
				{
						send_encoder_current_pack(moto_num,get_encoder_currentCount((encoder_num_enum)moto_num));
						height_weight_moto_finish_flag = 0;
						height_weight_par.event_status = DISABLE;
						g_moto_target[moto_num] = get_encoder_currentCount((encoder_num_enum)moto_num);
						g_estate[moto_num] = E_IDLE;
				}
		}
}


//电机运行
void moto_run(void)
{
	
	check_event();

	for(MOTO_NUM moto_num = MOTO_1; moto_num < MOTO_MAX_NUM; moto_num++)
	{
			moto_handle[g_estate[moto_num]](moto_num);
			
			if(moto_start_flag[moto_num])
			{
					clean_moto_status(moto_num);
					set_moto_target(moto_num,g_moto_target[moto_num]);
					moto_start_flag[moto_num] = 0;
			}
			else
			{
					set_moto_target(moto_num,g_moto_target[moto_num]);
			}
	}

	moto_PID_thread_run();
}