#include <Arduino.h>
#include <OTA.h>
#include <DebugUtils.h>
#include "credentials.h"

OTA ota;

void initDevBoard(uint8_t redValue = 0, uint8_t greenValue = 0, uint8_t blueValue = 0)
{
  // reset RGB-LED
  rgbLedWrite(PIN_RGB_LED, redValue, greenValue, blueValue);
}

void setup()
{
  serialBegin(115200);
  initDevBoard(0, 0, 0);

  ota.begin(AP_PSK);
  // or use this for an existing WLAN:
  // ota.begin(ap_default_psk, MY_SSID, PASSPHRASE);
}

void loop()
{
  ota.handle();

  // blink example
  // rgbLedWrite(PIN_RGB_LED, 0, 0, 255);
  // delay(200);

  // rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
  // delay(100);
}