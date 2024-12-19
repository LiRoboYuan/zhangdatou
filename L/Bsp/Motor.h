#ifndef _MOTOR_H
#define _MOTOR_H
#include <stdio.h>
#include <string.h>
#include "main.h"
void moto_to_zero();

#pragma pack(1) 
union Motor_Enable_Command_t {
    uint8_t data[6]; // ���ڴ洢����������ֽ�����
    struct  {
        uint8_t address;         // ��ַ
        uint8_t cmd;             // �����ֽ� 0xF3
        uint8_t sub_cmd;         // �������ֽ� 0xAB
        uint8_t enable_status;   // ʹ��״̬��1 ʹ�ܣ�0 ��ʹ��
        uint8_t sync_flag;       // ���ͬ����־��0 ��ͬ����1 ͬ��
        uint8_t checksum;        // У���ֽ�
    } MotorCmd;
};

union Clear_angle_send_t//��յ�ǰ�Ƕ���Ϣ
{
    /* data */
    uint8_t data[4];
    struct  {
        uint8_t     address; //��ַ
        uint8_t     flag;    //0x0A
        uint8_t     mode;    //0x6D
        uint8_t     tail;    //0x6B
    }clear_angle_t;
};

union Location_send_t {
    uint8_t data[13];  // ���ڴ洢��������������ֽ����飬����Ϊ13�ֽ�
    struct  {
        uint8_t address;         // ��ַ�ֶΣ����ڱ�ʶ�豸�ĵ�ַ
        uint8_t flag;            // ��־�ֶΣ����ڱ�ʶ�������ͻ���Ʊ�־
        uint8_t direction;       // ��ת����0x00��ʾ˳ʱ�루CW����0x01��ʾ��ʱ�루CCW��
        int16_t speed;          // �ٶȣ���RPM��ʾ������0x05DC = 1500RPM
        uint8_t acc_flag;        // ���ٶȵ�λ��0x00��ʾ0�����޼��ٶȣ�������ֵ��ʾ��ͬ�ļ��ٶȵ�λ
        int32_t pulse;          // �����������Ƶ��ת����������������0x00007D00 = 32000�����壬1Ȧ320����
        uint8_t mode;            // ����ģʽ��0x00��ʾ���λ��ģʽ��0x01��ʾ����λ��ģʽ
        uint8_t more_motor_mod;  // ���ͬ��ģʽ��0x00��ʾ������ͬ����0x01��ʾ����ͬ��
        uint8_t tail;            // β���ֶΣ�����У���������������Ϣ
    } Location_t;
};

// ���巵���������ݵĽṹ��
union MotorCommandResponse {
    uint8_t data[5];  // �ܹ���31���ֽڵķ�������
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

