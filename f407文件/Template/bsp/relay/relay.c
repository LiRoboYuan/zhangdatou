#include "relay.h"

static const uint32_t RELAY_PORT[RELAY_MAX_NUM] = {RELAY1_GPIO_PORT,RELAY2_GPIO_PORT,RELAY3_GPIO_PORT,RELAY4_GPIO_PORT};
static const uint32_t RELAY_PIN[RELAY_MAX_NUM]  = {RELAY1_PIN,RELAY2_PIN,RELAY3_PIN,RELAY4_PIN};
static const rcu_periph_enum RELAY_CLK[RELAY_MAX_NUM] = {RELAY1_GPIO_CLK,RELAY2_GPIO_CLK,RELAY3_GPIO_CLK,RELAY4_GPIO_CLK};

static void bsp_relay_gpio_init(relay_type_def relay){
	/* enable the led clock */
    rcu_periph_clock_enable(RELAY_CLK[relay]);
    /* configure led GPIO port */
    gpio_mode_set(RELAY_PORT[relay], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                  RELAY_PIN[relay]);
    gpio_output_options_set(RELAY_PORT[relay], GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ,
                            RELAY_PIN[relay]);
}
void bsp_relay_off(relay_type_def relay)
{
    gpio_bit_reset(RELAY_PORT[relay], RELAY_PIN[relay]);
}

void bsp_relay_on(relay_type_def relay)
{
    gpio_bit_set(RELAY_PORT[relay], RELAY_PIN[relay]);
}

void bsp_relay_init(void){
	for (relay_type_def relay = RELAY1; relay < RELAY_MAX_NUM; relay++)
    {
        bsp_relay_gpio_init(relay);
        bsp_relay_on(relay);//µÍµçÆ½£¬¹Ø±Õ
    }

}

//void bsp_relay_on(relay_type_def relay){
//	
//	
//}
