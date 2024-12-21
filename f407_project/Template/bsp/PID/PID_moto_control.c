#include "PID_moto_control.h"
#include "bsp_encoder.h"


PID_TypeDef motor_pid_4;
PID_TypeDef motor_pid_2;
PID_TypeDef motor_pid_3;
PID_TypeDef motor_pid_1;

//int32_t moto_max_target[4] = {50000,50000,50000,50000};
void PID_thread_init(void)
{
    pid_init(&motor_pid_4);
    pid_init(&motor_pid_2);
		pid_init(&motor_pid_3);
		pid_init(&motor_pid_1);
    motor_pid_4.f_param_init(&motor_pid_4,
            PID_Position,
            800,
						150,
            150,
            0,
            0,
            0,
            0,
            70,
            0,
            0);
    motor_pid_2.f_param_init(&motor_pid_2,
            PID_Position,
            800,
						150,
            150,
            0,
            0,
            0,
            0,
            70,
            0,
            0);
		motor_pid_3.f_param_init(&motor_pid_3,
						PID_Position,
							800,
							150,
							150,
							0,
							0,
							0,
							0,
							70,
							0,
							0);
		motor_pid_1.f_param_init(&motor_pid_1,
							PID_Position,
								800,
								150,
								150,
								0,
								0,
								0,
								0,
								70,
								0,
								0);
}



void set_moto_target(MOTO_NUM moto_num, int32_t pos_value)
{
//		if((pos_value >= -moto_max_target[moto_num]) && (pos_value <= moto_max_target[moto_num]))
    switch((uint8_t)moto_num)
    {
        case MOTO_1:
						motor_pid_1.f_set_target(&motor_pid_1, pos_value);
            break;
        case MOTO_2:
            motor_pid_2.f_set_target(&motor_pid_2, pos_value);
            break;
        case MOTO_3:
						motor_pid_3.f_set_target(&motor_pid_3, pos_value);
            break;
        case MOTO_4:
            motor_pid_4.f_set_target(&motor_pid_4, pos_value);
            break;
defalut:
            break;
    }
}


void set_moto_stop(MOTO_NUM moto_num)
{
    switch ((uint8_t) moto_num)
    {
				case MOTO_1:
            set_moto_target(moto_num, motor_pid_1.measure);
            break;
        case MOTO_2:
            set_moto_target(moto_num, motor_pid_2.measure);
            break;
				case MOTO_3:
            set_moto_target(moto_num, motor_pid_3.measure);
            break;
        case MOTO_4:
            set_moto_target(moto_num, motor_pid_4.measure);
            break;
				
        default:
            break;
    }
}




void set_moto_maxoutput(MOTO_NUM moto_num, int32_t pos_value)
{
	
		if(pos_value > 800)
			pos_value = 800;
		else if(pos_value < 800)
			pos_value = -800;
		switch((uint8_t)moto_num)
    {
        case MOTO_1:
						motor_pid_1.f_set_maxoutput(&motor_pid_1, pos_value);
            break;
        case MOTO_2:
            motor_pid_2.f_set_maxoutput(&motor_pid_2, pos_value);
            break;
        case MOTO_3:
						motor_pid_3.f_set_maxoutput(&motor_pid_3, pos_value);
            break;
        case MOTO_4:
            motor_pid_4.f_set_maxoutput(&motor_pid_4, pos_value);
            break;
defalut:
            break;
    }
}

