#ifndef	BSP_MX_SENSOR_H
#define	BSP_MX_SENSOR_H
#include "board.h"

#define AX_NUM 	40
#define AY_NUM	24

#define MX_SENSOR_SIZE	((AX_NUM+16)*AY_NUM*2)
#define SPEED_MAX_SIZE	4

#pragma pack (1)

typedef struct {
    uint16_t sof;
    uint8_t tran_type;
    uint16_t len;
    uint8_t type;
		uint8_t mx_sensor_value[MX_SENSOR_SIZE];
    uint16_t checksum;
} SerialFrame_t;


typedef struct {
		uint16_t sof;
		uint8_t tran_type;
    uint16_t len;
    uint8_t type;
		uint8_t mx_sensor_value[MX_SENSOR_SIZE-6];
    uint16_t checksum;
}SerialFrame_t_target;

typedef struct {
		uint8_t enable_flag;
		uint8_t speed;
		uint8_t is_have_flag;
}mx_sensor_par_stu;

typedef enum	{
		MX_SENSOR_ERR_NONE,
		MX_SENSOR_REV_SUCCESSFUL,
		MX_SENSOR_REV_FAIL,
		MX_SENSOR_DATA_SRC,
		MX_SENSOR_DATA_TURE,
}mx_sensor_err_enum;

#pragma pack ()

void set_mx_sensor_status(uint8_t enable, uint8_t speed);
void mx_sensor_run(void);
void mx_sensor_can_run(void);
uint8_t get_mx_sensor_data(uint8_t * buffer, uint16_t buffer_size);
#endif