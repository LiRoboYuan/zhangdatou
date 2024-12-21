#include "can.h"

#define DEV_CAN0_MASK       (0xFFFF)

void driver_can_init(typdef_can_general *can_general)
{
	can_gpio_config(can_general);
	can_network_config(can_general);
}

void can_gpio_config(typdef_can_general *can_general)
{
		rcu_periph_clock_enable(can_general->rcu_can); //CAN时钟使能
    rcu_periph_clock_enable(can_general->rcu_IO_port); //IO时钟使能
		
		/* configure CAN0 GPIO, CAN0_TX(PB9) and CAN0_RX(PB8) */
    gpio_output_options_set(can_general->IO_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, can_general->pin_tx);
    gpio_mode_set(can_general->IO_port, GPIO_MODE_AF, GPIO_PUPD_NONE, can_general->pin_tx);
    gpio_af_set(can_general->IO_port, can_general->can_af, can_general->pin_tx);
    
    gpio_output_options_set(can_general->IO_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, can_general->pin_rx);
    gpio_mode_set(can_general->IO_port, GPIO_MODE_AF, GPIO_PUPD_NONE, can_general->pin_rx);
    gpio_af_set(can_general->IO_port, can_general->can_af, can_general->pin_rx);
	
}


void can_network_config(typdef_can_general *can_general)
{
		can_parameter_struct can_parameter;
    can_filter_parameter_struct can_filter;
		
		can_struct_para_init(CAN_INIT_STRUCT, &can_parameter); //CAN初始化结构体的初始化
    can_struct_para_init(CAN_INIT_STRUCT, &can_filter); //CAN过滤器结构体的初始化
		
		can_deinit(can_general->can_port); //CAN的deinit
		
		/* initialize CAN */
    can_parameter.time_triggered = DISABLE;
    can_parameter.auto_bus_off_recovery = ENABLE;
    can_parameter.auto_wake_up = DISABLE;
    can_parameter.auto_retrans = ENABLE;
    can_parameter.rec_fifo_overwrite = DISABLE;
    can_parameter.trans_fifo_order = DISABLE;
    can_parameter.working_mode = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1 = CAN_BT_BS1_3TQ;
    can_parameter.time_segment_2 = CAN_BT_BS2_3TQ;
		   /* 1MBps */
#if CAN_BAUDRATE == 1000 //波特率设置
    can_parameter.prescaler = 6;
    /* 500KBps */
#elif CAN_BAUDRATE == 500
    can_parameter.prescaler = 12;
    /* 250KBps */
#elif CAN_BAUDRATE == 250
    can_parameter.prescaler = 24;
    /* 125KBps */
#elif CAN_BAUDRATE == 125
    can_parameter.prescaler = 48;
    /* 100KBps */
#elif  CAN_BAUDRATE == 100
    can_parameter.prescaler = 60;
    /* 50KBps */
#elif  CAN_BAUDRATE == 50
    can_parameter.prescaler = 120;
    /* 20KBps */
#elif  CAN_BAUDRATE == 20
    can_parameter.prescaler = 300;
#else
    #error "please select list can baudrate in private defines in main.c "
#endif
		
		can_init(CAN0, &can_parameter);
		
		can_filter.filter_number = 0;
		can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = DEV_CAN0_ID1<<5;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = (uint16_t)(DEV_CAN0_MASK<<5);
    can_filter.filter_mask_low = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable = ENABLE;
    can_filter_init(&can_filter);
		
		can_filter.filter_number = 1;
		can_filter.filter_list_high = DEV_CAN0_ID2<<5;
		can_filter.filter_list_low = 0x0000;
		can_filter.filter_mask_high = (uint16_t)(DEV_CAN0_MASK<<5);
		can_filter.filter_mask_low = 0x0000;
		can_filter.filter_fifo_number = CAN_FIFO1;
		
		if(can_general->can_rx_use_interrupt == SET)//打开CAN接收中断
		{
			can_interrupt_enable(can_general->can_port, CAN_INT_RFNE0);
			can_interrupt_enable(can_general->can_port, CAN_INT_RFNE1);
		}
}

uint8_t driver_can_transmit(typdef_can_general *can_general, can_trasnmit_message_struct * transmit_message)
{
		return can_message_transmit(can_general->can_port,transmit_message);
}



