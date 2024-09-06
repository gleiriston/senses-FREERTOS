#include "max17048.hpp"
#include <cmath>  // Inclua a biblioteca cmath para usar fabs()

float VCELL_bateria = 0;
float SOC_bateria = 0;

void Write_MAX17048(uint8_t addressRegister, uint16_t data)
{
    uint8_t D[3] = {0, 0, 0};

    D[0] = addressRegister;
    D[1] = (data & 0xFF00) >> 8;
    D[2] = (data & 0x00FF);

    Wire1.beginTransmission((uint8_t)ADDRESS_MAX17048);
    Wire1.write(D, sizeof(D));
    Wire1.endTransmission();
}

uint16_t Read_MAX17048(uint8_t addressRegister)
{
    uint8_t D[2] = {0, 0};
    uint16_t valueDigital = 0;

    Wire1.beginTransmission((uint8_t)ADDRESS_MAX17048);
    Wire1.write(addressRegister);
    Wire1.endTransmission();

    Wire1.requestFrom(ADDRESS_MAX17048, 2);
    Wire1.readBytes(D, 2);

    valueDigital = (D[0] << 8) | D[1];
    return valueDigital;
}

void readAndTransmitbat(void)
{
    static uint16_t packet = 1;
    uint32_t sendTimer = 0;
    bool sensorActivated = false;

    // Tentar calcular sem o deslocamento de bits
    double VCELL_bateria_atual = Read_MAX17048(VCELL) * (1.25E-03 / 16.0);
    double SOC_bateria_atual = Read_MAX17048(SOC) * 3.9065E-03;

    static bool primeira_notificacao = true;
    bool enviar_notificacao = false;
     if ( std::abs(SOC_bateria_atual - SOC_bateria) >= 0.01)
    {
        enviar_notificacao = true;
        VCELL_bateria = VCELL_bateria_atual;
        SOC_bateria = SOC_bateria_atual;
    }
if (enviar_notificacao || primeira_notificacao)
    {
        std::string strVCELL = std::to_string(VCELL_bateria).substr(0, 4);
        std::string strSOC = std::to_string(SOC_bateria).substr(0, 4);

        std::string str = getDateTime1() + "~CELL: " + strVCELL + "~SOC:" + strSOC + "%" + "\r";
        pCharacteristic->setValue(str.c_str());
        pCharacteristic->notify();
        primeira_notificacao = false;
        vTaskDelay(pdMS_TO_TICKS(device.delayLoop));
    }
}
