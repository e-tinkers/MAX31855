
/*
  Demo program for using MAX31855 Arduino Library.
  Author       Henry Cheung
  License      See LiCENSE
  Datasheet    https://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
  Note         According to MAX31855 datasheet, It is strongly recommended to
               add a 10nF differential capacitor, placed across the T+ and T-
               pins, in order to filter noise on the thermocouple lines.
*/
#include <MAX31855.h>

#define CS 10

MAX31855 sensor(CS);

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  while (!sensor.begin()) {
    Serial.println(F("Fault detected, check your thermocouple connection"));
    delay(5000);
  };
}

void loop()
{

  Serial.print(F("Temperatures in Celsius - Thermocouple: "));
  Serial.print(sensor.thermocoupleTemperature());
  Serial.print(F(", Internal/Chip: "));
  Serial.println(sensor.internalTemperature());

  delay(1000);
}
