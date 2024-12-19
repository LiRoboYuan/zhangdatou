#include "bsp_can.h"
#include "string.h"
can_receive_message_struct can0_receive_message_fifo0;
FlagStatus can0_receive_fifo0_flag;
can_receive_message_struct can0_receive_message_fifo1;
FlagStatus can0_receive_fifo1_flag;

uint8_t can0_rx0_buffer[1024] = {0};
uint8_t can0_rx1_buffer[1024] = {0};
uint16_t can0_rx0_length = 0;
uint16_t can0_rx1_length = 0;

typdef_can_general BSP_CAN0 = 
{
	.can_port = CAN0,
	.rcu_can = RCU_CAN0,		
	.rcu_IO_port = RCU_GPIOD,
	.IO_port = GPIOD,					
	.pin_tx = GPIO_PIN_1,							
	.pin_rx = GPIO_PIN_0,						
	.can_af = GPIO_AF_9,
	.gpio_speed = GPIO_OSPEED_50MHZ	,		
	.can_rx_use_interrupt = SET
};

can_trasnmit_message_struct bsp_can_transmit_message_1 = {
	.tx_sfid = 0x00,
	.tx_efid = 0x00,
	.tx_ft = CAN_FT_DATA,
	.tx_ff = CAN_FF_STANDARD,
	.tx_dlen = 1,
	.tx_data[0] = 0x01,
};


void bsp_can_init(void)
{
		driver_can_init(&BSP_CAN0);
		nvic_irq_enable(CAN0_RX0_IRQn,3,3);//使能CAN0 FIFO0 NVIC
		nvic_irq_enable(CAN0_RX1_IRQn,3,3);//使能CAN0 FIFO1 NVIC
}

uint8_t bsp_can_transmit_frame(uint32_t id, uint8_t *msg, uint8_t msg_len)//发送一帧
{
		if(msg_len > 8)
		{
			msg_len = 8;
		}
		
		bsp_can_transmit_message_1.tx_sfid = id;
		bsp_can_transmit_message_1.tx_dlen = msg_len;
		for(uint8_t i = 0; i < msg_len; i++)
		bsp_can_transmit_message_1.tx_data[i] = msg[i];
		return driver_can_transmit(&BSP_CAN0,&bsp_can_transmit_message_1);
}

void bsp_can__transmit_pack(uint32_t id, uint8_t *pack, uint8_t pack_len)
{
		uint8_t temp1 = pack_len/8;
		uint8_t temp2 = pack_len%8;

		
		if(temp1 == 0)
		{
			bsp_can_transmit_frame(id, pack, pack_len);
		}
		else
		{
			for(uint8_t i = 0; i < temp1; i++)
			{
				bsp_can_transmit_frame(id, pack + 8*i, 8);
			}
			if(temp2 != 0)
			{
				bsp_can_transmit_frame(id, pack + (pack_len - temp2), temp2);
			}
		}
}



/*!
* 说明     CAN FIFO0接收中断函数
* 输入     无
* 返回值   无
*/
void CAN0_RX0_IRQHandler(void)
{
	    /* check the receive message */
    can_message_receive(CAN0, CAN_FIFO0, &can0_receive_message_fifo0);
    
    if((0x300 == can0_receive_message_fifo0.rx_sfid)&&(0 != can0_receive_message_fifo0.rx_dlen))
		{  
			memcpy(can0_rx0_buffer + can0_rx0_length, can0_receive_message_fifo0.rx_data, can0_receive_message_fifo0.rx_dlen);
			can0_rx0_length += can0_receive_message_fifo0.rx_dlen;
		}
}

/*!
* 说明     CAN FIFO1接收中断函数
* 输入     无
* 返回值   无
*/
void CAN0_RX1_IRQHandler(void)
{
	    /* check the receive message */
    can_message_receive(CAN0, CAN_FIFO1, &can0_receive_message_fifo1);
    
    if((0x500 == can0_receive_message_fifo1.rx_sfid)&&(0 != can0_receive_message_fifo1.rx_dlen))
		{
			memcpy(can0_rx1_buffer + can0_rx1_length, can0_receive_message_fifo1.rx_data, can0_receive_message_fifo1.rx_dlen);
			can0_rx1_length += can0_receive_message_fifo1.rx_dlen;
		}
}
