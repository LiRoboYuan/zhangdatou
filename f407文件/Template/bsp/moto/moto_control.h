#ifndef _MOTO_CONTROL_H
#define _MOTO_CONTROL_H

#include "bsp_encoder.h"
#include "bsp_moto.h"
#include "gd32f4xx.h"
#include "board.h"
#include "PID_moto_control.h"
#include "bsp_master_serial.h"
#include "bsp_switch.h"
#include "bsp_encoder.h"

#define BACK_PHYSICS_VALUE		95		//单位角度
#define	FRONT_PHYSICS_VALUE		300		//单位mm
#define HEIGHT_PHYSICS_VALUE	60		//单位mm
#define TILT_PHYSICS_VALUE		11		//单位角度

typedef enum
{
	POSITIVE,
	NEGATIVE
}zero_direction_enum;

typedef enum
{
	ADJUST_MAX_ROUTE,
	ADJUST_ZERO
	
}adjust_sta_enum;

typedef struct
{
	uint8_t start_flag;
	adjust_sta_enum state;
	zero_direction_enum zero_direction;
	int32_t target;
	int32_t zero_value;
	int32_t max_route_value;
}adjust_mode_struct;

typedef enum {
	E_IDLE,
	E_SWITCH,
	E_MASTER_SERIAL,
	E_ADJUST,
	E_HEIGHT_WEIGHT,
}Enum_State;

typedef enum {
	IDLE_EVENT,
	SWITCH_EVENT,
	MASTER_EVENT,
	ADJUST_EVENT,
	HEIGHT_WEIGHT_EVENT,
}Enum_Event;

extern int32_t max_route_value[MOTO_MAX_NUM];
extern int16_t moto_physics_value[MOTO_MAX_NUM];

void moto_run(void);
void set_moto_adjust_mode(MOTO_NUM moto_num);
void set_all_zero_direction(void);
#endif
