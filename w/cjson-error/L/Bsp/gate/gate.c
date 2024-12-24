#include "gate.h"

void gate_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

void com_gate_for_usart(COM_GATE_T com_gate)
{
    switch(com_gate)
    {
        case RS232_GATE:
            RS2251_A(0);
            RS2251_B(0);
            RS2251_C(0);
            break;
        case MOTO1_GATE:
            RS2251_A(1);
            RS2251_B(1);
            RS2251_C(0);
            break;
        case MOTO2_GATE:
            RS2251_A(0);
            RS2251_B(1);
            RS2251_C(0);
            break;
        case MOTO3_GATE:
            RS2251_A(1);
            RS2251_B(0);
            RS2251_C(0);
            break;
        case ESP32_GATE:
            RS2251_A(0);
            RS2251_B(0);
            RS2251_C(1);
            break;
        case RS485_GATE:
            RS2251_A(1);
            RS2251_B(0);
            RS2251_C(1);
            break;
        default:
            break;
    }
}
