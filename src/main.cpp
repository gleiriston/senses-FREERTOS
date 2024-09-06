#include "main.hpp"

TaskHandle_t statusTaskHandle = nullptr; // Definição da variável aqui
TaskHandle_t readTaskHandle = nullptr;
TaskHandle_t transmitTaskHandle = nullptr;
TaskHandle_t insoleTaskHandle = nullptr;
TaskHandle_t imuTaskHandle = nullptr;

void readTask(void *pvParameters);     // Declaração da função readTask
void transmitTask(void *pvParameters); // Declaração da função transmitTask
void insoleTask(void *pvParameters);   // Declaração da função insoleTask
void imuTask(void *pvParameters);      // Declaração da função imuTask

void setup()
{
    Serial.begin(kSERIAL_BAUD_RATE);
    pinMode(kBUTTON_PIN, INPUT_PULLUP);
    deviceSetup();
    statusInit();
    inaInit();
    imuInit();
    bleInit();

    Wire1.begin(4, 16);
    Wire1.setClock(400000);

    // Aumentar o tamanho da pilha para 4096 bytes
    xTaskCreatePinnedToCore(readTask, "Read Task", 4096, NULL, 1, &readTaskHandle, 1);
    xTaskCreatePinnedToCore(transmitTask, "Transmit Task", 4096, NULL, 1, &transmitTaskHandle, 1);
    xTaskCreatePinnedToCore(insoleTask, "Insole Task", 4096, NULL, 1, &insoleTaskHandle, 1);
    xTaskCreatePinnedToCore(imuTask, "IMU Task", 4096, NULL, 1, &imuTaskHandle, 1);
}

void loop()
{
   
}

void readTask(void *pvParameters)
{
    for (;;)
    {
        if (device.sync && device.active)
        {                      // Verifica se o dispositivo está sincronizado e ativo
            readAndTransmitInsole(); // Lê os dados dos sensores e transmite
            readAndTransmitbat();
            readAndTransmitimu();
            
        }
        vTaskDelay(pdMS_TO_TICKS(7)); // Aguarda 14ms
    }
}

void transmitTask(void *pvParameters)
{
    for (;;)
    {
        if (device.sync && device.active)
        { // Verifica se o dispositivo está sincronizado e ativo
            boolean _process = false;

            if (device.pendingUpdate)
            {
                device.pendingUpdate = false;
            }
            else
            {
                if (device.active)
                {
                    LED_OFF;
                    if (device.sendMode == smContinuous)
                    {
                        _process = true;
                    }
                    else
                    {
                        if (device.sampleCounter > 0)
                        {
                            device.sampleCounter--;
                            _process = true;
                        }
                    }
                }
            }

            if (_process)
            {
                readAndTransmitInsole();
                readAndTransmitimu();
                readAndTransmitbat();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(device.delayLoop));
    }
}

void insoleTask(void *pvParameters)
{
    for (;;)
    {
        if (device.sync && device.active)
        {                 // Verifica se o dispositivo está sincronizado e ativo
            readInsole(); // Lê os dados da palmilha
        }
        vTaskDelay(pdMS_TO_TICKS(device.delayLoop));
    }
}

void imuTask(void *pvParameters)
{
    for (;;)
    {
        if (device.sync && device.active)
        {                 // Verifica se o dispositivo está sincronizado e ativo
            IMU.getAGT(); // Lê os dados do IMU
        }
        vTaskDelay(pdMS_TO_TICKS(device.delayLoop));
    }
}

void app_main()
{
    setup();
    while (1)
    {
        loop();
    }
}