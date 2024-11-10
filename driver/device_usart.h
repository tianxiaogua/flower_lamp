#ifndef USART_DEVICE_H
#define USART_DEVICE_H
#include "main.h"
#include "stdio.h"
#include "usart.h"

int fputc(int ch, FILE *f);
/*****************************************************************************
* @file   usart_device.c
* @brief  初始化串口和串口DMA中断
* @author Tianxiaogua
* @date   2023-04
****************************************************************************/
void strat_usart_interupt(void);

/*****************************************************************************
 * @file   usart_device.c
 * @brief  ͨDMA发送数据
 * @author Tianxiaogua
 * @date   2023-04
 ****************************************************************************/
void usart_driver_Transmit(uint8_t *buf, uint32_t len);
void usart2_driver_Transmit(uint8_t *buf, uint32_t len);

#endif

