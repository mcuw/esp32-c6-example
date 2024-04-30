#include <Arduino.h>
#include <OTA.h>
#include <DebugUtils.h>
#include <ESPTelnetStream.h>
#include "credentials.h"

OTA ota;
ESPTelnetStream telnet;

void initDevBoard(uint8_t redValue = 0, uint8_t greenValue = 0, uint8_t blueValue = 0)
{
  // reset RGB-LED
  neopixelWrite(PIN_NEOPIXEL, redValue, greenValue, blueValue);
}

void setup()
{
  serialBegin(115200);
  initDevBoard(0, 0, 0);

  ota.begin(AP_PSK);
  // or use this for an existing WLAN:
  // ota.begin(ap_default_psk, MY_SSID, PASSPHRASE);

  telnet.begin();
}

void loop()
{
  ota.handle();
  telnet.loop();

  if (telnet.available() > 0)
  {
    int message = telnet.read();

    switch (message)
    {
    case 'a':
      neopixelWrite(PIN_NEOPIXEL, 0, 0, 255);
      telnet.println("light");
      break;

    case 'o':
      neopixelWrite(PIN_NEOPIXEL, 0, 0, 0);
      telnet.println("dark");
      break;
    }
  }

  telnet.println("loop");
  delay(200);
}