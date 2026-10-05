/**
 * This example shows how to use an I2C device
 *
 * used features:
 * - I2C
 * - GY-BME-280 temperature and huminity sensor
 *
 */
#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA1, SCL1);
  rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
}

void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");
  nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
    rgbLedWrite(PIN_RGB_LED, 255, 0, 0);
  } else {
    Serial.println("done\n");
    rgbLedWrite(PIN_RGB_LED, 0, 255, 0);
  }
  
  delay(5000); // Wait 5 seconds for next scan
}