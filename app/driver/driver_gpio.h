#ifndef driver_gpio_h
#define driver_gpio_h
#include "main.h"
#include "driver_tool.h"

void init_gpio(void);
int32_t get_key(void);
void set_gpio(int32 in);
#endif

