#include "bsp_app.h"
#include "main.h"
#include "circular_buffer.h"
#include "uart.h"
#include <stdlib.h>
#include "string.h"


__IO fmc_state_enum fmc_state = FMC_READY;

uint8_t cic_buff_get[1024] = {0};
uint16_t check_data1 = 0;

uint8_t buffer[BUFFER_SIZE];
uint8_t *bufferPtr = buffer;


uint16_t check_uart_data(uint8_t * data);
void addToBuffer(const uint8_t *data, size_t dataSize);
void fmc_program_all(uint32_t *data, uint32_t data_len);
void fmc_clear_flash_flag(void);
void fmc_change_flash_flag(void);
typedef void (*MCURESET)(void);
void load_appx(uint32_t appaddx);
int cbuff_state = 0;

void boot_run(void){
		memset(cic_buff_get,0,sizeof(cic_buff_get));
		//cbuff_state = CircBuf_Pop(&USART0_RxCBuf, cic_buff_get, 1024);  //该串口输入仅用于升级
		cbuff_state = CircBuf_Read(&USART0_RxCBuf, cic_buff_get, 1024);   //存在其他功能观察该串口
    if(cbuff_state){
        if(cic_buff_get[0] == 0xA5 && cic_buff_get[1] == 0x5A && cic_buff_get[2] == 0xEF){
					//检测到进入boot程序，清除app就绪标志位
					fmc_clear_flash_flag();
					//准备开始跳转到Boot区域
					__set_PRIMASK(1);//关闭所有中断
					load_appx(FMC_BOOT_START_ADDR);
        }
    }

}

void load_appx(uint32_t appaddx)
{
    MCURESET reset;

		reset = (MCURESET)*(uint32_t*)(appaddx+4);
		__set_MSP(*(uint32_t*)appaddx);
		reset();
		__enable_irq();
    
}

uint32_t read_flag_flash(void){
	return *(volatile uint16_t*)(FMC_WRITE_START_ADDR - 0x00000400u); 
}

void fmc_clear_flash_flag(void){
	fmc_unlock();
	uint32_t flag_address = FMC_WRITE_START_ADDR - 0x00000400u;
	fmc_state = fmc_page_erase(flag_address);
	fmc_lock();
}
void fmc_change_flash_flag(void){
	fmc_unlock();
	uint32_t flag_address = FMC_WRITE_START_ADDR - 0x00000400u;
	uint32_t flash_app = 1;
	fmc_word_program(flag_address, flash_app);
	
	fmc_lock();
}


