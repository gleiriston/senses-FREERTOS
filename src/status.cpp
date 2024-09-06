#include "status.hpp"

void statusInit(void)
{
    BaseType_t rc;
    LED_OFF;
    rc = xTaskCreatePinnedToCore(
        statusTask,
        "LED STATUS",
        kSTATUS_STACK_SIZE,
        nullptr,
        kSTATUS_TASK_PRIORITY,
        &statusTaskHandle,
        kSTATUS_TASK_CORE);

    assert(rc == pdPASS);
}

static void statusTask(void *pvParameters)
{
    static boolean _status = true;
    uint16_t _delay;

    for (;;)
    {
        _delay = device.delayStatus;
        if (_status)
        {
            LED_ON;
            if (device.connected)
            {
                if (device.sync)
                {
                    if (device.active)
                    {
                        LED_OFF;
                    }
                    else
                    {
                        _delay >>= 2;
                    }
                }
            }
            else
            {
                _delay >>= 2;
            }
            _status = false;
        }
        else
        {
            LED_OFF;
            if (device.connected)
            {
                if (device.sync)
                {
                    if (!device.active)
                    {
                        _delay >>= 1;
                    }
                }
            }
            else
            {
                _delay <<= 2;
            }
            _status = true;
        }
        vTaskDelay(pdMS_TO_TICKS(_delay));
    }
}
