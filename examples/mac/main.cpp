#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  delay(1000);
}