#include "bsp_uart_ringbuffer.h"

CircBuf_t com_circbuf_rx[COM_UART_MAX_NUM] = {0};
CircBuf_t com_circbuf_tx[COM_UART_MAX_NUM] = {0};
int8_t uart_rx_ringbuffer_init(uart_type_def uart_num, uint16_t uart_cache_size)
{
    CircBuf_Alloc(com_circbuf_rx + uart_num, uart_cache_size);

		return 0;
}
int8_t uart_tx_ringbuffer_init(uart_type_def uart_num, uint16_t uart_cache_size)
{
		CircBuf_Alloc(com_circbuf_tx + uart_num, uart_cache_size);
		
		return 0;
}

uint16_t uart_rx_ringbuffer_pop_data(uart_type_def uart_num, uint8_t *data, uint16_t size)
{
    return CircBuf_Pop(com_circbuf_rx + uart_num, data, size);
}

uint16_t uart_tx_ringbuffer_pop_data(uart_type_def uart_num, uint8_t *data, uint16_t size)
{
		return CircBuf_Pop(com_circbuf_tx + uart_num, data, size);
}

uint16_t uart_rx_ringbuffer_push_data(uart_type_def uart_num, uint8_t *data, uint16_t size)
{
    return CircBuf_Push(com_circbuf_rx + uart_num, data, size);
}

uint16_t uart_tx_ringbuffer_push_data(uart_type_def uart_num, uint8_t *data, uint16_t size)
{
    return CircBuf_Push(com_circbuf_tx + uart_num, data, size);
}

uint16_t uart_rx_ringbuffer_getused(uart_type_def uart_num)
{
    return  CircBuf_GetUsedSize(com_circbuf_rx + uart_num);
}

uint16_t uart_tx_ringbuffer_getused(uart_type_def uart_num)
{
    return  CircBuf_GetUsedSize(com_circbuf_tx + uart_num);
}


uint16_t uart_rx_ringbuffer_read_data(uart_type_def uart_num, uint8_t *data, uint16_t size)
{
    return CircBuf_Read(com_circbuf_rx + uart_num, data, size);
}

uint16_t uart_tx_ringbuffer_read_data(uart_type_def uart_num, uint8_t *data, uint16_t size)
{
    return CircBuf_Read(com_circbuf_tx + uart_num, data, size);
}

void uart_rx_ringbuffer_drop_data(uart_type_def uart_num, uint16_t lentodrop)
{
    CircBuf_Drop(com_circbuf_rx + uart_num, lentodrop);
}

void uart_tx_ringbuffer_drop_data(uart_type_def uart_num, uint16_t lentodrop)
{
    CircBuf_Drop(com_circbuf_tx + uart_num, lentodrop);
}

void uart_rx_ringbuffer_clean(uart_type_def uart_num)
{
		CircBuf_Drop(com_circbuf_rx + uart_num, CircBuf_GetUsedSize(com_circbuf_rx + uart_num));
}

void uart_tx_ringbuffer_clean(uart_type_def uart_num)
{
		CircBuf_Drop(com_circbuf_tx + uart_num, CircBuf_GetUsedSize(com_circbuf_tx + uart_num));
}


unsigned int uart_rx_ringbuffer_get_data_count(uart_type_def uart_num)
{
		return CircBuf_GetUsedSize(com_circbuf_rx + uart_num);
}

unsigned int uart_tx_ringbuffer_get_data_count(uart_type_def uart_num)
{
		return CircBuf_GetUsedSize(com_circbuf_tx + uart_num);
}

int uart_rx_ringbuffer_IsEmpty(uart_type_def uart_num)
{
	return CircBuf_IsEmpty(com_circbuf_rx + uart_num);
}

int uart_tx_ringbuffer_IsEmpty(uart_type_def uart_num)
{
	return CircBuf_IsEmpty(com_circbuf_tx + uart_num);
}

void uart_rx_ringbuffer_deinit(uart_type_def uart_num)
{
//    Circbuf_Free(com_circbuf + uart_num);
}
