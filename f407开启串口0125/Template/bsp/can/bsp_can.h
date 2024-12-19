#ifndef __BSP_CAN_H
#define __BSP_CAN_H
#include "board.h"
#include "can.h"





extern typdef_can_general BSP_CAN0;
extern can_trasnmit_message_struct bsp_can_transmit_message_1;
extern uint8_t can0_rx0_buffer[1024];
extern uint8_t can0_rx1_buffer[1024];
extern uint16_t can0_rx0_length;
extern uint16_t can0_rx1_length;



void bsp_can_init(void);
uint8_t bsp_can_transmit_frame(uint32_t id, uint8_t *msg, uint8_t msg_len);

#endif