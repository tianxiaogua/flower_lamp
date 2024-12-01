#include "device_usart.h"
#include "driver_tool.h"

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch,1, 0xFFFF);
    return ch;
}

/*****************************************************************************
* @file   usart_device.c
* @brief  初始化串口和串口DMA中断
* @author Tianxiaogua
* @date   2023-04
****************************************************************************/
void strat_usart_interupt()
{
//	HAL_UART_Receive_DMA(&huart1, Rx_Buf, Rx_Max);  
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); 
	// HAL_UART_Receive_DMA(&huart2, Rx2_Buf, Rx_Max);  
	// __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); 
//  HAL_UART_Receive_DMA(&huart3, Rx3_Buf, Rx_Max);  
// 	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); 
}


/*****************************************************************************
 * @file   usart_device.c
 * @brief  ͨDMA发送数据
 * @author Tianxiaogua
 * @date   2023-04
 ****************************************************************************/
void usart_driver_Transmit(uint8_t *buf, uint32_t len)
{
  HAL_UART_Transmit_DMA(&huart1, buf, len);
  // HAL_UART_Transmit(&huart1, buf, len, 0XFFFF);
}
void usart2_driver_Transmit(uint8_t *buf, uint32_t len)
{
  HAL_UART_Transmit_DMA(&huart1, buf, len);
}

