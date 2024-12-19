#include "bsp_mx.sensor.h"
#include "bsp_can.h"
#include "bsp_uart.h"
#include "bsp_uart_ringbuffer.h"
#include "string.h"
#include "auto_adaptation.h"
SerialFrame_t serial_frame = {
	.sof = 0x5aa5,
	.tran_type = 0x01,
	.len = sizeof(serial_frame)-2,
	.type = 0X01
};

SerialFrame_t_target serial_frame_target = {
	.sof = 0x5aa5,
	.tran_type = 0x01,
	.len = sizeof(serial_frame)-2,
	.type = 0x01
};

mx_sensor_par_stu mx_sensor_par = {0};
extern void *usb_handle ;

 void set_mx_sensor_status(uint8_t enable, uint8_t speed)
{
		mx_sensor_par.enable_flag = enable;
		
		if(speed >= SPEED_MAX_SIZE)
			mx_sensor_par.speed = SPEED_MAX_SIZE;
		else
			mx_sensor_par.speed	 =	speed; 
}

static uint16_t CalChecksum(uint8_t * data, uint16_t len)
{
    uint16_t sum = 0;
    for(int i = 0; i < len; ++i)
    {
        sum += data[i];
    }
    return sum;
}


static void cut_mx_sensor(SerialFrame_t* mx_sensor_source, SerialFrame_t_target* mx_sensor_target)
{
		unsigned char cut_list[] = {14,15,38,39,40,60};
		unsigned int source_start_addr,target_start_addr,len;
		unsigned int j= 0,k = 0;
		uint16_t check_sum = 0;
		uint8_t (*serial_value_source)[AY_NUM] = (uint8_t (*)[AY_NUM])mx_sensor_source->mx_sensor_value;
		uint8_t (*serial_value_target)[AY_NUM] = (uint8_t (*)[AY_NUM])mx_sensor_target->mx_sensor_value;
		
		for(unsigned int i = 0; i < 2*AX_NUM*AY_NUM;i++)
		{
				if(i == cut_list[j])
				{
					j++;
				}
				else
				{
					//mx_sensor_target->mx_sensor_value[k] = mx_sensor_source->mx_sensor_value[]
				}
				
		}
		
//		for(unsigned char i = 0; i < sizeof(cut_list); i++)
//		{
//				if(i == 0)
//				{
//						source_start_addr = 0;
//						target_start_addr = 0;
//						len = cut_list[0];
//						memcpy(&serial_value_target[target_start_addr],&serial_value_source[source_start_addr], len*AY_NUM);
//						
//				}
//				else
//				{
//						source_start_addr = source_start_addr + len;
//						target_start_addr = cut_list[i-1] + 1;
//						len = cut_list[i]-target_start_addr;
//						 
//					 memcpy(&serial_value_target[target_start_addr],&serial_value_source[source_start_addr],(len*AY_NUM));
//				}
//		}
//		
//		for(unsigned char i = 0; i <= sizeof(cut_list); i++)
//		{
//				if(i == 0)
//				{
//						source_start_addr  = 0;
//						target_start_addr  = 0;
//						len  = cut_list[0]*AY_NUM;
//				}
//				else if(i == 6)
//				{
//						source_start_addr += len;
//						target_start_addr = (cut_list[i-1] + 1)*AY_NUM;
//						len = 
//				}
//				else
//				{
//						source_start_addr = source_start_addr + len;
//						target_start_addr = (cut_list[i-1] + 1)*AY_NUM;
//						len = cut_list[i]*AY_NUM - target_start_addr;
//				}
//				
//				memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr],&mx_sensor_source->mx_sensor_value[source_start_addr],len);
//		}
		
		
		source_start_addr = 0;
		target_start_addr = 0;
		len = cut_list[0]*AY_NUM;
		memcpy(&mx_sensor_target->mx_sensor_value[0], &mx_sensor_source->mx_sensor_value[0], len);
		
		target_start_addr = target_start_addr + len;
		source_start_addr = (cut_list[0]+1)*AY_NUM;
		len = cut_list[1]*AY_NUM -source_start_addr;
		memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr], &mx_sensor_source->mx_sensor_value[source_start_addr], len);
		
		target_start_addr = target_start_addr + len;
		source_start_addr = (cut_list[1]+1)*AY_NUM;
		len = cut_list[2]*AY_NUM -source_start_addr;
		memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr], &mx_sensor_source->mx_sensor_value[source_start_addr], len);
		
		target_start_addr = target_start_addr + len;
		source_start_addr = (cut_list[2]+1)*AY_NUM;
		len = cut_list[3]*AY_NUM -source_start_addr;
		memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr], &mx_sensor_source->mx_sensor_value[source_start_addr], len);
		
		target_start_addr = target_start_addr + len;
		source_start_addr = (cut_list[3]+1)*AY_NUM;
		len = cut_list[4]*AY_NUM -source_start_addr;
		memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr], &mx_sensor_source->mx_sensor_value[source_start_addr], len);
		
		target_start_addr = target_start_addr + len;
		source_start_addr = (cut_list[4]+1)*AY_NUM;
		len = cut_list[5]*AY_NUM -source_start_addr;
		memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr], &mx_sensor_source->mx_sensor_value[source_start_addr], len);
		
		target_start_addr = target_start_addr + len;
		source_start_addr = (cut_list[5]+1)*AY_NUM;
		len = 2*AX_NUM*AY_NUM -source_start_addr;
		memcpy(&mx_sensor_target->mx_sensor_value[target_start_addr], &mx_sensor_source->mx_sensor_value[source_start_addr], len);
		
		
		mx_sensor_target->len = mx_sensor_source->len - sizeof(cut_list)*AY_NUM;
		
		check_sum = CalChecksum((uint8_t*)&mx_sensor_target, mx_sensor_target->len);
		
		memcpy((uint8_t*)&check_sum, (uint8_t*)&mx_sensor_target + mx_sensor_target->len, 2);
	}


//一次查询30个字节
uint8_t mx_sensor_rev(void)
{
		uint16_t usart_len ;
		
	
		if(uart_rx_ringbuffer_IsEmpty(COM_UART_NUM2))
		{
			return MX_SENSOR_ERR_NONE;
		}
			
		usart_len  = uart_rx_ringbuffer_get_data_count(COM_UART_NUM2);
		

		if(usart_len >= 8)
		{
			
				for(uint16_t count = 0; count < 30; ++count)
				{
					uart_rx_ringbuffer_read_data(COM_UART_NUM2, (uint8_t*)&serial_frame.sof, 2);
					if(serial_frame.sof == 0x5AA5)
					{
							uart_rx_ringbuffer_read_data(COM_UART_NUM2, (uint8_t*)&serial_frame, 6);
							if(serial_frame.len <= sizeof(serial_frame) && serial_frame.len <= usart_len)
							{
								uart_rx_ringbuffer_pop_data(COM_UART_NUM2, (uint8_t*)&serial_frame, serial_frame.len + 2);
								return MX_SENSOR_REV_SUCCESSFUL;
							}
					}
					else
					{
						uart_rx_ringbuffer_drop_data(COM_UART_NUM2,1);
					}
				}
				uart_rx_ringbuffer_drop_data(COM_UART_NUM2,usart_len-30);
		}
			
		
		return MX_SENSOR_ERR_NONE;
}


//解包校验
uint8_t mx_sensor_unpack(SerialFrame_t *pack)
{
		if((serial_frame.len - 8) >= MX_SENSOR_SIZE)	return MX_SENSOR_DATA_SRC;
		
		memcpy((uint8_t*)&serial_frame.checksum, (uint8_t*)&serial_frame + serial_frame.len, 2);
									
		if(serial_frame.checksum == CalChecksum((uint8_t*)&serial_frame, serial_frame.len))
		{
				return MX_SENSOR_DATA_TURE;
				
		}
		
		return MX_SENSOR_DATA_SRC;
}


//进程调用，需要1ms调用一次
void mx_sensor_run(void)
{
		static uint8_t loop_speed = 0;//发送帧率调节：
		static uint8_t rev_fail_count = 0;
		mx_sensor_err_enum mx_sensor_err = 0;
	
		if(mx_sensor_par.enable_flag)
		{
				mx_sensor_err = mx_sensor_rev();
				if(mx_sensor_err == MX_SENSOR_REV_SUCCESSFUL)
				{
						mx_sensor_par.is_have_flag = 1;
						rev_fail_count = 0;
						mx_sensor_err = mx_sensor_unpack(&serial_frame);
						if(mx_sensor_err == MX_SENSOR_DATA_TURE)
						{
								loop_speed++;
						}
				}
				else 
				{
						if(mx_sensor_par.is_have_flag)
						{
								rev_fail_count++;
								mx_sensor_par.is_have_flag = rev_fail_count > 100 ? 0 : 1;
						}
				}
				
				if(loop_speed >= mx_sensor_par.speed)
				{
						loop_speed = 0;
						
						//cut_mx_sensor(&serial_frame,&serial_frame_target);
						bsp_usart_buffer_send_data(COM_UART_NUM5, (uint8_t*)&serial_frame, serial_frame.len + 2);
						memcpy(g_cushion_sensor_data.buffer, serial_frame.mx_sensor_value, AX_NUM*AY_NUM);
						memcpy(g_back_sensor_data.buffer, serial_frame.mx_sensor_value + (AX_NUM*AY_NUM), AX_NUM*AY_NUM);
						memcpy(g_16x16_sensor_data.buffer, serial_frame.mx_sensor_value + (2*AX_NUM*AY_NUM), 16*AY_NUM);
				}
				
		}	
		
}


//获取传感器数据
//1:成功 0：失败
uint8_t get_mx_sensor_data(uint8_t * buffer, uint16_t buffer_size)
{
  if(buffer_size != MX_SENSOR_SIZE)
	{
			return 0;
	}
  
	if(buffer == NULL || mx_sensor_par.is_have_flag == 0)
	{
			return 0;
	}
	
	memcpy(buffer, serial_frame.mx_sensor_value, buffer_size);
	return 1;
}

mx_sensor_err_enum mx_sensor_can_flow_rev(uint8_t *source_buffer)
{
		uint16_t sof = (source_buffer[1]<<8)| source_buffer[0];
		uint8_t	tran_type = source_buffer[2];
		uint16_t len = (source_buffer[4]<<8)| source_buffer[3];
		uint8_t type = source_buffer[5];
		uint16_t checksum = 0;
	
		if(sof != 0x5aa5)
		{
			return MX_SENSOR_REV_FAIL;
		}
		
		if(tran_type != 0x01)
		{
			return MX_SENSOR_REV_FAIL;
		}
		
		if(len != (AX_NUM*AY_NUM)+6)
		{
			return MX_SENSOR_REV_FAIL;
		}
		
		if(type != 0x01)
		{
			return MX_SENSOR_REV_FAIL;
		}
		
		checksum = (source_buffer[len+1]<<8)| source_buffer[len];
		if(checksum != CalChecksum(source_buffer,len))
		{
			return MX_SENSOR_REV_FAIL;
		}
		return MX_SENSOR_REV_SUCCESSFUL;
}

//要大于60ms调用一次
void mx_sensor_can_run(void)
{
		static uint8_t sta = 0;
		static mx_sensor_err_enum can0_rx0_flag = 0;
		static mx_sensor_err_enum can0_rx1_flag = 0;
		uint8_t send_data = 0x01;
		uint8_t can_loop_finish = 0;
	
		if(sta == 0)
		{
				bsp_can_transmit_frame(0x300,&send_data,1);
				if(can0_rx1_length == AX_NUM*AY_NUM +8)
				{
						can0_rx1_flag = mx_sensor_can_flow_rev(can0_rx1_buffer);
						if(can0_rx1_flag == MX_SENSOR_REV_SUCCESSFUL)
						{
								memcpy(serial_frame.mx_sensor_value + (AX_NUM*AY_NUM), &can0_rx1_buffer[6], AX_NUM*AY_NUM);
						}
				}
				can0_rx1_length = 0;
				can_loop_finish = 1;
				sta = 1;
		}
		else
		{
				bsp_can_transmit_frame(0x500,&send_data,1);
				if(can0_rx0_length == AX_NUM*AY_NUM +8)
				{
						can0_rx0_flag = mx_sensor_can_flow_rev(can0_rx0_buffer);
						if(can0_rx0_flag == MX_SENSOR_REV_SUCCESSFUL)
						{
								memcpy(serial_frame.mx_sensor_value, &can0_rx0_buffer[6], AX_NUM*AY_NUM);
						}
				}
				can0_rx0_length = 0;
				sta = 0;
		}
		
		
		if(can_loop_finish == 1)
		{
				if(can0_rx0_flag == 1 || can0_rx1_flag == 1)
				{
						serial_frame.checksum = CalChecksum((uint8_t *)&serial_frame,serial_frame.len);
						bsp_usart_buffer_send_data(COM_UART_NUM5, (uint8_t*)&serial_frame, serial_frame.len + 2);
						//usb_send_buffer(usb_handle, (uint8_t*)&serial_frame, serial_frame.len + 2);
				}
				can0_rx0_flag = 0;
				can0_rx1_flag = 0;
		}
		
}








