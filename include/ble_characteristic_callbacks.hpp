#pragma once

#include <Arduino.h>
#include <sys/time.h>
#include "esp_log.h"
#include <BLEDevice.h>
#include "device.hpp"

class BLEHandler; // Forward declaration

class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
public:
    void onWrite(BLECharacteristic *characteristic) override;

private:
    BLEHandler *handler;
};
