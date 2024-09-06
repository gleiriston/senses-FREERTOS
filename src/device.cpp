#include "device.hpp"

extern TaskHandle_t statusTaskHandle; // Declaração da task do status
extern int kinsoleChannelsValues[kINSOLE_CHANNELS];

Device_t device;
Insole_t insole;

void deviceSetup(void)
{
    device.connected = false;
    device.active = false;
    device.sendMode = smContinuous;
    device.sampleCounter = 0;

    device.delayLoop = kDEVICE_DELAY_LOOP_FAST_MS;
    device.delayStatus = kDEFAULT_DELAY_STATUS_MS;
    device.sync = false;
    device.pendingUpdate = false;

    deviceReadConfig();
}

void deviceUpdate(void) {}

void deviceRestart(void)
{
    ESP_LOGE("Device", "Device Restart");
    LED_OFF;
    device.active = false;
    ESP.restart();
}

void deviceReadConfig(void)
{
    Feet_t _value;

    insole.name = "SENSESHOES-teste";

    EEPROM.begin(kMEM_SIZE);

    _value = (Feet_t)EEPROM.read(kMEM_FEET_ADDRESS);

    if ((_value != ftLeft) && (_value != ftRight) && (_value != ftNone))
    {
        _value = ftNone;
        EEPROM.write(kMEM_FEET_ADDRESS, _value);
        EEPROM.commit();
    }
    insole.name += _value;

    insole.application = "S";
    insole.application += _value;

    ESP_LOGI("Device", "Dispositivo: %s", insole.name.c_str());
}

void deviceSaveConfig(void)
{
    EEPROM.begin(kMEM_SIZE);
    EEPROM.write(kMEM_FEET_ADDRESS, insole.feet);
    EEPROM.commit();
    deviceRestart();
}

void deviceSetFeet(Feet_t pFeet)
{
    if ((pFeet == ftLeft) || (pFeet == ftRight) || (pFeet == ftNone))
    {
        insole.feet = pFeet;
        deviceSaveConfig();
    }
}

void deviceSendStatus(void) {}

void deviceProcessIncomingData(std::string pMessage)
{
    std::string _message = "";

    ESP_LOGI("Device", "Received Message: %s", pMessage.c_str());

    for (int i = 0; i < pMessage.length(); i++)
    {
        if (pMessage[i] != '\n' && pMessage[i] != '\r')
        {
            _message += pMessage[i];
        }
    }

    if (_message == "CT_RESET")
    {
        deviceRestart();
    }

    if (_message == "CC_FEET_LEFT")
    {
        deviceSetFeet(ftLeft);
    }

    if (_message == "CC_FEET_RIGHT")
    {
        deviceSetFeet(ftRight);
    }

    if (_message == "CC_FEET_NONE")
    {
        deviceSetFeet(ftNone);
    }

    if (_message == "CT_START")
    {
        if (device.sync)
        {
            device.active = true;
            LED_OFF;
            vTaskSuspend(statusTaskHandle);
        }
    }

    if (_message == "CT_STOP")
    {
        device.active = false;
        vTaskResume(statusTaskHandle);
    }

    if (_message == "CS_ON")
    {
        device.sync = true;
    }

    if (_message == "CS_OFF")
    {
        device.sync = false;
        device.active = false;
    }

    if (_message == "PD_LOW")
    {
        device.delayLoop = kDEVICE_DELAY_LOOP_LOW_MS;
    }

    if (_message == "PD_NORMAL")
    {
        device.delayLoop = kDEVICE_DELAY_LOOP_NORMAL_MS;
    }

    if (_message == "PD_FAST")
    {
        device.delayLoop = kDEVICE_DELAY_LOOP_FAST_MS;
    }

    if (_message == "PM_CONT")
    {
        device.sendMode = smContinuous;
        device.sampleCounter = 0;
    }

    if (_message == "PM_SAMPLE")
    {
        device.sendMode = smSample;
        device.sampleCounter = kDEVICE_SAMPLE_TEST;
    }

    device.pendingUpdate = true;
}
