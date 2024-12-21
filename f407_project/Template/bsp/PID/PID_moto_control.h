#ifndef __PID_MOTO_CONTROL_H__
#define __PID_MOTO_CONTROL_H__

#include "bsp_moto.h"
#include "PID_base.h"

extern PID_TypeDef motor_pid_4;
extern PID_TypeDef motor_pid_2;
extern PID_TypeDef motor_pid_3;
extern PID_TypeDef motor_pid_1;

void set_moto_thread_status(uint8_t enable);
void PID_thread_init(void);
void moto_PID_thread_run(void);
void set_moto_target(MOTO_NUM moto_num, int32_t pos_value);
void set_moto_stop(MOTO_NUM moto_num);
void set_moto_maxoutput(MOTO_NUM moto_num, int32_t pos_value);

#endif
