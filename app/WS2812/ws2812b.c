#include "ws2812b.h"

#include "tim.h"

#define WS_H           45   // 1 码相对计数值
#define WS_L           25   // 0 码相对计数值
#define WS_REST        80   // 复位信号脉冲数量
#define LED_NUM       256   // WS2812灯个数
#define DATA_LEN       24   // WS2812数据长度，单个需要24个字节
#define WS2812_RST_NUM 80   // 官方复位时间为50us（40个周期），保险起见使用50个周期

//显存数组，长度为 灯的数量*24+复位周期
uint16_t WS2812b_RGB_Buff[LED_NUM*DATA_LEN+WS2812_RST_NUM] = {0}; 

/**
 * 函数：WS2812单灯设置函数
 * 参数：num:灯的位置，R、G、B分别为三个颜色通道的亮度，最大值为255
 * 作用：单独设置每一个WS2812的颜色
***/
void WS2812b_Set(uint16_t num,uint8_t R,uint8_t G,uint8_t B)
{
  uint32_t indexx=(num*(3*8));
  for (uint8_t i = 0;i < 8;i++)
  {
		//填充数组
		WS2812b_RGB_Buff[indexx+i]      = (G << i) & (0x80)?WS_H:WS_L;
		WS2812b_RGB_Buff[indexx+i + 8]  = (R << i) & (0x80)?WS_H:WS_L;
		WS2812b_RGB_Buff[indexx+i + 16] = (B << i) & (0x80)?WS_H:WS_L;
  }
}


//WS2812初始化函数
void WS2812b_Init()
{
	//设置关闭所有灯
  for(int i=0;i<256;i++)
  {
		WS2812b_Set(i,0,0,0);
  }
  //作用：调用DMA将显存中的内容实时搬运至定时器的比较寄存器
  HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,(uint32_t *)WS2812b_RGB_Buff,sizeof(WS2812b_RGB_Buff)/sizeof(uint16_t)); 
}

