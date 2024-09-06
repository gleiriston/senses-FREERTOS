#pragma once

#include "esp_log.h"
#include "device.hpp"
#include "system_constants.hpp"
#include "ble_handler.hpp"
#include <Arduino.h>
#include "ICM42688.h"
#include "kalman_filter.hpp"

// Estrutura para armazenar os dados do IMU
typedef struct
{
    struct
    {
        float x, y, z;
    } acc, gyro;
    float temp;
} Imu_t;

class IMU {
public:
    void readData();
    float getRawAccX();
    float getRawAccY();
    float getRawAccZ();
};

extern Imu_t kimuData;
extern ICM42688 IMU;

void imuInit(void);
void readIMU(void);
