## MAX31855 Arduino Library

A simple and lightweigth Arduino library for MAX31855 thermocouple-to-digital convert chip. I created this library because some of similar library doesn't handle the negative temperature correctly.

### Basic Usage in Simpliest Way

```
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
  if (!fault()) {
    Serial.print("Temperatures in Celsius- ");
    Serial.print(sensor.thermocoupleTemperature());
  }
  delay(1000);
}
```

### Note
According to [MAX31855 datasheet](https://datasheets.maximintegrated.com/en/ds/MAX31855.pdf), It is strongly recommended to add a 10nF differential capacitor, placed across the T+ and T-pins, in order to filter noise on the thermocouple lines.

In additional to thermocouple temperature reading, MAX31855 also provides an output for its internal(junction) temperature reading, which can be access via `MAX31855::internalTemperature()`, the internal chip junction temperature provides the reading between 127 and -55 degree Celsius with precision of 0.0625/C. See datasheet for more details. 

### License
MIT License. See LICENSE file for details.
