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


