#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

#define serbaud 9600
#define simbaud 38400
#define simreset 2
#define dallaspin 5

const char server[] PROGMEM = PUSH_HOST;

const char c0[]     PROGMEM = "AT";
const char c1[]     PROGMEM = "AT+IPR=38400"; //The speed must be set also here!
const char c2[]     PROGMEM = "AT+CBC";
const char c3[]     PROGMEM = "AT+CSTT=\"internet\",\"\",\"\"";
const char c4[]     PROGMEM = "AT+CIICR";
const char c5[]     PROGMEM = "AT+CIPSTATUS";
const char c6[]     PROGMEM = "AT+CIFSR";
const char c7[]     PROGMEM = "AT+CIPSEND";
const char c8[]     PROGMEM = "AT+CIPQSEND=1";
const char c9[]     PROGMEM = "AT+CIPCLOSE";
const char c10[]    PROGMEM = "AT+CIPSHUT";
const char c11[]    PROGMEM = "AT+CSCLK=2";
const char c12[]    PROGMEM = "AT+CIPSTART=\"TCP\",\"";
const char c16[]    PROGMEM = "POST /api/readings HTTP/1.1";
const char c17[]    PROGMEM = "\r\n";
const char c18[]    PROGMEM = "Host: ";
const char c19[]    PROGMEM = "User-Agent: ATTinySIM800";
const char c20[]    PROGMEM = "Connection: close";
const char c21[]    PROGMEM = "Content-Type: application/json";
const char c22[]    PROGMEM = "Content-Length: ";
const char c23[]    PROGMEM = ">>> ";
const char c24[]    PROGMEM = "\",80";
const char c25[]    PROGMEM = "";
const char c26[]    PROGMEM = "";
const char c27[]    PROGMEM = "";
const char c28[]    PROGMEM = "";
const char c29[]    PROGMEM = "key=iJcWrhwlMOhJ&sensor=VD000&class1=DALLAS_1&value1=";
const char c30[]    PROGMEM = ">>> ";
const char c31[]    PROGMEM = "Modem reset";
const char c32[]    PROGMEM = "AT+CIPSEND?";
const char c33[]    PROGMEM = "AT+CIPCLOSE";

const char *const string_table[] PROGMEM = {c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, "", server, "", c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27, c28, c29, c30, c31, c32, c33 };
char buffer[64];
bool fail = false;
bool modify = false;
bool success = false;
char endcom[2];
unsigned long timer;
const unsigned long wait = INTERVAL_READINGS;

const int VALUE_NA = -8888;

OneWire ow(dallaspin);
DallasTemperature dallas(&ow);

void prepare(byte which) {
   strcpy_P(buffer, (char *)pgm_read_word(&(string_table[which])));
}

void sendCommand()
{
   Serial1.print(">>> ");
   Serial1.print(buffer);
   Serial.print(buffer);

   if (modify)
   {
     prepare(14); Serial1.print(buffer); Serial.print(buffer);
     prepare(24); Serial1.print(buffer); Serial.print(buffer);
   }

   Serial1.println();
   Serial.println();
}

void receiveCommand()
{
   unsigned long stamp;
   success = false;
   char thischar = 0;
   stamp = millis();
   byte i = 1;

   while (!Serial.available() && millis() < stamp + 5000) {
     delay (50);
   }
   if (millis() > stamp + 5000) fail = true;
   delay (100);

   while (Serial.available()) {
     thischar = Serial.read();
     Serial1.write(thischar);

     buffer[i] = thischar;
     if (buffer[i - 1] == 'O' && buffer[i] == 'K') success = true;

     i++;
     if (i >= 60) i = 1;
   }
   Serial1.println();
}

void communicate()
{
   if (!fail) {
     sendCommand();
     receiveCommand();
   }
}


void trasmit()
{
    // 1. Prepare payload
    dallas.requestTemperatures();

    String payload = "";
    payload += "{";
    payload += "\"device\": \"";
    payload +=  DEVICE_ID;
    payload += "\", ";
    payload += "\"readings\": [";

    // get all sensor readings and generate appropriate reading attributes
    Serial1.print("Number of dallas devices: ");
    Serial1.println(dallas.getDeviceCount());
    for(int i = 0; i < dallas.getDeviceCount(); i++)
    {
        DeviceAddress deviceAddress;

        // get unique address of sensor
        dallas.getAddress(deviceAddress, i);

        // get string representation of a dallas device address
        String deviceAddressStr = "";
        for (uint8_t i = 0; i < 8; i++)
        {
            // zero pad the address if necessary
            if (deviceAddress[i] < 16) deviceAddressStr += "0";
            deviceAddressStr += String(deviceAddress[i], HEX);
        }

        // get temperature
        float temp = dallas.getTempCByIndex(i);
        if (temp == -127) temp = VALUE_NA;

        if (i > 1) payload += ",";

        payload += "{\"address\": \"" + deviceAddressStr + "\"";
        payload += ", \"t\": " + String(temp);
        payload += "}";
     }

     payload += "]}";

    // 2. Send HTTP request
    Serial.print("POST "); Serial.print(PUSH_PATH); Serial.print(" HTTP/1.1");
    Serial.print("\r\n");
    Serial.print("Host: ");
    Serial.print(PUSH_HOST);
    Serial.print("User-Agent: ATTinySIM800");
    Serial.print("Connection: close");
    Serial.print("Content-Type: application/json");
    Serial.print("Content-Length: ");
    Serial.print(payload.length());

    Serial.print("\r\n\r\n");

    Serial1.println(payload);
    Serial.print(payload);

    Serial.print("\r\n\r\n");

    Serial.print(endcom);
    Serial.print(endcom);
}

void gprs() {
   timer = millis();

   prepare(0);  Serial.println(buffer); delay(500);

   prepare(0);  communicate(); //AT
   prepare(1);  communicate(); //AT+IPR=9600
   prepare(2);  communicate(); //AT+CBC
   prepare(3);  communicate(); //AT+CSTT="internet","",""
   prepare(4);  communicate(); //AT+CIICR
   prepare(5);  communicate(); //AT+CIPSTATUS
   prepare(6);  communicate(); //AT+CIFSR
   modify = true;
   prepare(12); communicate(); //AT+CIPSTART="TCP","",80
   modify = false;
   if (!fail) delay (3000);
   prepare(0);  communicate(); //AT
   prepare(0);  communicate(); //AT
   prepare(0);  communicate(); //AT
   prepare(8);  communicate(); //AT+CIPQSEND=1
   prepare(7);  communicate(); //AT+CIPSEND
   if (!fail) {
     delay(2000);
     trasmit();
     delay(2000);
   }
   prepare(32); communicate(); //AT+CIPSEND?
   prepare(33); communicate(); //AT+CIPCLOSE
   prepare(10); communicate(); //AT+CIPSHUT
   prepare(11); communicate(); //AT+CSCLK=2
   if (!success) fail = true;

   if (fail)
   {
     fail = false;
     prepare(31); Serial1.println(buffer); //Modem reset
     pinMode(simreset, OUTPUT);
     digitalWrite(simreset, LOW);
     delay(50);
     pinMode(simreset, INPUT_PULLUP);
   }
}


void setup() {
   endcom[0] = 0x1a;
   endcom[1] = '\0';

   Serial1.begin(serbaud);
   Serial.begin(simbaud);

   dallas.begin();

   gprs();
}

void loop() {
   delay(100);
   if (millis() - timer > wait * 1000) gprs();
}

