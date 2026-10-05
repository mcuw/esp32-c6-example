#include <Arduino.h>
#include "OneButton.h"

// pinouts: https://github.com/espressif/arduino-esp32/blob/master/variants/esp32c6/pins_arduino.h
#define PIN_INPUT GPIO_NUM_9

OneButton button(PIN_INPUT, true);

void onClick() {
  Serial.println("Button clicked");
  rgbLedWrite(PIN_RGB_LED, 0, 0, 255);
  delay(200);
  rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
}

void onDoubleClick() {
  Serial.println("Button double clicked");
  rgbLedWrite(PIN_RGB_LED, 0, 255, 0);
  delay(200);
  rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
}

void onLongPress() {
  Serial.println("Button long pressed");
  rgbLedWrite(PIN_RGB_LED, 255, 0, 0);
}

void onDuringLongPress() {
  Serial.println("Button is being long pressed");
}

void onLongPressStop() {
  Serial.println("Button long press stopped");

  rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
}

void setup()
{
  Serial.begin(115200);

  button.attachClick(onClick);
  button.attachDoubleClick(onDoubleClick);
  button.attachLongPressStart(onLongPress);
  button.attachDuringLongPress(onDuringLongPress);
  button.attachLongPressStop(onLongPressStop);

  button.setLongPressIntervalMs(400);
}

void loop()
{
  button.tick();
  
  // You can implement other code in here or just wait a while
  delay(10);
}