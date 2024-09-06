#include "ble_characteristic_callbacks.hpp"

extern Device_t device;

void CharacteristicCallbacks::onWrite(BLECharacteristic *characteristic)
{
  std::string _rxMessage = characteristic->getValue();
  if (_rxMessage.length() > 0)
  {
    // Serial.printf("[INFO] Received message: ");
    for (unsigned int _counter = 0; _counter < _rxMessage.length(); _counter++)
    {
      Serial.printf("%02X ", _rxMessage[_counter]);
    }
    Serial.println();

    deviceProcessIncomingData(_rxMessage);
  }
  std::string rxHora = characteristic->getValue();
  if (rxHora.length() > 0)
  {
    int32_t unixTime = strtol(rxHora.c_str(), nullptr, 10);
    unixTime -= 3 * 3600; // Subtract 3 hours
    timeval epoch = {
        unixTime,
    };
    settimeofday((const timeval *)&epoch, nullptr);
  }
}
std::string getDateTime1()
/*{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t nowtime = tv.tv_sec;
  struct tm *nowtm = localtime(&nowtime);
  char tmbuf[64], buf[64];
  strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
  snprintf(buf, sizeof buf, "%s.%03ld", tmbuf, tv.tv_usec / 1000);
  return std::string(buf);
}*/

{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long long timestamp_ms = (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  return std::to_string(timestamp_ms);
}