#include <Arduino.h>
#include <NimBLEDevice.h>

#define BEACON_ID "findflow_beacon"
#define DEEP_SLEEP_TIME 10000000

uint32_t getMeasuredVoltage()
{
  // do multiple measurements and average them
  uint32_t sum = 0;
  for (int i = 0; i < 5; i++)
  {
    sum += analogReadMilliVolts(0);
  }
  return sum / 5;
}

float getBatteryPercentage()
{
  uint32_t u_r = getMeasuredVoltage();
  uint32_t u_batt = ((10 + 13) * u_r) / 13;

  float percentage = (float)(u_batt - 3500) / (4200 - 3500) * 100;
  if (percentage > 100)
  {
    percentage = 100;
  }
  return percentage;
}

void setup()
{
  setCpuFrequencyMhz(80);
  NimBLEDevice::init(BEACON_ID);
  esp_bt_sleep_enable();

  NimBLEServer *pServer = NimBLEDevice::createServer();
  NimBLEAdvertisementData oAdvertisementData = NimBLEAdvertisementData();
  oAdvertisementData.setName(BEACON_ID);
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

  uint8_t battData[1];
  // battData[0] = (int)getBatteryPercentage(); // Length of the data
  battData[0] = (int)getBatteryPercentage();

  oAdvertisementData.setServiceData(BLEUUID((uint16_t)0x180F), std::string((char *)battData, 1));

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setAdvertisementType(BLE_GAP_CONN_MODE_NON);
  pAdvertising->setMinInterval(32); // 100ms (160 * 0.625ms)
  pAdvertising->setMaxInterval(64); // 150ms (240 * 0.625ms)

  pAdvertising->start();
  delay(200);
  pAdvertising->stop();

  setCpuFrequencyMhz(10);

  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);
  esp_deep_sleep(DEEP_SLEEP_TIME);
}

void loop()
{
  // empty loop
}