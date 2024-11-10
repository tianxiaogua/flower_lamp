#include "driver_adc.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "driver_tool.h"

uint32_t driver_adc()
{
	HAL_ADC_Start(&hadc);    //启动ADC转换
	HAL_ADC_PollForConversion(&hadc,10); //等待转换完成，10ms表示超时时间
	uint32_t AD_Value = HAL_ADC_GetValue(&hadc);  //读取ADC转换数据（12位数据）
	//printf("ADC1_IN1 ADC value: %d\r\n",AD_Value);
	float Vol_Value = AD_Value*(3.3/4096);  //AD值乘以分辨率即为电压值
	//printf("ADC1_IN1 VOL value: %.2fV\r\n",Vol_Value);
	return 1;
}


//获取电压
float get_voltage(void)
{
	HAL_ADC_Start(&hadc);    //启动ADC转换
	HAL_ADC_PollForConversion(&hadc,10); //等待转换完成，10ms表示超时时间
	uint32_t AD_Value = HAL_ADC_GetValue(&hadc);  //读取ADC转换数据（12位数据）
	float Vol_Value = AD_Value*(3.3/4096)*2;  //AD值乘以分辨率即为电压值
	return Vol_Value;
}
