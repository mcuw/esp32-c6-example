#include <Arduino.h>
#include <DebugUtils.h>

// pinouts: https://github.com/espressif/arduino-esp32/blob/master/variants/esp32c6/pins_arduino.h

// see https://github.com/wuxx/nanoESP32-C6

void initDevBoard(uint8_t redValue = 0, uint8_t greenValue = 0, uint8_t blueValue = 0)
{
  neopixelWrite(PIN_NEOPIXEL, redValue, greenValue, blueValue); // default off RGB-LED if (0, 0, 0)
}

void setup()
{
  serialBegin(115200);
}

void loop()
{
  // reset RGB-LED, ...
  initDevBoard(0, 0, 0);

  // The ESP32-C6 has an attenuator on the ADC, which defaults to 11dB. This causes the resolution to be reduced
  analogSetAttenuation(ADC_0db);

  // // blink
  // serialPrintLn("green");
  // neopixelWrite(PIN_NEOPIXEL, 0, 255, 0);
  // delay(1000);
}