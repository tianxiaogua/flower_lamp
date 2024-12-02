#ifndef driver_h
#define driver_h

#include "device_usart.h"
#include "driver_adc.h"
#include "driver_gpio.h"
#include "driver_tool.h"


#define  OSAL
#ifdef OSAL
#include "cmsis_os.h"
typedef osStatus (*delay_callback)(unsigned int);
#else
typedef void (*delay_callback)(unsigned int);
#endif


typedef struct
{
    delay_callback delay_ms;
    delay_callback delay_us;
    delay_callback delay_ns;
} DRIVER_CLASS;

extern DRIVER_CLASS g_driver_user;

int32 driver_init(void);
int32 driver_register_delay_ms(delay_callback delay_cb);
void delay_ms(uint32 ms);
void delay_us(uint32 us);
#endif
