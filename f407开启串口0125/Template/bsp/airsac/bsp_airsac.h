#ifndef _AIRSAC_H
#define _AIRSAC_H
#include "lin.h"
#define LIN0_PID		0X03
#define AISC_MODE_TICK	200//任务多少毫秒调用一次

typedef enum
{
		POWER_ON,
		UP_CHARGE,
		DOWN_CHARGE,
		MIDDLE_CHARGE,
		UP_DISCHARGE,
		MIDDLE_DISCHARGE,
		DOWN_DISCHARGE,
		ALL_CHARGE,
		ALL_DISCHARGE,
		CLASSICAL_INTENSITY1,
		CLASSICAL_INTENSITY2,
		CLASSICAL_INTENSITY3,
		STANDARD_INTENSITY1,
		STANDARD_INTENSITY2,
		STANDARD_INTENSITY3,
		SLACK_INTENSITY1,
		SLACK_INTENSITY2,
		SLACK_INTENSITY3,
		EXIT_MASSAGE,
		LUMBAR_AIR_MODE_MAX_NUM
}lumbar_air_mode_enum;

typedef enum
{
		FLANK_INIT,
		LEFT_0,
		LEFT_20,
		LEFT_40,
		LEFT_60,
		LEFT_80,
		LEFT_100,
		RIGHT_0,
		RIGHT_20,
		RIGHT_40,
		RIGHT_60,
		RIGHT_80,
		RIGHT_100,
		TW0_0,
		TWO_20,
		TWO_40,
		TWO_60,
		TWO_80,
		TWO_100,
		FLANK_AIR_MODE_MAX_NUM,
}flank_air_mode_enum;

typedef enum
{
	UP_AIR,
	MIDDLE_AIR,
	DOWN_AIR,
}air_number_enum;

extern uint8_t flank_mode;

void airsac_thread_init(void);
void set_lumbar_airsac_mode(lumbar_air_mode_enum mode);
void set_airsac_keep_mode(lumbar_air_mode_enum mode);
void set_airsac_mode_ms(lumbar_air_mode_enum mode ,uint32_t time_ms);
void set_flank_airsac_keep_mode(flank_air_mode_enum mode);
uint8_t check_air_isfull(uint8_t air_number);
void airsac_thread_run(void);
void set_airsac_loop(lumbar_air_mode_enum air_mode1, lumbar_air_mode_enum air_mode2, lumbar_air_mode_enum air_mode3);
void stop_airsac_loop(void);
#endif
