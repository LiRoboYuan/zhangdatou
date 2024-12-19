#ifndef _UNIAXIAL_H
#define _UNIAXIAL_H
#include "board.h"

void demo(void);
void uniaxial_init(void);

#pragma pack(1) 
union Uniaxial_date_t{
	uint8_t	date[12];
	struct{
		uint16_t hand;
		uint8_t tran_type;
		uint8_t date_type;
		uint16_t date_press;
		uint16_t date_resis;
		uint16_t date_err;
		uint16_t date_check;
	}press_resis;
	
};
#pragma pack() 

#endif

