#include <Arduino.h>
#include <NimBLEDevice.h>

#define BEACON_ID "findflow_beacon"
#define DEEP_SLEEP_TIME 10000000

void setup()
{
  setCpuFrequencyMhz(80);
  NimBLEDevice::init(BEACON_ID);
  esp_bt_sleep_enable();

  NimBLEServer *pServer = NimBLEDevice::createServer();
  NimBLEAdvertisementData oAdvertisementData = NimBLEAdvertisementData();
  oAdvertisementData.setName(BEACON_ID);
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

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