/**
  This example shows how to use an I2C device
 
  used features:
  - I2C
  - GY-BME-280 temperature and huminity sensor
  - adjust sea level pressure
    DE: https://meteonews.ch/de/Messwertekarte/CDE/Deutschland#tab=pres
    CH: https://meteonews.ch/de/Messwertekarte/CCH/Schweiz#tab=pres
    FR: https://meteonews.ch/fr/Carte_mesures/CFR/Frankreich#tab=pres
  
  
  Hardware connections:
  BME280 -> ESP32-C6
  GND -> GND
  3.3 -> 3.3
  SDA -> GPIO6 I2C-LP
  SCL -> GPIO7 I2C-LP
*/
#include <Arduino.h>
#include <Wire.h>
#include "SparkFunBME280.h"


const float SEALEVELPRESSURE_PA = 101300; // 1013.0 h Adjust the sea level pressure used for altitude calculations
const byte SENSOR_ADDRESS = 0x76;

BME280 mySensor;

void setup()
{
  Serial.begin(115200);
  neopixelWrite(PIN_RGB_LED, 0, 0, 0);

  Wire.begin(SDA1, SCL1);
  Wire.setClock(400000); //Increase to fast I2C speed!
  mySensor.setI2CAddress(SENSOR_ADDRESS);

  if (mySensor.beginI2C()) {
    Serial.println("Sensor connected");
    neopixelWrite(PIN_RGB_LED, 0, 0, 255);

    // Adjust the sea level pressure used for altitude calculations
    mySensor.setReferencePressure(SEALEVELPRESSURE_PA);
  } else {
    Serial.println("Sensor connect failed. Reconfigure address default 0x76 or check correct PIN connection.");
    neopixelWrite(PIN_RGB_LED, 255, 0, 0);

    delay(5000); // Wait 5 seconds to restart
    ESP.restart();
  }
}

void loop()
{
  Serial.print("Humidity: ");
  Serial.print(mySensor.readFloatHumidity(), 0);
  Serial.print("% ");

  Serial.print("Pressure: ");
  Serial.print(mySensor.readFloatPressure(), 0);
  Serial.print("Pa ");

  Serial.print("Temp: ");
  Serial.print(mySensor.readTempC(), 2);
  // Serial.print(mySensor.readTempF(), 2);
  Serial.print("°C ");

  Serial.println();

  delay(2000); // Wait 2 seconds for next scan
}