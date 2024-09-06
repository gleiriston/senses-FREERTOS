#pragma once

#include <Arduino.h>
#include "esp_log.h"
#include <BLEDevice.h>
#include "device.hpp"

class MyServerCallbacks : public BLEServerCallbacks
{
public:
  void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override;
  void onDisconnect(BLEServer *pServer) override;
};
