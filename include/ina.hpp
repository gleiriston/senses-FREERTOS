#pragma once

#include <Wire.h>
#include <Arduino.h>
#include "esp_log.h"
#include "system_constants.hpp"
#include <stdint.h>
#include "device.hpp"
#include "system_constants.hpp"
#include "kalman_filter.hpp"


typedef struct
{
    uint16_t avg;
    uint16_t vbusct;
    uint16_t vshct;
    uint16_t mode;
    uint16_t data;
    uint8_t d1;
    uint8_t d2;
} INASetup_t;

class Insole {
public:
    void readPressureData();
    float getRawPressure1();
    float getRawPressure2();
};

extern INASetup_t inaSetup;
extern int kinsoleChannelsValues[kINSOLE_CHANNELS];

void inaInit(void);
void readInsole(void);
int16_t readInsoleChannel(uint8_t pInaAddress);
