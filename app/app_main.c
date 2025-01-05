#include "stdio.h"
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

#include<stdlib.h>

enum led_work_satus
{
	work_shutdown = 0,
	work_in
};

typedef struct
{
	uint8 led_mode;
	uint8 led_word;
	uint8 cfg_led_nums;
	uint32 time;
}APP_CTR;

typedef struct 
{
	uint8 red;
	uint8 blue;
	uint8 green;
}LED_REG;

typedef struct 
{
	uint32 led_num;
	uint32 light_time;
	uint32 off_time;
	uint32 color;
}LED_CONTROL;

LED_CONTROL g_led_ctl[17];

APP_CTR g_app_control;
KEY_PROCESS_TypeDef key_value;

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

uint32 get_rand(uint32 seed, uint32 rand_max, uint32 rand_min)
{
	// srand(g_app_control.time);
	return rand() % (rand_max + 1 - rand_min) + rand_min;//随机数生成
}


int32 led_delay(int32 ms)
{
	for (int i=0; i<ms; i++) {
		if (g_app_control.led_word == work_in) {
			delay_ms(1);
		} else {
			return REV_ERR;
		}
	}
	return REV_OK;
}


void light_clean()
{
	for(int i=0;i<17;i++) {
		WS2812b_Set(i,0,0,0);
	}
}


void get_reg_data(uint32 color, LED_REG *data)
{
	data->red = color>>16;
	data->green = color>>8 & 0xFF00FF;
	data->blue = color & 0x0000FF;
}

void set_led_color(uint32 num, uint32 color)
{
	LED_REG data;
	get_reg_data(color, &data);
	WS2812b_Set(num,data.red, data.green, data.blue);
}


void light_mode7(uint32 color)
{
	while (1) {
		for (int i=0; i<g_app_control.cfg_led_nums; i++) {
			set_led_color(i, color);
		}
		if (led_delay(10000) == REV_ERR) return;
	}
}


void light_mode4()
{
	uint32 ligth_time = 0;
	uint32 all_time = 10*1000;
	uint32 all_color = 450;

	srand(g_app_control.time);

	for (int i=0; i<g_app_control.cfg_led_nums; i++) {
		g_led_ctl[i].led_num = i;
		g_led_ctl[i].color = RGB_16[get_rand(g_app_control.time, all_color, 0)];
		g_led_ctl[i].light_time = get_rand(g_app_control.time, all_time, 0);
		g_led_ctl[i].off_time = g_led_ctl[i].light_time + 3000;
		GUA_LOGI("color:%d begin:%d end:%d", g_led_ctl[i].color, g_led_ctl[i].light_time, g_led_ctl[i].off_time);
	}

	while (1) {
		for (int i=0; i<g_app_control.cfg_led_nums; i++) {
			if (g_led_ctl[i].light_time == ligth_time) {
				set_led_color(g_led_ctl[i].led_num, g_led_ctl[i].color);
			}
			if (g_led_ctl[i].off_time == ligth_time) {
				set_led_color(g_led_ctl[i].led_num, 0x000000); // 熄灭
			}
		}

		ligth_time++;
		if (ligth_time > all_time) { // 10秒钟
			ligth_time = 0;

			for (int i=0; i<g_app_control.cfg_led_nums; i++) {
				g_led_ctl[i].led_num = i;
				g_led_ctl[i].color = RGB_16[get_rand(g_app_control.time, all_color, 0)];
				g_led_ctl[i].light_time = get_rand(g_app_control.time, all_time, 0);
				g_led_ctl[i].off_time = g_led_ctl[i].light_time + 1000;
				GUA_LOGI("color:%d begin:%d end:%d", g_led_ctl[i].color, g_led_ctl[i].light_time, g_led_ctl[i].off_time);
			}
		}
		if (led_delay(1) == REV_ERR) return;
	}
}


void light_mode3(uint32 color)
{
	uint32 ligth_time = 0;
	uint32 all_time = 10*1000;
	srand(g_app_control.time);

	for (int i=0; i<g_app_control.cfg_led_nums; i++) {
		g_led_ctl[i].led_num = i;
		g_led_ctl[i].color = color;
		g_led_ctl[i].light_time = get_rand(g_app_control.time, all_time, 0);
		g_led_ctl[i].off_time = g_led_ctl[i].light_time + 3000;
		GUA_LOGI("color:%d begin:%d end:%d", g_led_ctl[i].color, g_led_ctl[i].light_time, g_led_ctl[i].off_time);
	}

	while (1) {
		for (int i=0; i<g_app_control.cfg_led_nums; i++) {
			if (g_led_ctl[i].light_time == ligth_time) {
				set_led_color(g_led_ctl[i].led_num, g_led_ctl[i].color);
			}
			if (g_led_ctl[i].off_time == ligth_time) {
				set_led_color(g_led_ctl[i].led_num, 0x000000); // 熄灭
			}
		}

		ligth_time++;
		if (ligth_time > all_time) { // 10秒钟
			ligth_time = 0;

			for (int i=0; i<g_app_control.cfg_led_nums; i++) {
				g_led_ctl[i].led_num = i;
				g_led_ctl[i].color = color;
				g_led_ctl[i].light_time = get_rand(g_app_control.time, all_time, 0);
				g_led_ctl[i].off_time = g_led_ctl[i].light_time + 1000;
				GUA_LOGI("color:%d begin:%d end:%d", g_led_ctl[i].color, g_led_ctl[i].light_time, g_led_ctl[i].off_time);
			}
		}

		if (led_delay(1) == REV_ERR) return;
	}
}

void light_mode1()
{
	uint32 led_1, led_2, led_3; 
	uint8 led_display1[20], led_display2[20], led_display3[20];

	srand(g_app_control.time);
	while (1) {
		led_1 = get_rand(g_app_control.time, 131072, 0);// * 4096;
		for (int i=g_app_control.cfg_led_nums; i>=0; i--) {
			led_display1[i] =  (led_1 >> i) & 1;
		}
		
		led_2 = get_rand(g_app_control.time, 131072, 0);// * 4096;
		for (int i=g_app_control.cfg_led_nums; i>0; i--) {
			led_display2[i] =  (led_2 >> i) & 1;
		}
		led_3 = get_rand(g_app_control.time, 131072, 0);// * 4096;
		for (int i=g_app_control.cfg_led_nums; i>0; i--) {
			led_display3[i] =  (led_3 >> i) & 1;
		}
		// GUA_LOGI("led_display:%d %d %d", led_1, led_2, led_3);

		for(int i=0;i<17;i++) {
			if (led_display1[i])
				WS2812b_Set(i,0,20,0);
			else 
				WS2812b_Set(i,0,0,0);
		}
		
		if (led_delay(300) == REV_ERR) return;

		for(int i=0;i<17;i++) {
			if (led_display2[i]) 
				WS2812b_Set(i,0,0,20);
			else 
				WS2812b_Set(i,0,0,0);
		}
		if (led_delay(300) == REV_ERR) return;

		for(int i=0;i<17;i++) {
			if (led_display3[i]) 
				WS2812b_Set(i,20,0,0);
			else 
				WS2812b_Set(i,0,0,0);
		}
		if (led_delay(300) == REV_ERR) return;
	}
}


void light_mode2()
{
	uint32 clolor = 0x0F0F0F;
	LED_REG led_rgb;
	get_reg_data(clolor, &led_rgb);
	uint8 min_value = find_min(led_rgb.red, led_rgb.green, led_rgb.blue);
	uint8 middle_value = find_middle(led_rgb.red, led_rgb.green, led_rgb.blue);
	uint8 max_value = find_max(led_rgb.red, led_rgb.green, led_rgb.blue);

	GUA_LOGI("min_value %d middle_value %d max_value %d", min_value, middle_value, max_value);
	
	while (1)
	{
		for (int i=0; i<=min_value; i++) {
			for(int j=0; j<17; j++) {
				WS2812b_Set(j, i,  i, i);
			}
			if (led_delay(80) == REV_ERR) return;
		}
		
		for (int i=0; i<=min_value; i++) {
			for(int j=0; j<17; j++) {
				WS2812b_Set(j,led_rgb.red-i,led_rgb.green-i,led_rgb.blue-i);
			}
			if (led_delay(80) == REV_ERR) return;
		}
	}
}


void led_task(void)
{
	while (1) {
		g_app_control.led_word = work_in;
		switch (g_app_control.led_mode) {
			case 0: light_mode2(); break;
			case 1: light_mode4(); break;
			case 2: light_mode3(0x0F0F01); break;
			case 3: light_mode3(0xA52A2A); break;
			case 4: light_mode3(0x0F0F0F); break;
			case 5: light_mode1(); break;

			// 连按两次展示常亮灯效
			case 8: light_mode7(0xF09F01); break;
			case 7: light_mode7(0xFFFFFF); break;
			default: break;
		}
		light_clean();
		led_delay(10);
	}
}

void task_main(void)
{
	uint8 histery_key = 0, key =0;
	uint8 key_sta;
	uint8 led_status = 0;
	float data_voltage_buf[10] = {0};
	int32 rev = 0;
	float data_average = 0;

	memset(&g_app_control, 0, sizeof(APP_CTR));
	g_app_control.cfg_led_nums = 17;

	key_config_init(&key_value);
    driver_init();
	WS2812b_Init();
    driver_register_delay_ms(osDelay);

	#if !TEST
	for (int i=0; i<10; i++) {
		data_voltage_buf[i] = get_voltage();
		GUA_LOGW("time:%d get voltage = %.2fV", i, data_voltage_buf[i]);
		delay_ms(10);
	}
    rev = driver_remove_max_and_min(data_voltage_buf, sizeof(data_voltage_buf)/sizeof(float));
	for (int i=0; i<rev; i++) {
		data_average += data_voltage_buf[i];
	}
	data_average = data_average/rev;

	GUA_LOGW("voltage = %.2fV", data_average);

    if (data_average < LOW_POWER_VOLTAGE) {
        GUA_LOGW("lowpower sleep!");
        set_lowpower_sleep();
        return;
    }
	#endif
    init_lowpower();
    init_gpio();
    WS2812b_Init();
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);

	// for (int i=0; i<17; i++) {
	// 	set_led_color(i, 0xF09F01);
	// } while (1);

    GUA_LOGI("class begin");
    while (1)
    {
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

		// 键值处理
		if (key_sta == EVENT_SHORT_CLICK) {
			g_app_control.led_word = work_shutdown;
			g_app_control.led_mode++;
			if(g_app_control.led_mode > 5){
				g_app_control.led_mode = 0;
			}
			GUA_LOGI("led_mode %d", g_app_control.led_mode);
		}

		if (key_sta == EVENT_DOUBLE_CLICK) {
			if (key_value.press_cnt == 2) { // 连按2次
				GUA_LOGE("go to led long mode");
				led_status++;
				if (led_status > 2) {
					led_status = 1;
				}
				g_app_control.led_word = work_shutdown;
				switch (led_status) {
					case 1: g_app_control.led_mode = 7; break;
					case 2: g_app_control.led_mode = 8; break;
					default: break;
				}
			}
		
		}

		if (key_sta == EVENT_LONG_CLICK) {
			GUA_LOGE("sleep");
			HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
			light_clean();
			set_lowpower_sleep();
			delay_ms(3000);
		}

		g_app_control.time++;
		if (g_app_control.time > 10000000) {
			g_app_control.time = 0;
		}
    }
}







