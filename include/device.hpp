#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <string>
#include "ble_handler.hpp"
#include "ble_characteristic_callbacks.hpp"
#include <max17048.hpp>
#include "BLEDevice.h"
#include "esp_log.h"
#include "system_constants.hpp"

#define smContinuous 0
#define smSample 1

struct Device_t
{
    bool connected;
    bool active;
    int sendMode;
    int sampleCounter;
    int delayLoop;
    int delayStatus;
    bool sync;
    bool pendingUpdate;
};

struct Insole_t
{
    std::string name;
    std::string application;
    int feet;
};

enum Feet_t
{
    ftNone,
    ftLeft,
    ftRight
};

extern Device_t device;
extern Insole_t insole;

void deviceSetup(void);
void deviceUpdate(void);
void deviceRestart(void);
void deviceReadConfig(void);
void deviceSaveConfig(void);
void deviceSetFeet(Feet_t pFeet);
void deviceSendStatus(void);
void deviceProcessIncomingData(std::string pMessage);
void readAndTransmit(void);
void transmitBatteryData(void);
void transmitIMUData(void);
void transmitInsoleData(void);
