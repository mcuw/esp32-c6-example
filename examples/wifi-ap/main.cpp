#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  // Go into software AP mode.
  WiFi.mode(WIFI_AP);
  delay(10);
  int channel = 1;
  int ssid_hidden = 0;
  int max_connection = 4;
  WiFi.softAP("ESP32-c6", "passphrase", channel, ssid_hidden, max_connection, false, WIFI_AUTH_WPA3_PSK);
}

void loop()
{
  neopixelWrite(PIN_NEOPIXEL, 255, 0, 0);
  delay(200);
  neopixelWrite(PIN_NEOPIXEL, 0, 0, 255);
  delay(200);
}