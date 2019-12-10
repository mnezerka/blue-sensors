#define SDA_PORT PORTA
#define SCL_PORT PORTA
#define SDA_PIN 4
#define SCL_PIN 1

#define I2C_TIMEOUT 20
#define I2C_FASTMODE 1

#include <SoftWire.h>

SoftWire Wire1 = SoftWire();

void setup()
{
    Wire1.begin();

    Serial1.begin(9600);
    Serial1.println("\nI2C Scanner");
}

void loop()
{
    byte error, address;
    int nDevices;

    Serial1.println(F("Scanning I2C bus (7-bit addresses) ..."));

    nDevices = 0;
    for(address = 0; address < 127; address++ ) {

        Wire1.beginTransmission(address);
        error = Wire1.endTransmission();

        if (error == 0) {

            Serial1.print(F("I2C device found at address 0x"));
            if (address<16) {
                Serial1.print(F("0"));
            }
            Serial1.print(address,HEX);
            Serial1.println(F("  !"));

            nDevices++;
        } else if (error==4) {

           Serial1.print(F("Unknow error at address 0x"));

           if (address<16) {
                Serial1.print("0");
           }
           Serial1.println(address,HEX);
        }
    }

    if (nDevices == 0) {
        Serial1.println("No I2C devices found\n");
    } else {
        Serial1.println("done\n");
    }

   delay(2000);
}
