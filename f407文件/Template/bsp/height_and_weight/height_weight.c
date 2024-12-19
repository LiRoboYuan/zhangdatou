#include "height_weight.h"
#include "moto_control.h"

height_weight_par_struct height_weight_par = {0};

void height_weight_to_target(height_weight_par_struct *height_weight_par)
{
		if(height_weight_par->height < 165 )
		{
				if(height_weight_par->weight < 55)
				{	
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*14/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*60/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*50/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight >= 55 && height_weight_par->weight <= 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*15/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*90/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*40/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight > 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*16/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*120/300;
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*30/60;
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;
				}
		}
		else if(height_weight_par->height >= 165 && height_weight_par->height <= 173)
		{
				if(height_weight_par->weight < 55)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*15/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*100/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*30/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight >= 55 && height_weight_par->weight <= 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*16/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*130/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*30/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight > 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*18/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*150/300;
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*20/60;
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;
				}
		}
		else if(height_weight_par->height > 173 && height_weight_par->height <= 180)
		{
				if(height_weight_par->weight < 55)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*16/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*120/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*30/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight >= 55 && height_weight_par->weight <= 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*17/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*150/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*20/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight > 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*18/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*160/300;
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*10/60;
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;
				}
		}
		else if(height_weight_par->height > 180)
		{
				if(height_weight_par->weight >= 55 && height_weight_par->weight <= 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*18/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*170/300;//水平
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*10/60;//后高调
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;//前tilt
				}
				else if(height_weight_par->weight > 75)
				{
						height_weight_par->target_out[MOTO_4] = max_route_value[MOTO_4]*23/95;//靠背
						height_weight_par->target_out[MOTO_2] = max_route_value[MOTO_2]*190/300;
						height_weight_par->target_out[MOTO_1] = max_route_value[MOTO_1]*10/60;
						height_weight_par->target_out[MOTO_3] = max_route_value[MOTO_3]*4/11;
				}
		}
		
}

void set_height(height_weight_par_struct *height_weight_par, uint16_t height_value)
{
		height_weight_par->height = height_value;
}
	

void set_weight(height_weight_par_struct *height_weight_par, uint16_t weight_value)
{
		height_weight_par->weight = weight_value;
}



