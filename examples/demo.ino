
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
  sensor.begin();
}

void loop()
{
  uint8_t fault = sensor.thermocoupleFault();

  if (fault) {
    switch (fault) {
      case 1:
        Serial.println("Thermocouple is not connected");
        break;
      case 2:
        Serial.println("Thermocouple is short with Ground");
        break;
      case 4:
        Serial.println("Thermocouple is short with VCC");
      default:
        Serial.println("Unknown error");
    }
    while(1);
  }
  else {
    Serial.print("Temperatures in Celsius- Thermocouple: ");
    Serial.print(sensor.thermocoupleTemperature());
    Serial.print(", Internal: ");
    Serial.println(sensor.internalTemperature());
  }
  delay(1000);
}
