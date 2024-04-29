#include <Arduino.h>
#include <OTA.h>
#include "credentials.h"
#include <DebugUtils.h>

OTA ota;

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
  // or protected but does not work yet
  // ota.begin(ap_default_psk, MY_SSID, PASSPHRASE);
}

void loop()
{
  ota.handle();

  // blink example
  // neopixelWrite(PIN_NEOPIXEL, 0, 0, 255);
  // delay(200);
  // neopixelWrite(PIN_NEOPIXEL, 0, 0, 0);
  // delay(200);
}