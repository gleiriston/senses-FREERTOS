#include "ble_handler.hpp"

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLECharacteristic *pImuCharacteristic = NULL;
BLECharacteristic *pBatteryCharacteristic = NULL;

extern int kinsoleChannelsValues[kINSOLE_CHANNELS];

extern TaskHandle_t statusTaskHandle; // Declaração da task do status

void bleInit(void)
{
    BLEDevice::init(insole.name.c_str());

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        DHTDATA_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(new CharacteristicCallbacks());

    pImuCharacteristic = pService->createCharacteristic(
        IMU_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    pImuCharacteristic->addDescriptor(new BLE2902());

    pBatteryCharacteristic = pService->createCharacteristic(
        BATTERY_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    pBatteryCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->start();
}
