#ifndef __BSP_UART_RINGBUFFER_H__
#define __BSP_UART_RINGBUFFER_H__
#include "circular_buffer.h"
#include "priv_malloc.h"
#include "bsp_uart.h"


int8_t uart_rx_ringbuffer_init(uart_type_def uart_num, uint16_t uart_cache_size);
uint16_t uart_rx_ringbuffer_pop_data(uart_type_def uart_num, uint8_t *data, uint16_t size);
uint16_t uart_rx_ringbuffer_push_data(uart_type_def uart_num, uint8_t *data, uint16_t size);
uint16_t uart_rx_ringbuffer_getused(uart_type_def uart_num);
void uart_rx_ringbuffer_deinit(uart_type_def uart_num);
uint16_t uart_rx_ringbuffer_read_data(uart_type_def uart_num, uint8_t *data, uint16_t size);
void uart_rx_ringbuffer_drop_data(uart_type_def uart_num, uint16_t lentodrop);
void uart_rx_ringbuffer_clean(uart_type_def uart_num);

uint16_t uart_tx_ringbuffer_pop_data(uart_type_def uart_num, uint8_t *data, uint16_t size);
uint16_t uart_tx_ringbuffer_push_data(uart_type_def uart_num, uint8_t *data, uint16_t size);
int8_t uart_tx_ringbuffer_init(uart_type_def uart_num, uint16_t uart_cache_size);
unsigned int uart_rx_ringbuffer_get_data_count(uart_type_def uart_num);
unsigned int uart_tx_ringbuffer_get_data_count(uart_type_def uart_num);
int uart_rx_ringbuffer_IsEmpty(uart_type_def uart_num);
int uart_tx_ringbuffer_IsEmpty(uart_type_def uart_num);
#endif
