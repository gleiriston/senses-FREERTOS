#pragma once

#include <Wire.h>
#include <Arduino.h>
#include "esp_log.h"
#include "device.hpp"
#include "system_constants.hpp"
#include <stdint.h>

extern float VCELL_bateria;
extern float SOC_bateria;


void Write_MAX17048(uint8_t addressRegister, uint16_t data);
uint16_t Read_MAX17048(uint8_t addressRegister);
