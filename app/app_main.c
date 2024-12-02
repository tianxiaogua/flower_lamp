#include "app_main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "driver_tool.h"
#include "driver.h"
#include "driver_lowpower.h"
#include "RGB.h"
#include "ws2812b.h"
#include "driver_key.h"

typedef struct
{
	uint8 led_mode;

}APP_CTR;

APP_CTR g_app_control;
KEY_PROCESS_TypeDef key_value;
void light_mode()
{
	for(int i=0;i<17;i++)
	{
		WS2812b_Set(i,0,20,0);
	}
	HAL_Delay(200);
	for(int i=0;i<17;i++)
	{
		WS2812b_Set(i,20,0,0);
	}
	HAL_Delay(200);
	for(int i=0;i<17;i++)
	{
		WS2812b_Set(i,0,0,20);
	}
	HAL_Delay(200);
}

// 函数：返回三个数中的最小值
int find_min(int a, int b, int c) {
    return (a < b ? (a < c ? a : c) : (b < c ? b : c));
}
// 函数：返回三个数中的中间值
int find_middle(int a, int b, int c) {
    return (a >= b ? (a <= c ? a : (b >= c ? b : c)) : (a >= c ? a : (b <= c ? b : c)));
}
// 函数：返回三个数中的最大值
int find_max(int a, int b, int c) {
    return (a >= b ? (a >= c ? a : c) : (b >= c ? b : c));
}



void light_mode2()
{
	uint32 clolor = 0xFFFFFF;
	
	uint8 clolor_R = clolor>>16;
	uint8 clolor_G = clolor>>8 & 0xFF00FF;
	uint8 clolor_B = clolor & 0x0000FF;
	uint8 min_value = find_min(clolor_R, clolor_G, clolor_B);
	uint8 middle_value = find_middle(clolor_R, clolor_G, clolor_B);
	uint8 max_value = find_max(clolor_R, clolor_G, clolor_B);

	GUA_LOGI("min_value %d middle_value %d max_value %d", min_value, middle_value, max_value);
	
	for (int i=0; i<=min_value; i++)
	{
		for(int j=0;j<17;j++)
		{
			WS2812b_Set(j,clolor_R-i,clolor_G-i,clolor_B-i);
		}
		GUA_LOGI("i:%d",i);
		delay_ms(10);
	}
}

void led_task(void)
{
	while (1) {
		switch (g_app_control.led_mode)
		{
		case 0: break;
		case 1: light_mode2(); break;
		default:
			break;
		}
		
	}
}


void task_main(void)
{
	uint8 histery_key = 0, key =0;
	uint8 key_sta;
	memset(&g_app_control, 0, sizeof(APP_CTR));
	key_config_init(&key_value);
    driver_init();
	WS2812b_Init();
    driver_register_delay_ms(osDelay);
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
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    GUA_LOGI("class begin");
    while (1)
    {
			//  int32_t data = get_key();
			//  //GUA_LOGI("get key:%d", data);
			//  if (data == 1) {
			// 	g_app_control.led_mode++;
			// 	if(g_app_control.led_mode > 4) 
			// 	{
			// 		g_app_control.led_mode = 0;
			// 	}
				
			//  }
			//  delay_ms(300);
			key = get_key();
			 if (key == 1) {
				if (histery_key != key) {
					key_value.flag.key_state = KEY_STATE_PRESS;	//按下
					key_value.flag.check = 1;
					key_value.time_continus = 0;		//按键持续时间置零，准备开始计时
					histery_key = key;
				}
			 } else {
				if (histery_key != key) {
					key_value.flag.key_state = KEY_STATE_RELEASE;	//松开
					key_value.flag.check = 1;
					key_value.time_idle = 0;			//按键空闲时间置零，准备开始计时
					histery_key = key;
				}
			 }
			
			 key_Process(&key_value);
			 key_sta = key_scan(&key_value);
			 delay_ms(10);

			 if (key_sta == EVENT_SHORT_CLICK) {
				g_app_control.led_mode++;
				if(g_app_control.led_mode > 4) 
				{
					g_app_control.led_mode = 0;
				}
			 }
    }
    
}







