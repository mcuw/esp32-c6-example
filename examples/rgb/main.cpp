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
  serialPrintln("red");
  neopixelWrite(PIN_NEOPIXEL, 255, 0, 0);
  delay(1000);

  serialPrintln("green");
  neopixelWrite(PIN_NEOPIXEL, 0, 255, 0);
  delay(1000);

  serialPrintln("blue");
  neopixelWrite(PIN_NEOPIXEL, 0, 0, 255);
  delay(1000);
}