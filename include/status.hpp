#pragma once

#include "device.hpp"
#include <Arduino.h>
#include "esp_log.h"
#include <system_constants.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern TaskHandle_t statusTaskHandle;

void statusInit(void);
static void statusTask(void *pvParameters);
void deviceUpdateStatus(void);
