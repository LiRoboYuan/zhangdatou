#ifndef __CAN_H
#define __CAN_H
#include "board.h"

/* select CAN baudrate */
/* 1MBps */
#define CAN_BAUDRATE  1000
/* 500kBps */
/* #define CAN_BAUDRATE  500 */
/* 250kBps */
/* #define CAN_BAUDRATE  250 */
/* 125kBps */
/* #define CAN_BAUDRATE  125 */
/* 100kBps */ 
/* #define CAN_BAUDRATE  100 */
/* 50kBps */ 
/* #define CAN_BAUDRATE  50 */
/* 20kBps */ 
/* #define CAN_BAUDRATE  20 */

#define DEV_CAN0_ID1 0x300
#define DEV_CAN0_ID2 0X500


typedef struct __typdef_can_general
{
	uint32_t can_port;
	rcu_periph_enum rcu_can;		//ADCʱ�ӿ�
	rcu_periph_enum rcu_IO_port;//IO��ʱ��
	uint32_t IO_port;							//IO port
	uint32_t pin_tx;								//tx pin
	uint32_t pin_rx;								//rx pin
	uint32_t can_af;   //afͨ��
	uint32_t gpio_speed;				//IO ����
	//can_parameter_struct can_parameter; //CAN��ʼ�������ṹ��
	//can_filter_parameter_struct can_filter; //CAN�������ṹ��
	FlagStatus can_rx_use_interrupt; //�Ƿ�ʹ�ý����ж�
}typdef_can_general;

void driver_can_init(typdef_can_general *can_general);
void can_gpio_config(typdef_can_general *can_general);
void can_network_config(typdef_can_general *can_general);
uint8_t driver_can_transmit(typdef_can_general *can_general, can_trasnmit_message_struct * transmit_message);
#endif