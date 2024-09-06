#pragma once

#include <Arduino.h>
// Definições de constantes para o projeto

static const uint32_t kfator_tensao_ADC = 1.25E-03;
static const uint32_t kfator_valor_SOC = 3.9065E-03;

// Definições de outros constantes, pinos, etc.

static const uint32_t kBUTTON_PIN = 13;
static const uint32_t kSERIAL_BAUD_RATE = 115200;
static const uint32_t kLED_STATUS = 17;
#define LED_OFF digitalWrite(kLED_STATUS, HIGH);
#define LED_ON digitalWrite(kLED_STATUS, LOW);
static const uint32_t kSTATUS_STACK_SIZE = 3000;
static const uint32_t kSTATUS_TASK_PRIORITY = 2;
static const uint32_t kSTATUS_TASK_CORE = 1;

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define DHTDATA_CHAR_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define IMU_CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define BATTERY_CHARACTERISTIC_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
static const uint32_t kHL7_PAR_SEPARATOR = char('/');
static const uint32_t kINSOLE_CHANNELS = 16;

static const uint32_t kDEVICE_DELAY_LOOP_FAST_MS = 1;
static const uint32_t kDEVICE_DELAY_LOOP_NORMAL_MS = 100;
static const uint32_t kDEVICE_DELAY_LOOP_LOW_MS = 500;
static const uint32_t kDEFAULT_DELAY_STATUS_MS = 1000;
static const uint32_t kDEVICE_SAMPLE_TEST = 5;
static const uint32_t kMEM_SIZE = 100;
static const uint32_t kMEM_FEET_ADDRESS = 1;

// Definições para max17048
static const uint32_t ADDRESS_MAX17048 = 0x36;
static const uint32_t VCELL = 0x02;
static const uint32_t SOC = 0x04;
static const uint32_t fator_tensao_ADC = 1.25E-03;
static const uint32_t fator_valor_SOC = 3.9065E-03;

// Definições para INA (INA219)
static const uint32_t INA_SDA_PIN = 21;
static const uint32_t INA_SCL_PIN = 22;
static const uint32_t INA_I2C_SPEED = 400000;
static const uint32_t S0 = 27;
static const uint32_t S1 = 26;
static const uint32_t S2 = 33;
static const uint32_t S3 = 32;
static const uint32_t EN = 25;
static const uint32_t INA_READING_TRIGGER = 0x0002;
static const uint32_t INA_DEFAULT_DATA_VALUE = 0x4000;
static const uint32_t MUX_SELECTORS = 4;
static const uint32_t INA_BUS_VOLTAGE_REGISTER_ADDRESS = 0x02;
static const uint32_t INA_CONFIGURATION_REGISTER_ADDRESS = 0x00;
static const uint32_t INA_REGISTER_SIZE = 2;
static const uint32_t INA_ADDRESS = 0x40;
static const uint32_t  SMOOTHING_WINDOW_SIZE = 5;


 const int weights[] = {0, 500,   600,  700,  800,  900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 6000, 6100, 6200, 6300, 6400, 6500, 6600, 6700, 6800, 6900, 7000, 7100, 7200, 7300, 7400, 7500, 7600, 7700, 7800, 7900, 8000};
 const int bits[] = {0, 0, 1520, 1637, 1708, 1740, 1766, 1806, 1821, 1844, 1870, 1906, 1926, 1944, 1956, 1965, 1970, 1978, 1983, 1993, 2001, 2008, 2012, 2020, 2025, 2030, 2036, 2039, 2044, 2049, 2053, 2057, 2060, 2062, 2064, 2066, 2068, 2071, 2075, 2078, 2080, 2083, 2084, 2085, 2086, 2088, 2088, 2090, 2092, 2094, 2095, 2098, 2104, 2105, 2108, 2110, 2113, 2114, 2115, 2117, 2117, 2118, 2119, 2120, 2122, 2124, 2126, 2127, 2128, 2130, 2133, 2135, 2140, 2142, 2144, 2145, 2147, 2150};



