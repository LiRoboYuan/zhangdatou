#include "bsp_master_serial.h"
#include "bsp_master_serial_core.h"
#include "bsp_uart_ringbuffer.h"
#include "bsp_mx.sensor.h"
#include "PID_moto_control.h"
#include "bsp_encoder.h"
#include "bsp_airsac.h"
#include "bsp_switch.h"
#include "board.h"
#include "string.h"
#include "moto_control.h"
#include "height_weight.h"
#include "auto_adaptation.h"




uint8_t uart_send_buf[1024*3] = {0};
serial_state_struct serial_state;
serial_struct serial_pack;
master_serial_moto_par_struct master_serial_moto_par[4] = {0};
uint8_t mx_sensor_value[MX_SENSOR_SIZE] = {0};


serial_struct return_pack;

void bsp_master_serial_init(void)
{
		serial_state.sit=0;
    serial_state.step=0;
    serial_state.time=0;
    //配置包解析超时时间 毫秒
    serial_state.timeout=100;
		
		return_pack.sof = 0x5AA5;
}


/*
数据流读写接口，需要用户实现
*/
uint8_t serial_flow_read_func(uint8_t *value)
{
	return uart_rx_ringbuffer_pop_data(COM_UART_NUM5, value ,1);
}


static uint16_t master_serial_cal_checksum(uint8_t * data, uint16_t len)
{
    uint16_t sum = 0;
    for(int i = 0; i < len; ++i)
    {
        sum += data[i];
    }
    return sum;
}


void send_uart_frame(uint8_t tran_type, uint8_t type, uint8_t *datas, uint16_t datas_len)
{
		uint16_t num = 0;
		uint16_t loop = 0;
		uint16_t checksum = 0;
	
		uart_send_buf[num++] = SERIAL_HEAD1;
		uart_send_buf[num++] = SERIAL_HEAD2;
		uart_send_buf[num++] = tran_type;
		uart_send_buf[num++] = (datas_len + 6) & 0xff;
		uart_send_buf[num++] = (datas_len + 6) >> 8;
		uart_send_buf[num++] = type;
		for(loop = 0; loop < datas_len; loop++)
		{
			uart_send_buf[num++] = datas[loop];
		}
		
		checksum = master_serial_cal_checksum(uart_send_buf, num);
		
		uart_send_buf[num++] = checksum & 0xff;
		uart_send_buf[num++] = checksum >> 8;
		bsp_usart_buffer_send_data(COM_UART_NUM5, uart_send_buf,num);
}

void send_err_pack(void)
{
		uint8_t data = 0x00;
		send_uart_frame(0x31,0xff,&data,1);
}


void send_successful_pack()
{
		uint8_t data = 0x00;
		send_uart_frame(0x31,0xfe,&data,1);
}

void send_moto_staus_pack(uint8_t tran_type, uint8_t moto_num, uint8_t status)
{
		uint8_t data[3] = {moto_num+1,0x00,status};
		send_uart_frame(0x31,tran_type,(uint8_t *)&data,3);
}

void send_staus_pack(uint8_t tran_type, uint8_t status)
{
		uint8_t data = status;
		send_uart_frame(0x31,tran_type,&data,1);
}

void send_encoder_current_pack(int16_t moto,int16_t current)
{
		int16_t send_pack[2] = {moto+1,current};
		
		if(moto > MOTO_MAX_NUMBER||moto < 0)
			send_err_pack();
		else
			send_uart_frame(0x31,0x03,(uint8_t *)&send_pack,sizeof(send_pack));
}

void send_moto_position_pack(int16_t moto, int16_t current)
{
		int16_t send_pack[2] = {moto+1,0};
		
		if(current < 0)
		{
			current = 0;
		}
		send_pack[1] = (current*moto_physics_value[moto]*10/max_route_value[moto] + 5)/10;
		
		if(moto > MOTO_MAX_NUMBER||moto < 0)
			send_err_pack();
		else
			send_uart_frame(0x31,0x02,(uint8_t *)&send_pack,sizeof(send_pack));
}


void rev_set_moto_target(uint8_t * data)
{
	 int16_t moto_num = (data[1]<<8) | data[0];
	 int16_t target_temp = (data[3]<<8) | data[2];
	 
	 if(moto_num > MOTO_MAX_NUMBER || moto_num < 1)
	 {
			send_err_pack();
	 }
	 else
	 {
			master_serial_moto_par[moto_num-1].set_moto_flag = 1;
			master_serial_moto_par[moto_num-1].target = target_temp;
	 }
}

void rev_set_moto_position(uint8_t * data)
{
	 int16_t moto_num = (data[1]<<8) | data[0];
	 int16_t posit_temp = (data[3]<<8) | data[2];
	 
	 if(moto_num > MOTO_MAX_NUMBER || moto_num < 1)
	 {
			send_err_pack();
	 }
	 else if(posit_temp < 0 || posit_temp > moto_physics_value[moto_num-1])
	 {
			send_err_pack();
	 }
	 else
	 {
		 master_serial_moto_par[moto_num-1].set_moto_flag = 2;
		 master_serial_moto_par[moto_num-1].target = posit_temp*max_route_value[moto_num-1]/moto_physics_value[moto_num-1];
	 }
}
	
 void rev_set_airsac_keep_mode(uint8_t * data)
{
		lumbar_air_mode_enum air_mode = data[0];
		
		if(air_mode > LUMBAR_AIR_MODE_MAX_NUM)
			 send_err_pack();
		else
		{
				back_auto_adaptation_enable(false);
				set_airsac_keep_mode((lumbar_air_mode_enum) serial_pack.data[0]);
				
		}
}
void rev_set_flank_airsac_keep_mode(uint8_t *data)
{
		flank_air_mode_enum air_mode = data[0];
		if(air_mode > FLANK_AIR_MODE_MAX_NUM)
			send_err_pack();
		else
		{
			set_flank_airsac_keep_mode(air_mode);
			flank_mode = 0;
		}
}

void rev_set_moto_adjust_moto(uint8_t *data)
{
	int16_t moto_num = (data[1]<<8) | data[0];
	
	if(moto_num > MOTO_MAX_NUMBER || moto_num < 1)
	 {
			send_err_pack();
	 }
	 else
	 {
			set_moto_adjust_mode(moto_num-1);
	 }
}

void rev_set_moto_stop(uint8_t *data)
{
	int16_t moto_num = (data[1]<<8) | data[0];
	
	 if(moto_num > MOTO_MAX_NUMBER || moto_num < 1)
	 {
			send_err_pack();
	 }
	 else
	 {
		 master_serial_moto_par[moto_num-1].stop_flag = 1;
	 }
	 
}

void rev_set_height(uint8_t *data)
{
		uint16_t height_value = (data[1]<<8) | data[0];
		
		set_height(&height_weight_par, height_value);
		send_successful_pack();
		
}

void rev_set_weight(uint8_t *data)
{
		uint16_t weight_value = (data[1]<<8) | data[0];
	
		set_weight(&height_weight_par, weight_value);
		height_weight_par.event_status = ENABLE;
		height_weight_to_target(&height_weight_par);
		//send_successful_pack();
}

void rev_set_back_auto_adaptation_enable_flag(uint8_t *data)
{
		bool enable_flag = data[0];
		uint8_t mode_shit = data[1];
		if(enable_flag && (!check_auto_adaptation_enable_flag()))
		{
				back_auto_adaptation_enable(true);
				set_back_auto_adaptation_mode_shit(mode_shit); 
				set_flank_airsac_keep_mode(TWO_60);
				flank_mode = 0;
				
				if(get_mx_sensor_data(mx_sensor_value,sizeof(mx_sensor_value)) == true)
				{
						send_uart_frame(0x31,0x28,mx_sensor_value,sizeof(mx_sensor_value));
				}
				g_start_back__total_value = calculateTotal((uint8_t *)g_back_sensor_data.buffer, 0, 0, AY_NUM, AX_NUM, AY_NUM, AX_NUM, &g_start_back_pressure_number);
		}
		else if(enable_flag == 0)
		{
				back_auto_adaptation_enable(false);
				set_back_auto_adaptation_mode_shit(data[1]);
		}
		
}


void get_moto_max_route(uint8_t *data)
{
		int16_t moto_num = (data[1]<<8) | data[0];
		int16_t send_pack[2] = {moto_num,max_route_value[moto_num-1]};
		if(moto_num > MOTO_MAX_NUMBER || moto_num < 1)
		{
				send_err_pack();
		}
		else
		{
				send_uart_frame(0x31,0x06,(uint8_t *)&send_pack,sizeof(send_pack));
		}
}

void get_height(uint8_t *data)
{
		send_uart_frame(0x31,0x20,(uint8_t *)&height_weight_par.height,sizeof(height_weight_par.height));
}

void get_weight(uint8_t *data)
{
		send_uart_frame(0x31,0x21,(uint8_t *)&height_weight_par.weight,sizeof(height_weight_par.weight));
}

void get_seated_flag(uint8_t *data)
{
		send_uart_frame(0x31, 0x25, (uint8_t *)&g_seated_flag,sizeof(g_seated_flag));
}

void get_sitting_position(uint8_t *data)
{
		send_uart_frame(0x31, 0x26, (uint8_t *)&g_sitting_position, sizeof(g_sitting_position));
}

void rev_set_airsac_loop_mode(uint8_t *data)
{
	back_auto_adaptation_enable(false);
	set_airsac_loop(UP_DISCHARGE,MIDDLE_DISCHARGE,DOWN_DISCHARGE);
}

void rev_set_flank_mode(uint8_t *data)
{
	flank_mode = data[0] + 1;
}


void master_serial_run(uint16_t time_ms)
{
		serial_err_enum state;
		int16_t moto_num_temp = 0;
		int16_t target_temp = 0;
		
		state = serial_flow_rev(serial_flow_read_func,&serial_state,&serial_pack,time_ms);
		
		if(state == SERIAL_ERR_ENUM_NONE)
		{
				//设置电机气囊
				if(serial_pack.trantype == 0x14)
				{
						switch(serial_pack.type)
						{
								case 0x02:rev_set_moto_position(serial_pack.data);
										break;
								case 0x03:rev_set_moto_target(serial_pack.data);
										break;
								case 0x04:set_moto_maxoutput(serial_pack.data[0], serial_pack.data[2]);
										break;
								case 0x05:rev_set_moto_adjust_moto(serial_pack.data);
										break;
								case 0x07:rev_set_moto_stop(serial_pack.data);
										break;
								case 0x09:rev_set_airsac_loop_mode(serial_pack.data);
										break;
								case 0x12:rev_set_airsac_keep_mode(serial_pack.data);
										break;
								case 0x13:rev_set_flank_airsac_keep_mode(serial_pack.data);
										break;
								case 0x14:rev_set_flank_mode(serial_pack.data);
										break;
								case 0x20:rev_set_height(serial_pack.data);
										break;
								case 0x21:rev_set_weight(serial_pack.data);
										break;
								case 0x27:rev_set_back_auto_adaptation_enable_flag(serial_pack.data);
										break;
								default:
										break;
						}
				}
				else if(serial_pack.trantype == 0x24)//获取电机气囊参数
				{ 
						switch(serial_pack.type)
						{
							case 0x02:send_moto_position_pack(serial_pack.data[0]-1,get_encoder_currentCount(serial_pack.data[0]-1));
									break;
							case 0x03:send_encoder_current_pack(serial_pack.data[0]-1,get_encoder_currentCount(serial_pack.data[0]-1));
									break;
							case 0x06:get_moto_max_route(serial_pack.data);
									break;
							case 0x20:get_height(serial_pack.data);
									break;
							case 0x21:get_weight(serial_pack.data);
									break;
							case 0x25:get_seated_flag(serial_pack.data);
									break;
							case 0x26:get_sitting_position(serial_pack.data);
									break;
						}
				}
		}
}