#include "motor.h"
#include "bsp_uart.h"
#include "circular_buffer.h"
#include "bsp_uart_ringbuffer.h"
//#include "adc.h"
#include "relay.h"
#include "bsp_switch.h"
union Motor_Enable_Command_t  Motor_enable;
union Clear_angle_send_t           Clear_angele;
union Location_send_t         Location_send;
union MotorCommandResponse    Motor1_curr;


uint8_t get_vofa[128] = {0};
uint8_t get_motor[128] = {0};

int cbuff_state = 0;
int cbuff_state2 = 0;
int cbuff_state3 = 3;
#define TO_BIG_ENDIAN16(x) (((x) >> 8) | ((x) << 8))   // 将16位数据转换为大端格式
#define TO_BIG_ENDIAN32(x) (((x) >> 24) | (((x) >> 8) & 0x0000FF00) | (((x) << 8) & 0x00FF0000) | ((x) << 24))  // 将32位数据转换为大端格式

#define desk_move     640  //台子每运行1mm，需要640个信号


float uint8_to_float(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    uint8_t byte_array[4] = { byte1, byte2, byte3, byte4 };
    float result;
    memcpy(&result, byte_array, sizeof(result)); // 将4个字节拷贝到float类型
    return result;
}
/*
电机初始化
*/
void motor_init(){
    Motor_enable.MotorCmd.address       = 0x01;
    Motor_enable.MotorCmd.cmd           = 0xF3;
    Motor_enable.MotorCmd.sub_cmd       = 0xAB;
    Motor_enable.MotorCmd.enable_status = 0x01;
    Motor_enable.MotorCmd.sync_flag     = 0;
    Motor_enable.MotorCmd.checksum      = 0x6B;
		bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Motor_enable.data,sizeof(Motor_enable.data));
		
		delay_ms(5);
    Clear_angele.clear_angle_t.address  = 0x01;
    Clear_angele.clear_angle_t.flag     = 0x0A;
    Clear_angele.clear_angle_t.mode     = 0x6D;
    Clear_angele.clear_angle_t.tail     = 0x6B;
		
		bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Clear_angele.data,sizeof(Clear_angele.data));
		delay_ms(5);
    Location_send.Location_t.address    = 0x01;         // 地址字段，用于标识设备的地址
    Location_send.Location_t.flag       = 0xFD;            // 标志字段，用于标识命令类型或控制标志
    Location_send.Location_t.direction  = 0x00;       // 旋转方向：0x00表示顺时针（CW），0x01表示逆时针（CCW）
    Location_send.Location_t.speed      = 1500;          // 速度：以RPM表示，例如0x05DC = 1500RPM
    Location_send.Location_t.speed      = TO_BIG_ENDIAN16(Location_send.Location_t.speed);
    Location_send.Location_t.acc_flag   = 235;     // 加速度档位：0x00表示0档（无加速度），其他值表示不同的加速度档位
    Location_send.Location_t.pulse      = 0;          // 脉冲数：控制电机转动的脉冲数，例如0x00007D00 = 32000个脉冲，1圈320脉冲
    Location_send.Location_t.pulse      = TO_BIG_ENDIAN32(Location_send.Location_t.pulse);
    Location_send.Location_t.mode       = 0x01;            // 控制模式：0x00表示相对位置模式，0x01表示绝对位置模式
    Location_send.Location_t.more_motor_mod = 0x00;  // 多机同步模式：0x00表示不启用同步，0x01表示启用同步
    Location_send.Location_t.tail       = 0x6B;            // 尾部字段，用于校验或者其他额外信息
		bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Location_send.data,sizeof(Location_send.data));
		delay_ms(5);
}

/*
电机使能
*/
void motor_enable(int flag){
    if(flag == 1){//电机使能
        Motor_enable.MotorCmd.address       = 0x01;
        Motor_enable.MotorCmd.cmd           = 0xF3;
        Motor_enable.MotorCmd.sub_cmd       = 0xAB;
        Motor_enable.MotorCmd.enable_status = 0x01;
        Motor_enable.MotorCmd.sync_flag     = 0x00;
        Motor_enable.MotorCmd.checksum      = 0x6B;
    }
    else if(flag == 0){
        Motor_enable.MotorCmd.address       = 0x01;
        Motor_enable.MotorCmd.cmd           = 0xF3;
        Motor_enable.MotorCmd.sub_cmd       = 0xAB;
        Motor_enable.MotorCmd.enable_status = 0x00;
        Motor_enable.MotorCmd.sync_flag     = 0x00;
        Motor_enable.MotorCmd.checksum      = 0x6B;
    }
    bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Motor_enable.data,sizeof(Motor_enable.data));
}

/*
清空当前的角度信息
*/
void clear_angele(){
  
	bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Clear_angele.data,sizeof(Clear_angele.data));
}

/*
设定运行速度
*/
void speed_counter(){
	static uint8_t c[8];
	c[0] = 0x01;
	c[1] = 0xf6;
	c[2] = 0x00;
	c[3] = 0x00;
	c[4] = 0xc8;
	c[5] = 0xc8;
	c[6] = 0x00;
	c[7] = 0x6b;
	bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&c,sizeof(c));
}
/*
设定运行角度
*/
void Motor_run_(float x)//X代表坐标，相对位置移动
{
    Location_send.Location_t.mode = 0x00;
    if(x > 0){
        Location_send.Location_t.direction  = 0x00;       // 旋转方向：0x00表示顺时针（CW），0x01表示逆时针（CCW）
        Location_send.Location_t.pulse      = desk_move * x;
        Location_send.Location_t.pulse      = TO_BIG_ENDIAN32(Location_send.Location_t.pulse);
    }
    else{
        Location_send.Location_t.direction  = 0x01;       // 旋转方向：0x00表示顺时针（CW），0x01表示逆时针（CCW）
        Location_send.Location_t.pulse      =  desk_move * (x*-1);
        Location_send.Location_t.pulse      = TO_BIG_ENDIAN32(Location_send.Location_t.pulse);
    }
    
		bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Location_send.data,sizeof(Location_send.data));
}

void Motor_run_H(float x)//X代表坐标，绝对位置移动
{	
		if(x >= 0){
			Location_send.Location_t.direction  = 0x00;
			Location_send.Location_t.mode = 0x01;
			Location_send.Location_t.pulse      = desk_move * x;
			Location_send.Location_t.pulse      = TO_BIG_ENDIAN32(Location_send.Location_t.pulse);
		}
		else{
			Location_send.Location_t.direction  = 0x01;
			Location_send.Location_t.mode = 0x01;
			Location_send.Location_t.pulse      =  -1 * (desk_move * x);
			Location_send.Location_t.pulse      = TO_BIG_ENDIAN32(Location_send.Location_t.pulse);
		}
		bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&Location_send.data,sizeof(Location_send.data));
}
int16_t motor_curr = 0;
void get_curr(){
	static uint8_t abc[3];
	abc[0] = 0x01;
	abc[1] = 0x27;
	abc[2] = 0x6B;
	
	bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&abc,sizeof(abc));
}
int16_t motor_err = 0;
void get_err(){
	static uint8_t abc[3];
	abc[0] = 0x01;
	abc[1] = 0x37;
	abc[2] = 0x6B;
	bsp_usart_dma_send_data(COM_UART_NUM2,(uint8_t *)&abc,sizeof(abc));
}
int16_t	 a123123;
int16_t  pressure;
int rely_run = 0;
void Check_data_from_python(void){
	memset(get_vofa,0,sizeof(get_vofa));
	memset(get_motor,0,sizeof(get_motor));
	
	cbuff_state = CircBuf_Pop(&com_circbuf_rx[COM_UART_NUM0], get_vofa, 128);   //存在其他功能观察该串口

//	if(cbuff_state){
//		if(get_vofa[0] == 0xAA){
//			if(get_vofa[1] == 0x01){
//				a = uint8_to_float(get_vofa[2],get_vofa[3],get_vofa[4],get_vofa[5]);
//				Motor_run_(a);
//			}
//			else if(get_vofa[1] == 0x02){
//				a = uint8_to_float(get_vofa[2],get_vofa[3],get_vofa[4],get_vofa[5]);
//				if (a == 0){
//					motor_enable(0);
//				}
//				else{
//					motor_enable(1);
//				}
//			}
//			else if(get_vofa[1] == 0x03){
//				a = uint8_to_float(get_vofa[2],get_vofa[3],get_vofa[4],get_vofa[5]);
//				Motor_run_H(a);
//			}
//			else if(get_vofa[1] == 0x04){
//				clear_angele();
//			}
//		}
//	}
//	
	if(cbuff_state){
		if(get_vofa[0] == 0xA5 && get_vofa[1] == 0x5A){
			if(get_vofa[2] == 0x1A){
				if(get_vofa[3] == 0x01){
					motor_enable(0);
				}
				else if(get_vofa[3] == 0x02){
					motor_enable(1);
				}
				else if(get_vofa[3] == 0x03){
					moto_to_zero();
				}
				else if(get_vofa[3] == 0x04){
					a123123 = (get_vofa[4] ) | get_vofa[5]>>8;
					Motor_run_H(-a123123);
				}
				else if(get_vofa[3] == 0x07){
					rely_run = 1;
				}
			}
		}
	}
	cbuff_state2 = CircBuf_Pop(&com_circbuf_rx[COM_UART_NUM2], get_motor, 128);   //存在其他功能观察该串口
	if(cbuff_state2){
		if(get_motor[0] == 0x01){
			if(get_motor[1] == 0x37){
				motor_err = (get_motor[3] << 24) | (get_motor[4] << 16) | (get_motor[5] << 8) | get_motor[6];
				
			}
			else if(get_motor[1] == 0x03){
				pressure = get_motor[3]<<8 | get_motor[4];
				if(pressure < 0 ){
					pressure = 0;
				}
				
			}
			else{
				motor_curr = get_motor[2]<<8 | get_motor[3];
				if(motor_curr > 3000){
					motor_enable(0);
				}
			}
		}
	}
	
	cbuff_state3 = CircBuf_Pop(&com_circbuf_rx[COM_UART_NUM1], get_motor, 128);   //存在其他功能观察该串口
	if(cbuff_state3){
		if(get_motor[0] == 0x01){
			if(get_motor[1] == 0x37){
				motor_err = (get_motor[3] << 24) | (get_motor[4] << 16) | (get_motor[5] << 8) | get_motor[6];
				
			}
			else if(get_motor[1] == 0x03){
				pressure = get_motor[3]<<8 | get_motor[4];
				
			}
			else{
				motor_curr = get_motor[2]<<8 | get_motor[3];
				if(motor_curr > 3000){
					motor_enable(0);
				}
			}
		}
	}
}
uint16_t adc_get = 0;
int a = 0;
int16_t get_now_pressure(){
	return pressure;
}
int16_t get_now_motor(){
	return motor_err;
}


void run_relay(void){
	static int relay_count = 0;

		if(rely_run == 1){
			relay_count ++;
			bsp_relay_off(RELAY1);
		}
		if(relay_count%50 == 0 && rely_run == 1){
			relay_count = 0;
			rely_run = 0;					
			bsp_relay_on(RELAY1);
		}
}
void moto_to_zero(){
	speed_counter();
		while(1){
			static int a = 0;
//			speed_counter();
			adc_get = get_switch_value(SWITCH_1);
			if(adc_get >= 3000){
					clear_angele();
					delay_ms(2);
					motor_enable(0);
					delay_ms(100);
					motor_enable(1);
					delay_ms(100);
					Motor_run_H(-1);
					delay_ms(100);
					
					delay_ms(100);
					break;
				}
			delay_ms(100);
		}
	adc_get = 0;
}
