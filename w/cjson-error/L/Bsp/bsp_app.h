#ifndef __BSP_BOOTAPP_h
#define __BSP_BOOTAPP_h

#include "gd32e10x.h"
#include "string.h"
#include "systick.h"
#include "stdbool.h"


#define BUFFER_SIZE 	1 * 1024//待烧录缓冲区大小
#define app_dirction  64 //64*kb(目前0x08010000到最后共剩有64kb数据区)



#define FMC_PAGE_SIZE           ((uint16_t)0x400U)
#define FMC_WRITE_START_ADDR    ((uint32_t)(0x08003c00U))
#define FMC_WRITE_END_ADDR      ((uint32_t)(0x0801FFFFU))

#define FMC_BOOT_START_ADDR     ((uint32_t)(0x08000000U))


void fmc_program(uint32_t *data, uint32_t data_len);

extern __IO fmc_state_enum fmc_state;

void boot_run(void);
uint32_t read_flag_flash(void);

#endif

