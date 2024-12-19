#ifndef __HEIGHT_WEIGHT_H
#define __HEIGHT_WEIGHT_H
#include "board.h"


typedef struct
{
		uint16_t height;
		uint16_t weight;
		EventStatus event_status;
		uint8_t start_flag[4];
		int32_t target_out[4];
}height_weight_par_struct;

extern height_weight_par_struct height_weight_par;

void height_weight_to_target(height_weight_par_struct *height_weight_par);
void set_height(height_weight_par_struct *height_weight_par, uint16_t height_value);
void set_weight(height_weight_par_struct *height_weight_par, uint16_t weight_value);
#endif