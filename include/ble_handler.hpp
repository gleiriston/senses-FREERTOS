#pragma once

#include <string>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ble_characteristic_callbacks.hpp"
#include "ble_server_callbacks.hpp"
#include "imu.hpp"
#include <Arduino.h>
#include "esp_log.h"
#include "system_constants.hpp"

void bleInit(void);
std::string getDateTime1(void);
void readAndTransmitInsole(void);
void readAndTransmitbat(void);
void readAndTransmitimu(void);

// Declaração das variáveis globais
extern BLEServer *pServer;
extern BLEService *pService;
extern BLECharacteristic *pCharacteristic;
extern BLECharacteristic *pImuCharacteristic;
extern BLECharacteristic *pBatteryCharacteristic;
