#include "driver.h"

DRIVER_CLASS g_driver_user;

int32 driver_init(void)
{
    memset(&g_driver_user, 0, sizeof(DRIVER_CLASS));
    return REV_OK;
}

int32 driver_register_delay_ms(delay_callback delay_cb)
{
    if (delay_cb) {
        g_driver_user.delay_ms = delay_cb;
    } else {
        GUA_LOGE("callback = null!");
    }
		return REV_OK;
}

void delay_ms(uint32 ms)
{
    g_driver_user.delay_ms(ms);
}



#define CPU_FREQUENCY_MHZ    32		// STM32时钟主频
void delay_us(uint32 us)
{
    int last, curr, val;
    int temp;

    while (us != 0)
    {
        temp = us > 900 ? 900 : us;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        us -= temp;
    }
}
