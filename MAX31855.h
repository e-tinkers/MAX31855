/*
  @file      MAX31855.h
             Arduino library for MAX31855 thermocouple-to-digital converter chip.
  @author    Henry Cheung
  @license   See LiCENSE
  @reference https://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
  @section   HISTORY
  V1.0.1     8 Jun, 2021 - Change begin() method API to report fault status
  V1.0.0     8 Jun, 2021 - Creation of this library
*/
#ifndef MAX31855_h
#define MAX31855_h

#include <Arduino.h>
#include <SPI.h>

class MAX31855 {
public:
  MAX31855(uint8_t chipSelect);
  uint8_t begin(void);
  uint8_t thermocoupleFault(void);
  float internalTemperature(void);

private:
  uint8_t _cs;
  uint8_t _fault;
  int32_t _spiRead();
};

MAX31855::MAX31855(uint8_t chipSelect=SS) {
  _cs = chipSelect;
  _fault = 0xFF;
}

uint8_t MAX31855::begin() {
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  SPI.begin();
  _spiRead();
  return _fault;
}

/*
 * Read 32-bit data from SPI. Clock speed is hardcoded as max clock speed
 * for MAX31855 is 5MHz.
 */
int32_t MAX31855::_spiRead()
{
    uint8_t data[4] = {0};

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    delayMicroseconds(1);
    SPI.transfer(data, 4);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();

    _fault = data[3] & 0x07;

    int32_t d = 0;
    d = (data[0] << 8) | data[1];
    d = (d << 8) | data[2];
    d = (d << 8) | data[3];
    return _d;
}

/*
 * Bits 31 - 18 (14-bit) are thermocouple temperature with 0.25/c precision(see datasheet page 10)
 * 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 * |  |                                   |
 * |  |-----------------------------------|------------ thermocouple temperature
 * |--------------------------------------------------- sign bit
*/
float MAX31855::thermocoupleTemperature()
{
  int32_t d = _spiRead();

  d = d >> 18;           // get raw temperature value
  if (d & 0x00020000) {  // check the sign bit to see if it is a negative value
    d = 0xFFFC0000 | (d & 0x0003FFFF);  //do the correction on sign value
  }
  return d/4;
}

/*
 *  Bits 15-4 provide a 12-bit internal(junction) temperature data with a precision of 0.0625/C
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *                                               |  |                             |
 *           sign bit ---------------------------|  |                             |
 * internal temperature ----------------------------|-----------------------------|
*/
float MAX31855::internalTemperature()
{
  int32_t d = _spiRead();

  d = (d & 0x0000FFFF) >> 4;  // get raw temperature value
  if (d & 0x00000800) {       // check the sign bit to see if it is a negative value
    d = (d & 0x000000FF) | 0xFFFFFF00;  //do the correction on sign value
  }
  return d/16;
}
#endif
