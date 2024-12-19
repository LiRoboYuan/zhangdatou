#ifndef _MOTOR_H
#define _MOTOR_H
#include <stdio.h>
#include <string.h>
#include "main.h"
void moto_to_zero();

#pragma pack(1) 
union Motor_Enable_Command_t {
    uint8_t data[6]; // 用于存储整个命令的字节数组
    struct  {
        uint8_t address;         // 地址
        uint8_t cmd;             // 命令字节 0xF3
        uint8_t sub_cmd;         // 子命令字节 0xAB
        uint8_t enable_status;   // 使能状态：1 使能，0 不使能
        uint8_t sync_flag;       // 多机同步标志：0 不同步，1 同步
        uint8_t checksum;        // 校验字节
    } MotorCmd;
};

union Clear_angle_send_t//清空当前角度信息
{
    /* data */
    uint8_t data[4];
    struct  {
        uint8_t     address; //地址
        uint8_t     flag;    //0x0A
        uint8_t     mode;    //0x6D
        uint8_t     tail;    //0x6B
    }clear_angle_t;
};

union Location_send_t {
    uint8_t data[13];  // 用于存储整个命令的数据字节数组，长度为13字节
    struct  {
        uint8_t address;         // 地址字段，用于标识设备的地址
        uint8_t flag;            // 标志字段，用于标识命令类型或控制标志
        uint8_t direction;       // 旋转方向：0x00表示顺时针（CW），0x01表示逆时针（CCW）
        int16_t speed;          // 速度：以RPM表示，例如0x05DC = 1500RPM
        uint8_t acc_flag;        // 加速度档位：0x00表示0档（无加速度），其他值表示不同的加速度档位
        int32_t pulse;          // 脉冲数：控制电机转动的脉冲数，例如0x00007D00 = 32000个脉冲，1圈320脉冲
        uint8_t mode;            // 控制模式：0x00表示相对位置模式，0x01表示绝对位置模式
        uint8_t more_motor_mod;  // 多机同步模式：0x00表示不启用同步，0x01表示启用同步
        uint8_t tail;            // 尾部字段，用于校验或者其他额外信息
    } Location_t;
};

// 定义返回命令数据的结构体
union MotorCommandResponse {
    uint8_t data[5];  // 总共有31个字节的返回命令
    struct __packed {
			uint8_t address;
			uint8_t flag;
			uint16_t curr;
			uint8_t tail;
    } response_params;
};



//union Statue_get_t{
//	
//	
//	
//}
void motor_init(void);
void Motor_run_(float x);
void Check_data_from_python(void);
void get_curr(void);
void get_err(void);
void motor_enable(int flag);
void clear_angele(void);
void Motor_run_H(float x);
void Motor_run_(float x);
void speed_counter(void);
#pragma pack() 


#endif

