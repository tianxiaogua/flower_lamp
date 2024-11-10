#include "app_main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "driver_tool.h"
#include "driver.h"
#include "driver_lowpower.h"

int32 init_statis = 0;

//void task_led(void)
//{
//    while (1) {
//        if (init_statis == 1) {
//            delay_ms(1000);
//            GUA_LOGI("1111111111111111111111");
//        }
//        osDelay(100);
//    }
//}

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
    
    init_statis = 1;
    GUA_LOGI("class begin");
    while (1)
    {
        delay_ms(100);
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		delay_ms(100);
		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		int32_t data = get_key();
        GUA_LOGI("get data:%d", data);
        if (data == 1) {
                GUA_LOGI("in to low power!");
        }
    }
    
}







