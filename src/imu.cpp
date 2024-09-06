#include "imu.hpp"
#include "kalman_filter.hpp"

Imu_t kimuData = {0};
ICM42688 IMU(SPI, 5);

// Variáveis para armazenar os offsets
static float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
static float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;

// Instâncias do filtro de Kalman para os dados do IMU
KalmanFilter kalmanAccX(0.1, 0.1, 0.1, 0.0);
KalmanFilter kalmanAccY(0.1, 0.1, 0.1, 0.0);
KalmanFilter kalmanAccZ(0.1, 0.1, 0.1, 0.0);
KalmanFilter kalmanGyrX(0.1, 0.1, 0.1, 0.0);
KalmanFilter kalmanGyrY(0.1, 0.1, 0.1, 0.0);
KalmanFilter kalmanGyrZ(0.1, 0.1, 0.1, 0.0);

void calibrateIMU()
{
    const int knumSamples = 100;
    float ax = 0, ay = 0, az = 0;
    float gx = 0, gy = 0, gz = 0;

    for (int i = 0; i < knumSamples; i++)
    {
        IMU.getAGT(); // Lê todos os dados do sensor de uma vez
        ax += IMU.accX();
        ay += IMU.accY();
        az += IMU.accZ();
        gx += IMU.gyrX();
        gy += IMU.gyrY();
        gz += IMU.gyrZ();
        delay(10);
    }

    accelOffsetX = ax / knumSamples;
    accelOffsetY = ay / knumSamples;
    accelOffsetZ = az / knumSamples;
    gyroOffsetX = gx / knumSamples;
    gyroOffsetY = gy / knumSamples;
    gyroOffsetZ = gz / knumSamples;
}

void imuInit(void)
{
    kimuData.acc.x = 0;
    kimuData.acc.y = 0;
    kimuData.acc.z = 0;
    kimuData.gyro.x = 0;
    kimuData.gyro.y = 0;
    kimuData.gyro.z = 0;
    kimuData.temp = 0;

    int status = IMU.begin();
    if (status < 0)
    {
        while (1)
        {
        }
    }

    IMU.setAccelFS(ICM42688::gpm8);
    IMU.setGyroFS(ICM42688::dps500);
    IMU.setAccelODR(ICM42688::odr12_5);
    IMU.setGyroODR(ICM42688::odr12_5);

    calibrateIMU();
}

void readIMU()
{
    IMU.getAGT(); // Lê todos os dados do sensor de uma vez

    // Aplicação dos offsets
    float rawAccX = IMU.accX() - accelOffsetX;
    float rawAccY = IMU.accY() - accelOffsetY;
    float rawAccZ = IMU.accZ() - accelOffsetZ;
    float rawGyrX = IMU.gyrX() - gyroOffsetX;
    float rawGyrY = IMU.gyrY() - gyroOffsetY;
    float rawGyrZ = IMU.gyrZ() - gyroOffsetZ;


    // Aplicar filtro de Kalman
    kimuData.acc.x = kalmanAccX.update(rawAccX);
    kimuData.acc.y = kalmanAccY.update(rawAccY);
    kimuData.acc.z = kalmanAccZ.update(rawAccZ);
    kimuData.gyro.x = kalmanGyrX.update(rawGyrX);
    kimuData.gyro.y = kalmanGyrY.update(rawGyrY);
    kimuData.gyro.z = kalmanGyrZ.update(rawGyrZ);
    kimuData.temp = IMU.temp();

}

void readAndTransmitimu(void)
{
    static uint16_t packet = 1;
    uint32_t sendTimer = 0;
    bool sensorActivated = false;

    // Chamar a função que aplica o filtro e lê os dados
    readIMU();

  
}
