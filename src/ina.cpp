#include "ina.hpp"
#include "kalman_filter.hpp"

INASetup_t inaSetup;

int selectorMatrix[MUX_SELECTORS] = {S3, S2, S1, S0};
int kinsoleChannelsValues[kINSOLE_CHANNELS];

// Ajuste dos parâmetros do filtro de Kalman
KalmanFilter kalmanFilters[kINSOLE_CHANNELS] = {
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0), 
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0),
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0),
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0),
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0),
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0),
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0),
    KalmanFilter(0.1, 0.2, 0.1, 0.0), KalmanFilter(0.1, 0.2, 0.1, 0.0)
};

// Parâmetros do filtro de média móvel
const int windowSize = 5;
float movingAverage[kINSOLE_CHANNELS][windowSize] = {{0}};
int movingAvgIndex = 0;

float applyMovingAverage(float newValue, int channel) {
    movingAverage[channel][movingAvgIndex % windowSize] = newValue;
    float sum = 0;
    for (int i = 0; i < windowSize; i++) {
        sum += movingAverage[channel][i];
    }
    movingAvgIndex++;
    return sum / windowSize;
}

byte multiplexMatrix[kINSOLE_CHANNELS][MUX_SELECTORS] = {
    {0, 0, 0, 0}, // 0  em decimal
    {0, 0, 0, 1}, // 1  em decimal
    {0, 0, 1, 0}, // 2  em decimal
    {0, 0, 1, 1}, // 3  em decimal
    {0, 1, 0, 0}, // 4  em decimal
    {0, 1, 0, 1}, // 5  em decimal
    {0, 1, 1, 0}, // 6  em decimal
    {0, 1, 1, 1}, // 7  em decimal
    {1, 1, 1, 1}, // 15 em decimal 1111
    {1, 1, 1, 0}, // 14 em decimal 1110
    {1, 1, 0, 1}, // 13 em decimal 1101
    {1, 1, 0, 0}, // 12 em decimal 1100
    {1, 0, 1, 1}, // 11 em decimal 1011
    {1, 0, 1, 0}, // 10 em decimal 1010
    {1, 0, 0, 1}, // 9  em decimal 1001
    {1, 0, 0, 0}  // 8  em decimal 1000
};

void inaInit(void)
{
    Wire.begin(INA_SDA_PIN, INA_SCL_PIN);
    Wire.setClock(INA_I2C_SPEED);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(EN, OUTPUT);

    digitalWrite(EN, LOW); // Multiplexer sempre habilitado

    pinMode(kLED_STATUS, OUTPUT);
    LED_OFF;

    inaSetup.avg = 0;
    inaSetup.vbusct = 0;
    inaSetup.vshct = 0;
    inaSetup.mode = INA_READING_TRIGGER;

    inaSetup.data = INA_DEFAULT_DATA_VALUE | (inaSetup.avg << 9) | (inaSetup.vbusct << 6) | (inaSetup.vshct << 3) | inaSetup.mode;

    inaSetup.d1 = (inaSetup.data & 0xFF00) >> 8;
    inaSetup.d2 = (inaSetup.data & 0x00FF);
}

// Função para aplicar zona morta
float applyDeadZone(float value, float threshold) {
    if (fabs(value) < threshold) {
        return 0.0;
    }
    return value;
}

void readAndTransmitInsole(void)
{
    
    static uint16_t packet = 1;
    static uint32_t lastTime = 0; // Armazena o último tempo de envio
    static uint32_t packetCount = 0; // Contador de pacotes enviados
    uint32_t currentTime = millis(); // Tempo atual em milissegundos

    uint32_t sendTimer = 0;
    bool sensorActivated = false;

    // Verificar se o dispositivo BLE está conectado antes de enviar
    if (!device.connected) {
        return; // Sai da função se o BLE não estiver conectado
    }

    // Buffer para a formatação de strings
    char combinedStr[512];  // Aumentar o tamanho do buffer para incluir todos os dados
    int index = 0;

    index += sprintf(&combinedStr[index], "%s ", getDateTime1().c_str());

    static float previousFilteredValues[kINSOLE_CHANNELS] = {0};

    for (int _channel = 0; _channel < kINSOLE_CHANNELS; _channel++)
    {
        int rawValue = kinsoleChannelsValues[_channel];
        float filteredValue = applyMovingAverage(kalmanFilters[_channel].update(rawValue), _channel);

        if (fabs(filteredValue - previousFilteredValues[_channel]) > 10)
        {
            previousFilteredValues[_channel] = filteredValue;
        }
        else
        {
            filteredValue = previousFilteredValues[_channel];
        }

        index += sprintf(&combinedStr[index], "S%d:%.f", _channel + 1, filteredValue);
        if (_channel < (kINSOLE_CHANNELS - 1))
        {
            combinedStr[index++] = kHL7_PAR_SEPARATOR;
        }
    }

    // Adicionar dados do IMU como vetores
    float imuAcc[3] = {kimuData.acc.x, kimuData.acc.y, kimuData.acc.z};
    float imuGyro[3] = {kimuData.gyro.x, kimuData.gyro.y, kimuData.gyro.z};

    // Aplicar zona morta aos valores do giroscópio
    float deadZoneThreshold = 0.1; // Defina um limiar adequado para sua aplicação
    imuGyro[0] = applyDeadZone(imuGyro[0], deadZoneThreshold);
    imuGyro[1] = applyDeadZone(imuGyro[1], deadZoneThreshold);
    imuGyro[2] = applyDeadZone(imuGyro[2], deadZoneThreshold);

    index += sprintf(&combinedStr[index], " | Acc: [%.2f, %.2f, %.2f]", imuAcc[0], imuAcc[1], imuAcc[2]);
    index += sprintf(&combinedStr[index], " | Gyro: [%.2f, %.2f, %.2f]", imuGyro[0], imuGyro[1], imuGyro[2]);

    // Imprimir os vetores de dados do IMU na saída serial
    Serial.print("IMU Acc: [");
    Serial.print(imuAcc[0], 2);
    Serial.print(", ");
    Serial.print(imuAcc[1], 2);
    Serial.print(", ");
    Serial.print(imuAcc[2], 2);
    Serial.println("]");

    Serial.print("IMU Gyro: [");
    Serial.print(imuGyro[0], 2);
    Serial.print(", ");
    Serial.print(imuGyro[1], 2);
    Serial.print(", ");
    Serial.print(imuGyro[2], 2);
    Serial.println("]");

    // Incrementar o contador de pacotes
    packetCount++;

    // Calcular e enviar a taxa de envio a cada segundo
    if (currentTime - lastTime >= 1000) {
        // Adicionar a taxa de pacotes à string combinada
        index += sprintf(&combinedStr[index], " | Pacotes por segundo: %d", packetCount);
        
        // Imprimir a taxa de pacotes na saída serial
        Serial.print("Pacotes por segundo: ");
        Serial.println(packetCount);

        // Resetar o contador de pacotes e atualizar o último tempo de envio
        packetCount = 0;
        lastTime = currentTime;
    }

    // Finalizar a string com nova linha
    index += sprintf(&combinedStr[index], "\r\n");

    // Enviar todos os dados combinados via BLE em uma única chamada
    pCharacteristic->setValue(combinedStr);
    pCharacteristic->notify();

    vTaskDelay(pdMS_TO_TICKS(device.delayLoop));
}




void readInsole()
{
    for (int _counterChannel = 0; _counterChannel < kINSOLE_CHANNELS; _counterChannel++)
    {
        for (int _counterSelector = 0; _counterSelector < MUX_SELECTORS; _counterSelector++)
        {
            digitalWrite(selectorMatrix[_counterSelector], multiplexMatrix[_counterChannel][_counterSelector]);
        }

        kinsoleChannelsValues[_counterChannel] = readInsoleChannel(INA_ADDRESS);
    }
}

int16_t readInsoleChannel(uint8_t pInaAddress)
{
    int16_t _kinsoleChannelValue = 0;
    uint8_t _index = 0;
    uint8_t _registerSize = INA_REGISTER_SIZE;
    uint8_t _registerBuffer[INA_REGISTER_SIZE] = {0, 0};

    Wire.beginTransmission(pInaAddress);
    Wire.write(INA_CONFIGURATION_REGISTER_ADDRESS);
    Wire.write(inaSetup.d1);
    Wire.write(inaSetup.d2);
    Wire.endTransmission();

    delayMicroseconds(kDEVICE_DELAY_LOOP_FAST_MS);

    Wire.beginTransmission(pInaAddress);
    Wire.write(INA_BUS_VOLTAGE_REGISTER_ADDRESS);
    Wire.endTransmission();

    _index = 0;
    Wire.requestFrom(pInaAddress, _registerSize);
    while (Wire.available() && _index < _registerSize)
    {
        _registerBuffer[_index] = Wire.read();
        _index++;
    }

    _kinsoleChannelValue = (_registerBuffer[0] << 8) | _registerBuffer[1];

    return _kinsoleChannelValue;
}
