#include <Arduino.h>
#include <OTA.h>
#include "credentials.h"

OTA ota;
String ap_default_psk(AP_PSK); // Default PSK.

void setup()
{
  // reset RGB-LED
  neopixelWrite(PIN_NEOPIXEL, 0, 0, 0);

  ota.begin(ap_default_psk);
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