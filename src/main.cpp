#include <Arduino.h>
#include <DebugUtils.h>

// pinouts: https://github.com/espressif/arduino-esp32/blob/master/variants/esp32c6/pins_arduino.h

// see https://github.com/wuxx/nanoESP32-C6

void setup()
{
  serialBegin(115200);
}

void loop()
{
  // The ESP32-C6 has an attenuator on the ADC, which defaults to 11dB. This causes the resolution to be reduced
  analogSetAttenuation(ADC_0db);

  // // blink
  // serialPrintLn("green");
  // neopixelWrite(PIN_NEOPIXEL, 0, 255, 0);
  // delay(1000);
}