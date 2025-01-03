#include "bsp_uart.h"
#include <stdio.h>
#include <stdbool.h>
#include "bsp_switch.h"
#include "bsp_uart_ringbuffer.h"
#include "string.h"
#include "bsp_led.h"
__attribute__((aligned (32)))  uint8_t usart0_rx_buffer[COM_UART0_RX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart0_tx_buffer[COM_UART0_TX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart1_rx_buffer[COM_UART1_RX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart1_tx_buffer[COM_UART1_TX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart2_rx_buffer[COM_USART2_RX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart2_tx_buffer[COM_USART2_TX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart5_rx_buffer[COM_USART5_RX_BUFFER_SIZE] = {0};
__attribute__((aligned (32)))  uint8_t usart5_tx_buffer[COM_USART5_TX_BUFFER_SIZE] = {0};

static const uint32_t COM_UART[COM_UART_MAX_NUM] =
{
    COM_UART0,
    COM_UART1,
    COM_USART2,
    COM_USART5
};
static const IRQn_Type COM_UART_IRQn[COM_UART_MAX_NUM] =
{
    COM_UART0_IRQn,
    COM_UART1_IRQn,
    COM_USART2_IRQn,
    COM_USART5_IRQn
};
static const rcu_periph_enum COM_UART_CLK[COM_UART_MAX_NUM] =
{
    COM_UART0_CLK,
    COM_UART1_CLK,
    COM_USART2_CLK,
    COM_USART5_CLK
};
static const rcu_periph_enum COM_UART_TX_GPIO_CLK[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_GPIO_CLK,
    COM_UART1_TX_GPIO_CLK,
    COM_USART2_TX_GPIO_CLK,
    COM_USART5_TX_GPIO_CLK
};
static const uint32_t COM_UART_TX_GPIO_AF[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_GPIO_AF,
    COM_UART1_TX_GPIO_AF,
    COM_USART2_TX_GPIO_AF,
    COM_USART5_TX_GPIO_AF
};
static const uint32_t COM_UART_TX_PORT[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_PORT,
    COM_UART1_TX_PORT,
    COM_USART2_TX_PORT,
    COM_USART5_TX_PORT
};
static const uint32_t COM_UART_TX_PIN[COM_UART_MAX_NUM] =
{
    COM_UART0_TX_PIN,
    COM_UART1_TX_PIN,
    COM_USART2_TX_PIN,
    COM_USART5_TX_PIN
};
static const rcu_periph_enum COM_UART_RX_GPIO_CLK[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_GPIO_CLK,
    COM_UART1_RX_GPIO_CLK,
    COM_USART2_RX_GPIO_CLK,
    COM_USART5_RX_GPIO_CLK
};
static const uint32_t COM_UART_RX_GPIO_AF[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_GPIO_AF,
    COM_UART1_RX_GPIO_AF,
    COM_USART2_RX_GPIO_AF,
    COM_USART5_RX_GPIO_AF
};
static const uint32_t COM_UART_RX_PORT[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_PORT,
    COM_UART1_RX_PORT,
    COM_USART2_RX_PORT,
    COM_USART5_RX_PORT
};
static const uint32_t COM_UART_RX_PIN[COM_UART_MAX_NUM] =
{
    COM_UART0_RX_PIN,
    COM_UART1_RX_PIN,
    COM_USART2_RX_PIN,
    COM_USART5_RX_PIN
};

static const uint32_t COM_UART_BAUD[COM_UART_MAX_NUM] =
{
    COM_UART0_BAUD,
    COM_UART1_BAUD,
    COM_USART2_BAUD,
    COM_USART5_BAUD
};

static const uint32_t COM_UART_DMA_RCU[COM_UART_MAX_NUM] =
{
    COM_USART0_DMA_RCU,
    COM_USART1_DMA_RCU,
    COM_USART2_DMA_RCU,
    COM_USART5_DMA_RCU
};


static const uint32_t COM_UART_DMA[COM_UART_MAX_NUM] =
{
    COM_USART0_DMA,
    COM_USART1_DMA,
    COM_USART2_DMA,
    COM_USART5_DMA
};

static const uint32_t COM_UART_TX_DMA_CHANNEL[COM_UART_MAX_NUM] =
{
    COM_USART0_TX_DMA_CHANNEL,
    COM_USART1_TX_DMA_CHANNEL,
    COM_USART2_TX_DMA_CHANNEL,
    COM_USART5_TX_DMA_CHANNEL
};

static const uint32_t COM_UART_RX_DMA_CHANNEL[COM_UART_MAX_NUM] =
{
    COM_USART0_RX_DMA_CHANNEL,
    COM_USART1_RX_DMA_CHANNEL,
    COM_USART2_RX_DMA_CHANNEL,
    COM_USART5_RX_DMA_CHANNEL
};

static const uint32_t COM_UART_RX_DMA_SUBPERI[COM_UART_MAX_NUM] =
{
    COM_USART0_RX_DMA_SUBPERI,
    COM_USART1_RX_DMA_SUBPERI,
    COM_USART2_RX_DMA_SUBPERI,
    COM_USART5_RX_DMA_SUBPERI
};

static const uint32_t COM_UART_TX_DMA_SUBPERI[COM_UART_MAX_NUM] =
{
    COM_USART0_TX_DMA_SUBPERI,
    COM_USART1_TX_DMA_SUBPERI,
    COM_USART2_TX_DMA_SUBPERI,
    COM_USART5_TX_DMA_SUBPERI
};

static const uint32_t COM_UART_RX_DMA_IRQ[COM_UART_MAX_NUM] =
{
    COM_USART0_RX_DMA_IRQ,
    COM_USART1_RX_DMA_IRQ,
    COM_USART2_RX_DMA_IRQ,
    COM_USART5_RX_DMA_IRQ
};

static const uint32_t COM_UART_RX_BUFFER_SIZE[COM_UART_MAX_NUM] = 
{	
		COM_UART0_RX_BUFFER_SIZE,
		COM_UART1_RX_BUFFER_SIZE,
		COM_USART2_RX_BUFFER_SIZE,
		COM_USART5_RX_BUFFER_SIZE
		
};

static const uint32_t COM_UART_TX_BUFFER_SIZE[COM_UART_MAX_NUM] = 
{
		COM_UART0_TX_BUFFER_SIZE,
		COM_UART1_TX_BUFFER_SIZE,
		COM_USART2_TX_BUFFER_SIZE,
		COM_USART5_TX_BUFFER_SIZE
};

static uint8_t const* COM_UART_TX_BUFFER_ADDR[COM_UART_MAX_NUM] =
{
		usart0_tx_buffer,
		usart1_tx_buffer,
		usart2_tx_buffer,
		usart5_tx_buffer,
};

static uint8_t const* COM_UART_RX_BUFFER_ADDR[COM_UART_MAX_NUM] =
{
		usart0_rx_buffer,
		usart1_rx_buffer,
		usart2_rx_buffer,
		usart5_rx_buffer
};

// clang-format on
void bsp_uart_periph_init(uart_type_def uart_num)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable( COM_UART_TX_GPIO_CLK[uart_num]);
    rcu_periph_clock_enable( COM_UART_RX_GPIO_CLK[uart_num]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_UART_CLK[uart_num]);

    /* connect port to USARTx_Tx */
    gpio_af_set(COM_UART_TX_PORT[uart_num], COM_UART_TX_GPIO_AF[uart_num], COM_UART_TX_PIN[uart_num]);

    /* connect port to USARTx_Rx */
    gpio_af_set(COM_UART_RX_PORT[uart_num], COM_UART_RX_GPIO_AF[uart_num], COM_UART_RX_PIN[uart_num]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(COM_UART_TX_PORT[uart_num], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_TX_PIN[uart_num]);
    gpio_output_options_set(COM_UART_TX_PORT[uart_num], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_TX_PIN[uart_num]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(COM_UART_RX_PORT[uart_num], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_RX_PIN[uart_num]);
    gpio_output_options_set(COM_UART_RX_PORT[uart_num], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_RX_PIN[uart_num]);

    /* USART configure */
    usart_deinit(COM_UART[uart_num]);
    usart_baudrate_set(COM_UART[uart_num],COM_UART_BAUD[uart_num]);
    usart_receive_config(COM_UART[uart_num], USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_UART[uart_num], USART_TRANSMIT_ENABLE);
    usart_enable(COM_UART[uart_num]);


#ifndef ENABLE_USART_DMA
    /* USART interrupt configuration */
    if (uart_num == COM_UART_NUM0)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 2, 0);
    }
    else if (uart_num == COM_UART_NUM1)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 2, 0);
    }
    else if (uart_num == COM_UART_NUM2)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 0, 0);
    }
    else if (uart_num == COM_UART_NUM5)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 1, 0);
    }
    else
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 2, 0);
    }

    usart_interrupt_enable(COM_UART[uart_num], USART_INT_RBNE);
#else
    /* USART interrupt configuration */
    if (uart_num == COM_UART_NUM0)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 2, 0);
    }
    else if (uart_num == COM_UART_NUM1)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 2, 0);
    }
    else if (uart_num == COM_UART_NUM2)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 0, 0);
    }
    else if (uart_num == COM_UART_NUM5)
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 1, 0);
    }
    else
    {
        nvic_irq_enable(COM_UART_IRQn[uart_num], 2, 0);
    }

    usart_interrupt_enable(COM_UART[uart_num], USART_INT_IDLE);
#endif
}

void bsp_dma_usart_int(uart_type_def uart_num, bool dma_tx_enable, bool dma_rx_enable)
{
    dma_single_data_parameter_struct dma_init_struct;

    if (dma_tx_enable)
    {
        /* enable DMA1 */
        rcu_periph_clock_enable(COM_UART_DMA_RCU[uart_num]);
        /* deinitialize DMA */
        dma_deinit(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num]);
        dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
        dma_init_struct.memory0_addr = (uint32_t)COM_UART_TX_BUFFER_ADDR[uart_num];
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
        dma_init_struct.number = COM_UART_TX_BUFFER_SIZE[uart_num];
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(COM_UART[uart_num]);
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
        dma_single_data_mode_init(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], &dma_init_struct);
        /* configure DMA mode */
        dma_circulation_disable(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num]);
        dma_channel_subperipheral_select(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], COM_UART_TX_DMA_SUBPERI[uart_num]);

#ifdef ENABLE_USART_DMA_TX
        /* close DMA TX interrupt */
        nvic_irq_enable(DMA1_Channel7_IRQn, 0, 0);
        dma_interrupt_enable(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], DMA_CHXCTL_FTFIE);
#endif

}

    if (dma_rx_enable)
    {
        rcu_periph_clock_enable(COM_UART_DMA_RCU[uart_num]);

        dma_deinit(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num]);
        dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;		
				dma_init_struct.memory0_addr = (uint32_t)COM_UART_RX_BUFFER_ADDR[uart_num];
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;			
				dma_init_struct.number = COM_UART_RX_BUFFER_SIZE[uart_num];
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(COM_UART[uart_num]);
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
        dma_single_data_mode_init(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], &dma_init_struct);
        /* configure DMA mode */
        dma_circulation_disable(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num]);
        dma_channel_subperipheral_select(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], COM_UART_RX_DMA_SUBPERI[uart_num]);

        /* close DMA RX interrupt */
        nvic_irq_enable(COM_UART_RX_DMA_IRQ[uart_num], 1, 0);
        dma_interrupt_enable(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], DMA_CHXCTL_FTFIE);

        dma_channel_enable(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num]);
        usart_dma_receive_config(COM_UART[uart_num], USART_RECEIVE_DMA_ENABLE);
    }
}

void bsp_uart_init(void)
{
    for (uart_type_def uart = COM_UART_NUM0; uart < COM_UART_MAX_NUM; uart++)
    {
        bsp_uart_periph_init(uart);
    }
}

void bsp_usart_dma_send_data(uart_type_def uart_num ,uint8_t *buffer, uint16_t buffer_size)
{
	
	while(dma_flag_get(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], DMA_FLAG_FTF))
	dma_flag_clear(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], DMA_FLAG_FTF);
	dma_memory_address_config(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], DMA_MEMORY_0, (uint32_t)buffer);
	dma_transfer_number_config(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num], buffer_size);
	dma_channel_enable(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num]);
	usart_dma_transmit_config(COM_UART[uart_num], USART_TRANSMIT_DMA_ENABLE);
}

void bsp_usart_dma_recv_data(uart_type_def uart_num, uint8_t *buffer, uint16_t buffer_size)
{
    if (dma_flag_get(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], DMA_FLAG_FTF))
    {
        dma_flag_clear(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], DMA_FLAG_FTF);
        dma_channel_enable(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num]);
        bsp_usart_dma_send_data(COM_UART_NUM2, buffer, buffer_size);
    }
}

uint16_t bsp_usart_buffer_send_data(uart_type_def uart_num, uint8_t *buffer, uint16_t buffer_size)
{
		uint16_t ret = 0;
		ret = uart_tx_ringbuffer_push_data(uart_num, buffer, buffer_size);
		usart_interrupt_enable(COM_UART[COM_UART_NUM5], USART_INT_TC);
		return ret;
}

#if !defined(__MICROLIB)
//不使用微库的话就需要添加下面的函数
#if (__ARMCLIB_VERSION <= 6000000)
//如果编译器是AC5  就定义下面这个结构体
struct __FILE
{
    int handle;
};
#endif

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
#endif

void usart_thread_init(void)
{
#if 1
//    bsp_uart_periph_init(COM_UART_NUM5);
//    bsp_uart_periph_init(COM_UART_NUM2);
	bsp_uart_periph_init(COM_UART_NUM0);
	bsp_uart_periph_init(COM_UART_NUM1);
	bsp_uart_periph_init(COM_UART_NUM2);
	bsp_uart_periph_init(COM_UART_NUM5);
#ifndef ENABLE_USART_DMA
#else
	bsp_dma_usart_int(COM_UART_NUM0, true, true);
	bsp_dma_usart_int(COM_UART_NUM1, true, true);
	bsp_dma_usart_int(COM_UART_NUM2, true, true);
	bsp_dma_usart_int(COM_UART_NUM5, true, true);
	
	uart_rx_ringbuffer_init(COM_UART_NUM0, COM_UART0_RX_BUFFER_SIZE * 2);
	uart_rx_ringbuffer_init(COM_UART_NUM1, COM_UART1_RX_BUFFER_SIZE * 2);
	uart_rx_ringbuffer_init(COM_UART_NUM2, COM_USART2_RX_BUFFER_SIZE * 2);
	uart_rx_ringbuffer_init(COM_UART_NUM5, COM_USART5_RX_BUFFER_SIZE * 2);
	
	uart_tx_ringbuffer_init(COM_UART_NUM0, COM_UART0_RX_BUFFER_SIZE * 2);
	uart_tx_ringbuffer_init(COM_UART_NUM1, COM_UART1_RX_BUFFER_SIZE * 2);
	uart_tx_ringbuffer_init(COM_UART_NUM2, COM_USART2_RX_BUFFER_SIZE * 2);
	uart_tx_ringbuffer_init(COM_UART_NUM5, COM_USART5_RX_BUFFER_SIZE * 2);
#endif
#else
    usart_dma_test(COM_UART_NUM5);
#endif
}

char txbuffer[20] = "dma test success";
void usart_dma_test(uart_type_def uart_num)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* enable GPIO clock */
    rcu_periph_clock_enable( COM_UART_TX_GPIO_CLK[uart_num]);
    rcu_periph_clock_enable( COM_UART_RX_GPIO_CLK[uart_num]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_UART_CLK[uart_num]);

    /* connect port to USARTx_Tx */
    gpio_af_set(COM_UART_TX_PORT[uart_num], COM_UART_TX_GPIO_AF[uart_num], COM_UART_TX_PIN[uart_num]);

    /* connect port to USARTx_Rx */
    gpio_af_set(COM_UART_RX_PORT[uart_num], COM_UART_RX_GPIO_AF[uart_num], COM_UART_RX_PIN[uart_num]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(COM_UART_TX_PORT[uart_num], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_TX_PIN[uart_num]);
    gpio_output_options_set(COM_UART_TX_PORT[uart_num], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_TX_PIN[uart_num]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(COM_UART_RX_PORT[uart_num], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_UART_RX_PIN[uart_num]);
    gpio_output_options_set(COM_UART_RX_PORT[uart_num], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_UART_RX_PIN[uart_num]);

    /* USART configure */
    usart_deinit(COM_UART[uart_num]);
    usart_baudrate_set(COM_UART[uart_num],COM_UART_BAUD[uart_num]);
    usart_receive_config(COM_UART[uart_num], USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_UART[uart_num], USART_TRANSMIT_ENABLE);
    usart_enable(COM_UART[uart_num]);

    while(1) {
        /* deinitialize DMA channel2 (USART0 RX) */
        dma_deinit(COM_UART_DMA[uart_num], COM_UART_TX_DMA_CHANNEL[uart_num]);
        usart_flag_clear(COM_UART[uart_num], USART_FLAG_RBNE);
        usart_dma_receive_config(COM_UART[uart_num], USART_RECEIVE_DMA_ENABLE);

        dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
        dma_init_struct.memory0_addr = (uint32_t)COM_UART_RX_BUFFER_ADDR[uart_num];
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.number = 2;
        dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(COM_UART[COM_UART_NUM5]));
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
        dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
        dma_single_data_mode_init(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], &dma_init_struct);
        /* configure DMA mode */
        dma_circulation_disable(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num]);
        dma_channel_subperipheral_select(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], DMA_SUBPERI5);
        /* enable DMA channel2 */
        dma_channel_enable(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num]);

        /* wait DMA channel transfer complete */
        while(RESET == dma_flag_get(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], DMA_FLAG_FTF)){};
        dma_flag_clear(COM_UART_DMA[uart_num], COM_UART_RX_DMA_CHANNEL[uart_num], DMA_FLAG_FTF);

        //usart_dma_receive_config(COM_UART[uart_num], USART_RECEIVE_DMA_DISABLE);
        //printf("\n\r%s\n\r", rxbuffer);
    }
}


uint8_t test_buffer[20] = {0};
uint8_t test_buffer2[] = {0xff,0xff,0xff,0xff,0xff};
void usart_thread_run(void)
{
//    sprintf((char *)test_buffer, "%d %d %d %d %d",
//            get_switch_value(SWITCH_0),
//            get_switch_value(SWITCH_1),
//            get_switch_value(SWITCH_2),
//            get_switch_value(SWITCH_3),
//            get_switch_value(SWITCH_4)
//           );
    if (uart_rx_ringbuffer_pop_data(COM_UART_NUM2, test_buffer, 64) != 0)
    {
        bsp_usart_dma_send_data(COM_UART_NUM2, test_buffer, 64);
        delay_ms(20);
        mymemset((void *)test_buffer, 0x00, 64);
    }

    if (uart_rx_ringbuffer_pop_data(COM_UART_NUM5, test_buffer, 64) != 0)
    {
        //bsp_usart_dma_send_data(COM_UART_NUM5, test_buffer, 64);
				bsp_usart_buffer_send_data(COM_UART_NUM5,test_buffer,sizeof(test_buffer));
				bsp_usart_buffer_send_data(COM_UART_NUM5,test_buffer2,sizeof(test_buffer2));
        delay_ms(20);
        mymemset((void *)test_buffer, 0x00, 64);
    }


    //bsp_usart_dma_recv_data(COM_UART_NUM5, test_buffer, 30);
}




/* usart0*/
void DMA1_Channel7_IRQHandler()
{
		uint16_t send_num = 0;
    if(dma_interrupt_flag_get(DMA1, DMA_CH7, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INT_FLAG_FTF);
				send_num = uart_tx_ringbuffer_pop_data(COM_UART_NUM0, usart0_tx_buffer, COM_UART0_TX_BUFFER_SIZE);
				if(send_num)
				{
						dma_channel_disable(COM_UART_DMA[COM_UART_NUM0], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM0]);
						dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM0], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM0], send_num);
						dma_channel_enable(COM_UART_DMA[COM_UART_NUM0], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM0]);
				}
    }
}
void DMA1_Channel5_IRQHandler()
{
    if(dma_interrupt_flag_get(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0], DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0], DMA_INT_FLAG_FTF);
        dma_channel_enable(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0]);
    }
}
void USART0_IRQHandler()
{
    uint16_t recv_num = 0;
    if((RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) &&
            (RESET != usart_flag_get(USART0, USART_FLAG_IDLE)))
    {
			USART_STAT0(USART0);
			USART_DATA(USART0);
			bsp_led_toggle(LED1);
			recv_num = COM_UART0_RX_BUFFER_SIZE - dma_transfer_number_get(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0]);
			uart_rx_ringbuffer_push_data(COM_UART_NUM0, usart0_rx_buffer, recv_num);
			unsigned char data = 0x00;
			memcpy(usart0_rx_buffer, &data, sizeof(usart0_rx_buffer));
			dma_channel_disable(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0]);
			dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0], COM_UART0_RX_BUFFER_SIZE);
			dma_channel_enable(COM_UART_DMA[COM_UART_NUM0], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM0]);
    }

}

/* usart1*/

void DMA0_Channel6_IRQHandler()
{
		uint16_t send_num = 0;
    if(dma_interrupt_flag_get(DMA0, DMA_CH5, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA0, DMA_CH5, DMA_INT_FLAG_FTF);
				send_num = uart_tx_ringbuffer_pop_data(COM_UART_NUM1, usart1_tx_buffer, COM_UART1_TX_BUFFER_SIZE);
				if(send_num)
				{
						dma_channel_disable(COM_UART_DMA[COM_UART_NUM1], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM1]);
						dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM1], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM1], send_num);
						dma_channel_enable(COM_UART_DMA[COM_UART_NUM1], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM1]);
				}
    }
}
void DMA0_Channel5_IRQHandler()
{
    if(dma_interrupt_flag_get(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1], DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1], DMA_INT_FLAG_FTF);
        dma_channel_enable(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1]);
    }
}

void USART1_IRQHandler()
{
    uint16_t recv_num = 0;
    if((RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) &&
            (RESET != usart_flag_get(USART1, USART_FLAG_IDLE)))
    {
			USART_STAT1(USART1);
			USART_DATA(USART1);
			recv_num = COM_UART1_RX_BUFFER_SIZE - dma_transfer_number_get(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1]);
			uart_rx_ringbuffer_push_data(COM_UART_NUM1, usart1_rx_buffer, recv_num);
			dma_channel_disable(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1]);
			dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1], COM_UART1_RX_BUFFER_SIZE);
			dma_channel_enable(COM_UART_DMA[COM_UART_NUM1], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM1]);
    }

}

/*usart2*/
void USART2_IRQHandler()
{
    uint16_t recv_num = 0;
    if((RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE)) &&
            (RESET != usart_flag_get(USART2, USART_FLAG_IDLE)))
    {
			USART_STAT0(USART2);
			USART_DATA(USART2);
			recv_num = COM_USART2_RX_BUFFER_SIZE - dma_transfer_number_get(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2]);
			uart_rx_ringbuffer_push_data(COM_UART_NUM2, usart2_rx_buffer, recv_num);
			dma_channel_disable(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2]);
			dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2], COM_USART2_RX_BUFFER_SIZE);
			dma_channel_enable(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2]);

    }

}
/* usart2 RX dma IRQ */
void DMA0_Channel1_IRQHandler()
{
    if(dma_interrupt_flag_get(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2], DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2], DMA_INT_FLAG_FTF);
        dma_channel_enable(COM_UART_DMA[COM_UART_NUM2], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM2]);
    }
}
/* usart2 TX dma IRQ */
void DMA0_Channel3_IRQHandler()
{
		uint16_t send_num = 0;
    if(dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);
				send_num = uart_tx_ringbuffer_pop_data(COM_UART_NUM2, usart2_tx_buffer, COM_USART2_TX_BUFFER_SIZE);
				if(send_num)
				{
						dma_channel_disable(COM_UART_DMA[COM_UART_NUM2], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM2]);
						dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM2], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM2], send_num);
						dma_channel_enable(COM_UART_DMA[COM_UART_NUM2], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM2]);
				}
    }
}

/*usart5*/

void USART5_IRQHandler()
{
    uint16_t recv_num = 0;
		uint16_t send_num = 0;
    if((RESET != usart_interrupt_flag_get(USART5, USART_INT_FLAG_IDLE)) &&
            (RESET != usart_flag_get(USART5, USART_FLAG_IDLE)))
    {        
        USART_STAT0(USART5);
        USART_DATA(USART5);
        recv_num = COM_USART5_RX_BUFFER_SIZE - dma_transfer_number_get(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5]);
        uart_rx_ringbuffer_push_data(COM_UART_NUM5, usart5_rx_buffer, recv_num);
        dma_channel_disable(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5]);
        dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5], COM_USART5_RX_BUFFER_SIZE);
        dma_channel_enable(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5]);
    }
		
		if(RESET != usart_interrupt_flag_get(USART5,USART_INT_FLAG_TC))
		{
				send_num = uart_tx_ringbuffer_pop_data(COM_UART_NUM5, usart5_tx_buffer, COM_USART5_TX_BUFFER_SIZE);
				if(send_num)
				{

						usart_interrupt_flag_clear(USART5,USART_INT_FLAG_TC);
						dma_channel_disable(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5]);
						dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INT_FLAG_FTF);
						dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5], send_num);
						dma_memory_address_config(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5], DMA_MEMORY_0, (uint32_t)COM_UART_TX_BUFFER_ADDR[COM_UART_NUM5]);
						dma_channel_enable(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5]);
						usart_dma_transmit_config(COM_UART[COM_UART_NUM5], USART_TRANSMIT_DMA_ENABLE);
				}
				else
				{
						usart_interrupt_disable(COM_UART[COM_UART_NUM5], USART_INT_TC);
				}
			
		}
}
/* usart5 TX dma IRQ */
void DMA1_Channel6_IRQHandler()
{
		uint16_t send_num = 0;
    if(dma_interrupt_flag_get(DMA1, DMA_CH6, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA1, DMA_CH6, DMA_INT_FLAG_FTF);
				send_num = uart_tx_ringbuffer_pop_data(COM_UART_NUM5, usart5_tx_buffer, COM_USART5_TX_BUFFER_SIZE);
				if(send_num)
				{
						dma_channel_disable(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5]);
						dma_transfer_number_config(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5], send_num);
						dma_channel_enable(COM_UART_DMA[COM_UART_NUM5], COM_UART_TX_DMA_CHANNEL[COM_UART_NUM5]);
				}
    }
}

/* usart5 RX dma IRQ */
void DMA1_Channel2_IRQHandler()
{
    if(dma_interrupt_flag_get(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5], DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5], DMA_INT_FLAG_FTF);
        dma_channel_enable(COM_UART_DMA[COM_UART_NUM5], COM_UART_RX_DMA_CHANNEL[COM_UART_NUM5]);
    }
}

#if 1
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(COM_UART[COM_UART_NUM5], (uint8_t)ch);
    while(RESET == usart_flag_get(COM_UART[COM_UART_NUM5], USART_FLAG_TBE));
    return ch;
}

#endif

