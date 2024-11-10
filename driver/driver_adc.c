#include "driver_adc.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "driver_tool.h"

uint32_t driver_adc()
{
	HAL_ADC_Start(&hadc);    //����ADCת��
	HAL_ADC_PollForConversion(&hadc,10); //�ȴ�ת����ɣ�10ms��ʾ��ʱʱ��
	uint32_t AD_Value = HAL_ADC_GetValue(&hadc);  //��ȡADCת�����ݣ�12λ���ݣ�
	//printf("ADC1_IN1 ADC value: %d\r\n",AD_Value);
	float Vol_Value = AD_Value*(3.3/4096);  //ADֵ���Էֱ��ʼ�Ϊ��ѹֵ
	//printf("ADC1_IN1 VOL value: %.2fV\r\n",Vol_Value);
	return 1;
}


//��ȡ��ѹ
float get_voltage(void)
{
	HAL_ADC_Start(&hadc);    //����ADCת��
	HAL_ADC_PollForConversion(&hadc,10); //�ȴ�ת����ɣ�10ms��ʾ��ʱʱ��
	uint32_t AD_Value = HAL_ADC_GetValue(&hadc);  //��ȡADCת�����ݣ�12λ���ݣ�
	float Vol_Value = AD_Value*(3.3/4096)*2;  //ADֵ���Էֱ��ʼ�Ϊ��ѹֵ
	return Vol_Value;
}
