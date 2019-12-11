#ifndef TINYBME280_H
#define TINYBME280_H

/*
#define SDA_PORT PORTA
#define SCL_PORT PORTA
#define SDA_PIN 4
#define SCL_PIN 1
*/

#define I2C_TIMEOUT 20
#define I2C_FASTMODE 1

#include <stdint.h>
#include <SoftWire.h>

class TinyBME280 {

public:

    TinyBME280(SoftWire*);

    // Can be called before begin()
    void setI2Caddress(uint8_t address);

    // Sets Normal mode, no upsampling, and reads the chip calibrations
    void begin();

    bool isAvailable();

    // Temperature in DegC, resolution is 0.01 DegC
    // Output value of “5123” equals 51.23 DegC
    int32_t temperature();

    // Pressure in Pa as unsigned 32 bit integer
    // Output value of “96386” equals 96386 Pa = 963.86 hPa
    uint32_t pressure();

    // Humidity in %RH, resolution is 0.01%RH
    // Output value of “4653” represents 46.53 %RH
    uint32_t humidity();


private:

    int16_t read16();
    int32_t read32();

    // Take a pointer to the SoftWire instance
    SoftWire* _swire;

    int16_t T[4], P[10], H[7];
    int32_t BME280t_fine;

    int BME280address = 118;
};

TinyBME280::TinyBME280(SoftWire* swire) {
    _swire = swire;
}

int16_t TinyBME280::read16() {
    uint8_t lo, hi;
    lo = _swire->read(); hi = _swire->read();
    return hi<<8 | lo;
}

int32_t TinyBME280::read32() {
    uint8_t msb, lsb, xlsb;
    msb = _swire->read(); lsb = _swire->read(); xlsb = _swire->read();
    return (uint32_t)msb<<12 | (uint32_t)lsb<<4 | (xlsb>>4 & 0x0F);
}

// Can be called before BME280begin
void TinyBME280::setI2Caddress (uint8_t address) {
    BME280address = address;
}

// Must be called once at start
void TinyBME280::begin() {
    delay(2);

    // Set the mode to Normal, no upsampling
    _swire->beginTransmission(BME280address);
    _swire->write(0xF2);                             // ctrl_hum
    _swire->write(0b00000001);
    _swire->write(0xF4);                             // ctrl_meas
    _swire->write(0b00100111);

    // Read the chip calibrations.
    _swire->write(0x88);
    _swire->endTransmission();
    _swire->requestFrom(BME280address, 26);
    for (int i=1; i<=3; i++) T[i] = read16();     // Temperature
    for (int i=1; i<=9; i++) P[i] = read16();     // Pressure
    _swire->read();  // Skip 0xA0
    H[1] = (uint8_t)_swire->read();                  // Humidity

    _swire->beginTransmission(BME280address);
    _swire->write(0xE1);
    _swire->endTransmission();
    _swire->requestFrom(BME280address, 7);
    H[2] = read16();
    H[3] = (uint8_t)_swire->read();
    uint8_t e4 = _swire->read(); uint8_t e5 = _swire->read();
    H[4] = ((int16_t)((e4 << 4) + (e5 & 0x0F)));
    H[5] = ((int16_t)((_swire->read() << 4) + ((e5 >> 4) & 0x0F)));
    H[6] = ((int8_t)_swire->read()); // 0xE7

    // Read the temperature to set BME280t_fine
    temperature();
}

bool TinyBME280::isAvailable() {
    _swire->beginTransmission(BME280address);
    //int result = _swire->write(0x00);
    byte result = _swire->endTransmission();
    return (result == 0x00);
}

// Returns temperature in DegC, resolution is 0.01 DegC
// Output value of “5123” equals 51.23 DegC
int32_t TinyBME280::temperature () {
    _swire->beginTransmission(BME280address);
    _swire->write(0xFA);
    _swire->endTransmission();
    _swire->requestFrom(BME280address, 3);
    int32_t adc = read32();

    // Compensate
    int32_t var1, var2;
    var1 = ((((adc>>3) - ((int32_t)((uint16_t)T[1])<<1))) * ((int32_t)T[2])) >> 11;
    var2 = ((((adc>>4) - ((int32_t)((uint16_t)T[1]))) * ((adc>>4) - ((int32_t)((uint16_t)T[1])))) >> 12);
    var2 = (var2 * ((int32_t)T[3])) >> 14;
    BME280t_fine = var1 + var2;

    return (BME280t_fine * 5 +128) >> 8;
}

// Returns pressure in Pa as unsigned 32 bit integer
// Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t TinyBME280::pressure () {
    _swire->beginTransmission(BME280address);
    _swire->write(0xF7);
    _swire->endTransmission();
    _swire->requestFrom(BME280address, 3);
    int32_t adc = read32();

    if (adc == 0x800000) // value in case temp measurement was disabled
        return -999;

    // Compensate
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)BME280t_fine)>>1) - (int32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)P[6]);
    var2 = var2 + ((var1*((int32_t)P[5]))<<1);
    var2 = (var2>>2) + (((int32_t)P[4])<<16);
    var1 = (((P[3] * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)P[2]) * var1)>>1))>>18;
    var1 = ((((32768+var1))*((int32_t)((uint16_t)P[1])))>>15);
    if (var1 == 0) return 0;
    p = (((uint32_t)(((int32_t)1048576) - adc) - (var2>>12)))*3125;
    if (p < 0x80000000) p = (p << 1) / ((uint32_t)var1);
    else p = (p / (uint32_t)var1) * 2;
    var1 = (((int32_t)P[9]) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)P[8]))>>13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + P[7]) >> 4));

    return p;
}

// Humidity in %RH, resolution is 0.01%RH
// Output value of “4653” represents 46.53 %RH
uint32_t TinyBME280::humidity () {
    _swire->beginTransmission(BME280address);
    _swire->write(0xFD);
    _swire->endTransmission();
    _swire->requestFrom(BME280address, 2);
    uint8_t hi = _swire->read(); uint8_t lo = _swire->read();
    int32_t adc = (uint16_t)(hi<<8 | lo);
    // Compensate
    int32_t var1; 
    var1 = (BME280t_fine - ((int32_t)76800));
    var1 = (((((adc << 14) - (((int32_t)H[4]) << 20) - (((int32_t)H[5]) * var1)) +
    ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)H[6])) >> 10) * (((var1 *
    ((int32_t)H[3])) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
    ((int32_t)H[2]) + 8192) >> 14));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)H[1])) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);
    return (uint32_t)((var1>>12)*25)>>8;
}

#endif // TINYBME280_H
