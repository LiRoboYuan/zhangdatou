#include "bsp_master_serial_core.h"

serial_err_enum serial_flow_rev(SERIAL_FLOW_READ_PFUNC flowread, serial_state_struct *sate, serial_struct *pack, uint16_t divtime_ms)
{
		uint8_t data;
		if(flowread == 0) return SERIAL_ERR_ENUM_DATA_SRC;
		while(flowread(&data)!=0)
		{
				sate->time=0;  //重置解析时间，防止超时
				switch(sate->step)
				{
						case 0:
								if(data == SERIAL_HEAD1)sate->step = 10, pack->checksum = data;
								break;
						case 10:
								if(data == SERIAL_HEAD2)sate->step = 20, pack->checksum += data;
								else if(data != SERIAL_HEAD1)sate->step = 0;
								break;
						case 20:
								pack->trantype = data;
								pack->checksum += data;
								sate->sit = 0;
								sate->step = 30;
								break;
						case 30:
								pack->checksum += data;
								pack->datalen = data;
								sate->step = 32;
								break;
						case 32:
								pack->checksum += data;
								pack->datalen |= (data<<8);
								if(pack->datalen > SERIAL_MAX_PACK_LENGTH + 6) sate->step = 0;
								else if(pack->datalen < 6) sate->step = 0;
								else if(pack->datalen == 6) sate->step = 50;
								else sate->step =35;
								break;
						case 35:
								pack->checksum += data;
								pack->type = data;
								sate->step = 40;
								break;
						case 40:
								pack->checksum += data;
								pack->data[sate->sit] = data;
								sate->sit++;
								if(sate->sit >= pack->datalen-6)
									sate->step = 50;
								break;
						case 50:
								if((pack->checksum & 0x00ff) == data)
								{
									sate->step = 52;
								}
								else
								{
									sate->step = 0;
								}
								break;
						case 52:
								sate->step = 0;
								if((pack->checksum>>8) == data)
								{
									return SERIAL_ERR_ENUM_NONE;
								}
								break;
						default:
								if(data == SERIAL_HEAD1)sate->step = 10, pack->checksum = data;
								else sate->step = 0;
				}
		}	
		if(sate->step!=0)
    {
        sate->time+=divtime_ms;//协议解析时间累计
        if(sate->time>sate->timeout)//数据流超时
        {
            sate->step=0;
            sate->time=0;
            return SERIAL_ERR_ENUM_OVERTIME;
        }
    }
    return SERIAL_ERR_ENUM_WAIT;
}


static uint16_t serial_cal_checksum(uint8_t * data, uint16_t len)
{
    uint16_t sum = 0;
    for(int i = 0; i < len; ++i)
    {
        sum += data[i];
    }
    return sum;
}



uint16_t serial_creat_flow(SERIAL_FLOW_WRITE_PFUNC write, uint8_t tran_type, uint16_t datalen,uint8_t type, uint8_t *data)
{
		uint8_t send_data = 0;
		uint16_t check = 0;
		if(datalen>SERIAL_MAX_PACK_LENGTH||write==0||datalen<SERIAL_MIN_PACK_LENGTH)return 0;
		send_data = SERIAL_HEAD1;
		check += send_data;
		write(&send_data, 1);
		send_data = SERIAL_HEAD2;
		check += send_data;
		write(&send_data, 1);
		send_data = tran_type;
		check += send_data;
		write(&send_data, 1);
		send_data = datalen;
		check += send_data;
		write(&send_data, 1);
		send_data = datalen>>8;
		check += send_data;
		write(&send_data, 1);
		send_data = type;
		check += send_data;
		write(&send_data, 1);
		
		write(data, datalen - SERIAL_MIN_PACK_LENGTH);
		for(uint16_t i = 0; i < datalen  - SERIAL_MIN_PACK_LENGTH; i++)
		{
				check += data[i];
		}
		write((uint8_t *)&check, 2);
		
		return datalen;
		
}


uint16_t serial_pack_flow(SERIAL_FLOW_WRITE_PFUNC write, serial_struct *pack)
{
		if(pack->datalen>SERIAL_MAX_PACK_LENGTH||write==0||pack->datalen<SERIAL_MIN_PACK_LENGTH)return 0;
		
		pack->sof = SERIAL_HEAD1;
		pack->sof = SERIAL_HEAD2<<8;
		
		write((uint8_t*)pack, pack->datalen);
		
		pack->checksum = serial_cal_checksum((uint8_t*)pack, pack->datalen);
		write((uint8_t *)&pack->checksum, 2);
		return pack->datalen;
}











#if 0

serial_err_enum serial_flow_rev(serial_state_struct *sate, serial_struct *pack, uint16_t divtime_ms)
{
		uint8_t data = 0;
		while(uart_ringbuffer_read_data(COM_UART_NUM5, &data,1))
		{
				switch (sate->step)
				{
						case 0:	if(data == SERIAL_HEAD1) sate->step = 10;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 10:if(data == SERIAL_HEAD2)	sate->step = 20;
										else if(data != SERIAL_HEAD1) sate->step = 0;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 20:pack->trantype = data;
										sate->step = 30;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 30:pack->datalen = data;
										sate->step = 32;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 32:pack->datalen |= (data<<8);
										if(pack->datalen > SERIAL_MAX_PACK_LENGTH + 6) sate->step = 0;
										else if(pack->datalen < 6) sate->step = 0;
										else if(pack->datalen == 6) sate->step = 50;
										else sate->step =35;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 35:pack->type = data, sate->step = 40;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 40:sate->sit = uart_ringbuffer_pop_data(COM_UART_NUM5, (uint8_t *)pack->data + sate->sit,pack->datalen-sate->sit-6);
										sate->step = 50;
								break;
						case 50:pack->checksum = data;
										sate->step = 52;
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
								break;
						case 52:pack->checksum |= (data<<8);
										
										uart_ringbuffer_drop_data(COM_UART_NUM5, 1);
										
																					
				}
		}
}


#endif

