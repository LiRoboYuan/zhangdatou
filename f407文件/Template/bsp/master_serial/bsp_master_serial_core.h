#ifndef __BSP_MASTER_SERIAL_CORE_H
#define __BSP_MASTER_SERIAL_CORE_H
#include "gd32f4xx.h"

//ͷ1
#define SERIAL_HEAD1																	0XA5
//ͷ2
#define	SERIAL_HEAD2																	0X5A
//���ݳ���
#define SERIAL_MAX_PACK_LENGTH                        64
#define SERIAL_MIN_PACK_LENGTH												6



//������ȡһ���ֽ����ݣ�����0��ʾ��ȡʧ��
typedef uint8_t(*SERIAL_FLOW_READ_PFUNC)(uint8_t *value);
//д��������ֽڵ����У�����0��ʾд��ʧ��
typedef uint8_t(*SERIAL_FLOW_WRITE_PFUNC)(uint8_t *value, uint16_t datalen);

typedef enum
{
    //�޴���
    SERIAL_ERR_ENUM_NONE=0,
    //��ʱ
    SERIAL_ERR_ENUM_OVERTIME,
    //����
    SERIAL_ERR_ENUM_OVERLEN,
    //�ȴ�
    SERIAL_ERR_ENUM_WAIT,
    //����Դ����
    SERIAL_ERR_ENUM_DATA_SRC,
}serial_err_enum;

typedef struct
{
		uint16_t sof;
    //������
    uint8_t trantype;
    //��������
    uint16_t datalen;
		//����
		uint8_t type;
    //����ʵ��
    uint8_t data[SERIAL_MAX_PACK_LENGTH];
    //�ۼӺ�У���ȡ��
    uint16_t checksum;
}serial_struct;

typedef struct
{    
    uint8_t step;         ////��ǰ��������   
    uint16_t time;        ////������������ʱ�����ڣ�����    
    uint16_t timeout;     ////����������ʱʱ�䣬����   
    uint16_t sit;        //����д��������ʱλ��
	
}serial_state_struct;     //Э�����״̬�ṹ��



serial_err_enum serial_flow_rev(SERIAL_FLOW_READ_PFUNC flowread, serial_state_struct *sate, serial_struct *pack, uint16_t divtime_ms);
uint16_t serial_creat_flow(SERIAL_FLOW_WRITE_PFUNC write, uint8_t tran_type, uint16_t datalen,uint8_t type, uint8_t *data);
uint16_t serial_pack_flow(SERIAL_FLOW_WRITE_PFUNC write, serial_struct *pack);
#endif