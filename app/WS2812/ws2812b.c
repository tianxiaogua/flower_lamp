#include "ws2812b.h"

#include "tim.h"

#define WS_H           45   // 1 ����Լ���ֵ
#define WS_L           25   // 0 ����Լ���ֵ
#define WS_REST        80   // ��λ�ź���������
#define LED_NUM       256   // WS2812�Ƹ���
#define DATA_LEN       24   // WS2812���ݳ��ȣ�������Ҫ24���ֽ�
#define WS2812_RST_NUM 50   // �ٷ���λʱ��Ϊ50us��40�����ڣ����������ʹ��50������

//�Դ����飬����Ϊ �Ƶ�����*24+��λ����
uint16_t WS2812b_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 

/**
 * ������WS2812�������ú���
 * ������num:�Ƶ�λ�ã�R��G��B�ֱ�Ϊ������ɫͨ�������ȣ����ֵΪ255
 * ���ã���������ÿһ��WS2812����ɫ
***/
void WS2812b_Set(uint16_t num,uint8_t R,uint8_t G,uint8_t B)
{
  uint32_t indexx=(num*(3*8));
  for (uint8_t i = 0;i < 8;i++)
  {
		//�������
		WS2812b_RGB_Buff[indexx+i]      = (G << i) & (0x80)?WS_H:WS_L;
		WS2812b_RGB_Buff[indexx+i + 8]  = (R << i) & (0x80)?WS_H:WS_L;
		WS2812b_RGB_Buff[indexx+i + 16] = (B << i) & (0x80)?WS_H:WS_L;
  }
}


//WS2812��ʼ������
void WS2812b_Init()
{
	//���ùر����е�
  for(int i=0;i<256;i++)
  {
		WS2812b_Set(i,0,0,0);
  }
  //���ã�����DMA���Դ��е�����ʵʱ��������ʱ���ıȽϼĴ���
  HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *)WS2812b_RGB_Buff,sizeof(WS2812b_RGB_Buff)/sizeof(uint16_t)); 
}

