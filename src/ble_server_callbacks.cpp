#include "ble_server_callbacks.hpp"

extern Device_t device;

void MyServerCallbacks::onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
{
    Serial.printf("[INFO] Client Connected!\n");
    device.connected = true;
    BLEDevice::startAdvertising();
}

void MyServerCallbacks::onDisconnect(BLEServer *pServer)
{
    Serial.printf("[INFO] Client Disconnected!\n");
    device.connected = false;
}
