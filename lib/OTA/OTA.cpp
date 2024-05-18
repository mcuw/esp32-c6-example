#include <ArduinoOTA.h>
#include <WiFi.h>
#include "./OTA.h"

#define HOSTNAME_PREFIX "ESP32-"

bool loading = false;

OTA::OTA()
{
}

// get MAC of STA-, Serial- and ESPNow-MAC
String OTA::getHostname()
{
  String hostname(HOSTNAME_PREFIX);
  if (WiFi.getMode() != WIFI_STA)
  {
    WiFi.mode(WIFI_STA);
    delay(10);
  }
  hostname += WiFi.macAddress();
  hostname.replace(":", "");
  WiFi.disconnect(true);
  return hostname;
}

void OTA::startAP(const String &passphrase)
{
  String hostname(getHostname());
  WiFi.mode(WIFI_AP);
  delay(10);
  int channel = 1;
  int ssid_hidden = 0;
  int max_connection = 4;
  if (WiFi.softAP(hostname, passphrase, channel, ssid_hidden, max_connection, false, WIFI_AUTH_WPA3_PSK))
  {
    // Start OTA server.
    ArduinoOTA.setHostname(hostname.c_str()); // open http://esp32-XX.local
    ArduinoOTA.setPassword(passphrase.c_str());
    ArduinoOTA.begin();
  }
  else
  {
    neopixelWrite(PIN_NEOPIXEL, 255, 0, 0);
  }
}

boolean OTA::startSTA(const char *station_ssid, const char *station_passphrase, const String &passphrase)
{
  if (strcmp(station_ssid, "") == 0 || strcmp(station_passphrase, "") == 0)
  {
    return false;
  }

  String hostname(getHostname());
  // Serial.println(hostname);

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

    // Serial.print("New SSID: ");
    // Serial.println(WiFi.SSID());
  }

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
    // Serial.println("Failed to connect to WiFi station");
    return false;
  }

  // print IP Address
  // Serial.print("STA IP address: ");
  // Serial.println(WiFi.localIP());

  // Start OTA server
  ArduinoOTA.setHostname(hostname.c_str()); // open http://esp32-XX.local
  ArduinoOTA.setPassword(passphrase.c_str());

  ArduinoOTA
      .onStart([]()
               {
                loading = true;
                // Serial.println("Start updating ");
                neopixelWrite(PIN_NEOPIXEL, 255, 255, 255); })
      .onEnd([]()
             {
               loading = false;
               // Serial.println("\nEnd");
             });
  // .onProgress([](unsigned int progress, unsigned int total)
  //             {
  //               // Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //             })
  // .onError([](ota_error_t error)
  //          {
  //            Serial.printf("Error[%u]: ", error);
  //            // if (error == OTA_AUTH_ERROR)
  //            // {
  //            //   Serial.println("Auth Failed");
  //            // }
  //            // else if (error == OTA_BEGIN_ERROR)
  //            // {
  //            //   Serial.println("Begin Failed");
  //            // }
  //            // else if (error == OTA_CONNECT_ERROR)
  //            // {
  //            //   Serial.println("Connect Failed");
  //            // }
  //            // else if (error == OTA_RECEIVE_ERROR)
  //            // {
  //            //   Serial.println("Receive Failed");
  //            // }
  //            // else if (error == OTA_END_ERROR)
  //            // {
  //            //   Serial.println("End Failed");
  //            // }
  //          });

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

bool OTA::isLoading()
{
  return loading;
}
