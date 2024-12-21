#ifndef __BSP_MASTER_SERIAL_CORE_H
#define __BSP_MASTER_SERIAL_CORE_H
#include "gd32f4xx.h"

//头1
#define SERIAL_HEAD1																	0XA5
//头2
#define	SERIAL_HEAD2																	0X5A
//数据长度
#define SERIAL_MAX_PACK_LENGTH                        64
#define SERIAL_MIN_PACK_LENGTH												6



//从流获取一个字节数据，返回0表示获取失败
typedef uint8_t(*SERIAL_FLOW_READ_PFUNC)(uint8_t *value);
//写入任意个字节到流中，返回0表示写入失败
typedef uint8_t(*SERIAL_FLOW_WRITE_PFUNC)(uint8_t *value, uint16_t datalen);

typedef enum
{
    //无错误
    SERIAL_ERR_ENUM_NONE=0,
    //超时
    SERIAL_ERR_ENUM_OVERTIME,
    //超长
    SERIAL_ERR_ENUM_OVERLEN,
    //等待
    SERIAL_ERR_ENUM_WAIT,
    //数据源出错
    SERIAL_ERR_ENUM_DATA_SRC,
}serial_err_enum;

typedef struct
{
		uint16_t sof;
    //包类型
    uint8_t trantype;
    //参数长度
    uint16_t datalen;
		//类型
		uint8_t type;
    //参数实体
    uint8_t data[SERIAL_MAX_PACK_LENGTH];
    //累加和校验和取反
    uint16_t checksum;
}serial_struct;

typedef struct
{    
    uint8_t step;         ////当前解析步骤   
    uint16_t time;        ////解析函数调用时间周期，毫秒    
    uint16_t timeout;     ////包解析，超时时间，毫秒   
    uint16_t sit;        //参数写入数据临时位置
	
}serial_state_struct;     //协议解析状态结构体



serial_err_enum serial_flow_rev(SERIAL_FLOW_READ_PFUNC flowread, serial_state_struct *sate, serial_struct *pack, uint16_t divtime_ms);
uint16_t serial_creat_flow(SERIAL_FLOW_WRITE_PFUNC write, uint8_t tran_type, uint16_t datalen,uint8_t type, uint8_t *data);
uint16_t serial_pack_flow(SERIAL_FLOW_WRITE_PFUNC write, serial_struct *pack);
#endif