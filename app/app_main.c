#include "app_main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "driver_tool.h"
#include "driver.h"
#include "driver_lowpower.h"
#include "RGB.h"
#include "ws2812b.h"


void light_mode()
{
	for(int i=0;i<17;i++)
	{
		WS2812b_Set(i,0,20,0);
	}
	HAL_Delay(600);
	for(int i=0;i<17;i++)
	{
		WS2812b_Set(i,20,0,0);
	}
	HAL_Delay(600);
	for(int i=0;i<17;i++)
	{
		WS2812b_Set(i,0,0,20);
	}
	HAL_Delay(600);
}


void task_main(void)
{
    driver_init();
    driver_register_delay_ms(HAL_Delay);
    float data = get_voltage();
    if (data < LOW_POWER_VOLTAGE) {
        GUA_LOGW("get voltage = %.2fV", data);
        GUA_LOGW("lowpower sleep!");
        #if TEST

        #else
            set_lowpower_sleep();
        #endif
        return;
    }
    init_lowpower();
    init_gpio();
    WS2812b_Init();
		light_mode();
    GUA_LOGI("class begin");
    while (1)
    {
       GUA_LOGI("go!");
			 delay_ms(100);
			 HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
			 delay_ms(100);
			 HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
			 int32_t data = get_key();
			 GUA_LOGI("get key:%d", data);
			 if (data == 1) {
							 GUA_LOGI("in to low power!");
			 }
    }
    
}







