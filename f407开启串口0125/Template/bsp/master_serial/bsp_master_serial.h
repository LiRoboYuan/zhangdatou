#ifndef __BSP_MASTER_SERIAL_H
#define __BSP_MASTER_SERIAL_H
#include "gd32f4xx.h"
#include "board.h"
#include <stdbool.h>


typedef enum
{
		ENCODER_CURRENT,
		MOTO_SPEED,
		GENERAL_PACK_TYPE_MAX_NUM
}general_pack_type_enum;

typedef struct
{
		uint8_t set_moto_flag;
		uint8_t stop_flag;
		int32_t target;
}master_serial_moto_par_struct;

extern master_serial_moto_par_struct master_serial_moto_par[4];

void bsp_master_serial_init(void);

void send_err_pack(void);

void send_successful_pack(void);

void send_moto_staus_pack(uint8_t tran_type, uint8_t moto_num, uint8_t status);

void send_staus_pack(uint8_t tran_type, uint8_t status);

void send_encoder_current_pack(int16_t moto,int16_t current);

void send_moto_position_pack(int16_t moto, int16_t posit);

void master_serial_run(uint16_t time_ms);


#endif