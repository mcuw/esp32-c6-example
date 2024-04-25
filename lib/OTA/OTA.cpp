#include <ArduinoOTA.h>
#include <WiFi.h>
#include "./OTA.h"
#include "esp_mac.h" // required - exposes esp_mac_type_t values

#define HOSTNAME "ESP32-"

OTA::OTA()
{
}

String OTA::getDefaultMacAddress()
{
  String mac = "";

  unsigned char mac_base[6] = {0};

  if (esp_efuse_mac_get_default(mac_base) == ESP_OK)
  {
    char buffer[13]; // 6*2 characters for hex + 1 character for null terminator
    sprintf(buffer, "%02X%02X%02X%02X%02X%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
    mac = buffer;
  }

  return mac;
}

String OTA::getHostname()
{
  String hostname(HOSTNAME);
  hostname += getDefaultMacAddress();
  return hostname;
}

void OTA::startAP(const String &passphrase)
{
  String ssid(getHostname());

  // Go into software AP mode.
  WiFi.mode(WIFI_AP);
  delay(10);
  int channel = 1;
  int ssid_hidden = 0;
  int max_connection = 4;
  WiFi.softAP(ssid, passphrase, channel, ssid_hidden, max_connection, false, WIFI_AUTH_WPA3_PSK);

  // Start OTA server.
  ArduinoOTA.setHostname(ssid.c_str());
  ArduinoOTA.setPassword(passphrase.c_str());
  ArduinoOTA.begin();
}

boolean OTA::startSTA(const char *station_ssid, const char *station_passphrase, const String &passphrase)
{
  if (strcmp(station_ssid, "") == 0 || strcmp(station_passphrase, "") == 0)
  {
    return false;
  }

  String hostname(getHostname());

  // Check Wifi mode
  if (WiFi.getMode() != WIFI_STA)
  {
    WiFi.mode(WIFI_STA);
    delay(10);
  }

  // Compare file config with sdk config
  if (strcmp(WiFi.SSID().c_str(), station_ssid) == 0 && strcmp(WiFi.psk().c_str(), station_passphrase) == 0)
  {
    // Begin with sdk config
    WiFi.begin();
  }
  else
  {
    // Serial.println(F("WiFi config changed"));

    // Connect to WiFi station
    WiFi.begin(station_ssid, station_passphrase);

    // Serial.print(F("New SSID: "));
    // Serial.println(WiFi.SSID());
  }

  // Serial.println(F("Wait for WiFi connection."));

  // Give ESP 10 seconds to connect to station
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
  {
    // Serial.print(".");
    // Serial.print(WiFi.status());
    delay(500);
  }
  // Serial.println();

  // Check connection
  if (WiFi.status() != WL_CONNECTED)
  {
    // Serial.println(F("Failed to connect to WiFi station"));
    return false;
  }

  // print IP Address
  // Serial.print(F("STA IP address: "));
  // Serial.println(WiFi.localIP());

  // Start OTA server
  ArduinoOTA.setHostname(hostname.c_str());
  ArduinoOTA.setPassword(passphrase.c_str());
  ArduinoOTA.begin();

  return true;
}

void OTA::begin(const String &password, const char *station_ssid, const char *station_psk)
{
  if (!startSTA(station_ssid, station_psk, password))
  {
    startAP(password);
  }
}

void OTA::handle()
{
  ArduinoOTA.handle();
}
