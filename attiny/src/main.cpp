#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

#define SERBAUD 9600
#define SIMBAUD 38400
#define SIMRESET 2
#define DALLASPIN 5

#define BUFFER_SIZE 64
#define MAX_BODY_SIZE 255

const char server[] PROGMEM = PUSH_HOST;

#define IX_AT 0
const char c0[]     PROGMEM = "AT";
#define IX_AT_IPR 1
const char c1[]     PROGMEM = "AT+IPR=38400"; //The speed must be set also here!
#define IX_AT_CBC 2
const char c2[]     PROGMEM = "AT+CBC";
#define IX_AT_CSTT 3
const char c3[]     PROGMEM = "AT+CSTT=\"internet\",\"\",\"\"";
#define IX_AT_CIICR 4
const char c4[]     PROGMEM = "AT+CIICR";
#define IX_AT_CIPSTATUS 5
const char c5[]     PROGMEM = "AT+CIPSTATUS";
#define IX_AT_CIFSR 6
const char c6[]     PROGMEM = "AT+CIFSR";
#define IX_AT_CIPSEND 7
const char c7[]     PROGMEM = "AT+CIPSEND";
#define IX_AT_CIPSEND1 8
const char c8[]     PROGMEM = "AT+CIPQSEND=1";
#define IX_AT_CIPCLOSE 9
const char c9[]     PROGMEM = "AT+CIPCLOSE";
#define IX_AT_CIPSHUT 10
const char c10[]    PROGMEM = "AT+CIPSHUT";
#define IX_AT_CSCLK2 11
const char c11[]    PROGMEM = "AT+CSCLK=2";
#define IX_AT_CIPSTART 12
const char c12[]    PROGMEM = "AT+CIPSTART=\"TCP\",\"";
#define IX_HTTP_METHOD 16
const char c16[]    PROGMEM = "POST ";
#define IX_EOL 17
const char c17[]    PROGMEM = "\r\n";
#define IX_HTTP_HEADER_HOST 18
const char c18[]    PROGMEM = "Host: ";
#define IX_HTTP_HEADER_USER_AGENT 19
const char c19[]    PROGMEM = "User-Agent: ArduinoSIM800";
#define IX_HTTP_HEADER_CONNECTION_CLOSE 20
const char c20[]    PROGMEM = "Connection: close";
#define IX_HTTP_HEADER_CONTENT_TYPE 21
const char c21[]    PROGMEM = "Content-Type: application/json";
#define IX_HTTP_HEADER_CONTENT_LENGTH 22
const char c22[]    PROGMEM = "Content-Length: ";
#define IX_HTTP_PROTOCOL 23
const char c23[]    PROGMEM = " HTTP/1.1";
#define IX_HTTP_BODY_DEVICE 24
const char c24[]    PROGMEM = "{\"device\": \"";
#define IX_HTTP_BODY_READINGS 25
const char c25[]    PROGMEM = "\",  \"readings\": [";
#define IX_HTTP_BODY_READING_ADDR 26
const char c26[]    PROGMEM = "{\"address\": \"";
#define IX_HTTP_BODY_READING_TEMP 27
const char c27[]    PROGMEM = "\", \"t\": ";
#define IX_HTTP_BODY_READING_ADDR_END 28
const char c28[]    PROGMEM = "";
#define IX_HTTP_BODY_READING_END 29
const char c29[]    PROGMEM = "}";
#define IX_HTTP_BODY_END 30
const char c30[]    PROGMEM = "]}";
const char c31[]    PROGMEM = "Modem reset";
#define IX_AT_CIPSENDQ 32
const char c32[]    PROGMEM = "AT+CIPSEND?";

const char *const string_table[] PROGMEM = {c0, c1, c2, c3, c4, c5, c6,
c7, c8, c9, c10, c11, c12, "", server, "", c16, c17, c18, c19, c20,
c21, c22, c23, c24, c25, c26, c27, c28, c29, c30, c31, c32};

char buffer[BUFFER_SIZE];
char body[MAX_BODY_SIZE];
char tmp[10];
bool fail = false;
bool modify = false;
bool success = false;
char endcom[2];
unsigned long timer;
const unsigned long wait = INTERVAL_READINGS;

const int VALUE_NA = -8888;

OneWire ow(DALLASPIN);
DallasTemperature dallas(&ow);

void prepare(byte which) {
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[which])));
}

void prepareAndSend(byte which) {
    prepare(which);
    Serial.print(buffer);
}

void resetModem()
{
    fail = false;
    Serial1.println("Modem reset");
    pinMode(SIMRESET, OUTPUT);
    digitalWrite(SIMRESET, LOW);
    delay(50);
    pinMode(SIMRESET, INPUT_PULLUP);
}

void sendCommand()
{
   Serial1.print(">>> ");
   Serial1.print(buffer);
   Serial.print(buffer);

   // modify flag just adds host name and port after current command
   if (modify) {
        Serial.print(PUSH_HOST); Serial.print("\","); Serial.print(PUSH_PORT);
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

     // check if last chars form word "OK"
     if (buffer[i - 1] == 'O' && buffer[i] == 'K') success = true;

     i++;
     if (i >= BUFFER_SIZE - 4) i = 1;
   }
   Serial1.println();
}

void communicate(byte ix)
{
    prepare(ix);

    if (!fail) {
        sendCommand();
        receiveCommand();
    }
}

// send http body
void prepareBody()
{
    body[0] = '\0';

    Serial1.print("Number of dallas devices: ");
    Serial1.println(dallas.getDeviceCount());

    dallas.requestTemperatures();

    prepare(IX_HTTP_BODY_DEVICE); strcat(body, buffer);
    strcat(body, DEVICE_ID);
    prepare(IX_HTTP_BODY_READINGS); strcat(body, buffer);

    // get all sensor readings and generate appropriate reading attributes
    for(int i = 0; i < dallas.getDeviceCount(); i++)
    {
        if (i > 1) Serial.print(",");

        DeviceAddress deviceAddress;

        // get unique address of sensor
        dallas.getAddress(deviceAddress, i);

        prepare(IX_HTTP_BODY_READING_ADDR); strcat(body, buffer);
        // get string representation of a dallas device address
        for (uint8_t i = 0; i < 8; i++)
        {
            // zero pad the address if necessary
            if (deviceAddress[i] < 16) strcat(body, "0");
            sprintf(tmp, "%x", deviceAddress[i]);
            strcat(body, tmp);
        }
        prepare(IX_HTTP_BODY_READING_TEMP); strcat(body, buffer);

        // get temperature
        float temp = dallas.getTempCByIndex(i);
        if (temp == -127) temp = VALUE_NA;
        //sprintf(tmp, "%f", temp);
        dtostrf(temp, 4, 1, tmp);
        strcat(body, tmp);
        prepare(IX_HTTP_BODY_READING_END); strcat(body, buffer);
    }
    prepare(IX_HTTP_BODY_END); strcat(body, buffer);
}

// send HTTP payload
void sendPayload()
{
    // prepare body in buffer to be able to compute
    // its length when writing HTTP Content-length header
    prepareBody();
    //Serial1.print(body);
    Serial1.print("Body length: ");
    Serial1.println(strlen(body));

    // request line
    prepareAndSend(IX_HTTP_METHOD);
    Serial.print(PUSH_PATH);
    prepareAndSend(IX_HTTP_PROTOCOL);
    prepareAndSend(IX_EOL);

    // headers
    prepareAndSend(IX_HTTP_HEADER_HOST); Serial.print(PUSH_HOST); prepareAndSend(IX_EOL);
    prepareAndSend(IX_HTTP_HEADER_USER_AGENT); prepareAndSend(IX_EOL);
    prepareAndSend(IX_HTTP_HEADER_CONNECTION_CLOSE); prepareAndSend(IX_EOL);
    prepareAndSend(IX_HTTP_HEADER_CONTENT_TYPE); prepareAndSend(IX_EOL);
    prepareAndSend(IX_HTTP_HEADER_CONTENT_LENGTH);
    Serial.print(strlen(body) + 4); // 4 newlines (not sure, may be 2 additional bytes would be more precise)
    prepareAndSend(IX_EOL);

    // new line to separate headers and body
    prepareAndSend(IX_EOL);

    /// body
    Serial.print(body);

    // two empty lines at the end of the body
    prepareAndSend(IX_EOL);
    prepareAndSend(IX_EOL);

    Serial.print(endcom);
    Serial.print(endcom);
}

void gprs() {
    timer = millis();

    Serial.println("AT");
    delay(500);

    communicate(IX_AT);
    communicate(IX_AT_IPR); //The speed must be set also here!
    communicate(IX_AT_CBC);
    communicate(IX_AT_CSTT);
    if (!success)
    {
        Serial1.println("Not registered in network");
        resetModem();
        return;
    }

    communicate(IX_AT_CIICR); // starts wireless connection with the GPRS, it obtains IP addres from provider
    communicate(IX_AT_CIPSTATUS);
    communicate(IX_AT_CIFSR); // gets the ip address
    modify = true;
    communicate(IX_AT_CIPSTART); // starts TCP connection
    modify = false;
    if (!fail) delay (3000);
    communicate(IX_AT);
    communicate(IX_AT);
    communicate(IX_AT);
    communicate(IX_AT_CIPSEND1); // select data transmitting mode

    //send data
    communicate(IX_AT_CIPSEND);
    if (!fail) {
        delay(2000);
        sendPayload();
        delay(2000);
    }

    communicate(IX_AT_CIPSENDQ);
    communicate(IX_AT_CIPCLOSE);
    communicate(IX_AT_CIPSHUT);
    communicate(IX_AT_CSCLK2);

    if (!success) fail = true;

    if (fail)
    {
        resetModem();
    }
}


void setup() {
   endcom[0] = 0x1a;
   endcom[1] = '\0';

   Serial1.begin(SERBAUD);
   Serial.begin(SIMBAUD);

   dallas.begin();

   gprs();
}

void loop() {
   delay(100);
   if (millis() - timer > wait * 1000) gprs();
}
