#ifndef GPRSCONNECTIVITY_H
#define GPRSCONNECTIVITY_H

#include <SoftwareSerial.h>
#include "DeviceState.h"

#define SERBAUD 9600
#define SIMBAUD 38400
#define SIMTX 12
#define SIMRX 13
#define SIMRESET 14

#define BUFFERSIZE PUSH_AES_BUFFER_SIZE

/*
https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial/
*/
class GprsConnectivity
{
    private:
        SoftwareSerial *sim800;

        char buffer[BUFFERSIZE];
        char voltage[4];
        bool fail = false;
        bool success = false;
        byte comma = 0;

    public:

        GprsConnectivity() {
            sim800 = new SoftwareSerial(SIMRX, SIMTX);
        }

        void begin() {
            DeviceState::getInstance().state("Init GPRS");
            sim800->begin(SIMBAUD);
        }

        void processReadings(const String &push_data) {
            DeviceState::getInstance().state("GPRS - process readings");

            sim800->println("AT"); delay(500);

            communicate("AT");
            communicate("AT+IPR=38400");
            communicate("AT+CBC");
            if (comma < 50)
            {
                voltage[0] = buffer[comma + 1];
                voltage[1] = buffer[comma + 2];
                voltage[1] = buffer[comma + 3];
                voltage[3] = buffer[comma + 4];
            }

            communicate("AT+CSTT=\"internet\",\"\",\"\"");

            if (!success)
            {
                DeviceState::getInstance().debug("Not registered in network");
                resetModem();
                return;
            }

            communicate("AT+CIICR");        // starts wireless connection with the GPRS, it obtains IP addres from provider
            communicate("AT+CIPSTATUS");
            communicate("AT+CIFSR");        // gets the ip address
            communicate("AT+CIPSTART=\"TCP\",\"iot.pavoucek.net\",80");     // starts TCP connection
            if (!fail) delay (3000);

            communicate("AT");
            communicate("AT");
            communicate("AT");
            communicate("AT+CIPQSEND=1");           // select data transmitting mode

            // send data
            String request = payload(push_data);
            communicate("AT+CIPSEND=" + String(request.length()));
            if (!fail) {
                delay(2000);
                DeviceState::getInstance().debug(request);
                sim800->print(request);
                delay(2000);
            }

            communicate("AT+CIPSEND?");
            communicate("AT+CIPCLOSE");
            communicate("AT+CIPSHUT");
            communicate("AT+CSCLK=2");

            if (!success) fail = true;

            if (fail)
            {
                resetModem();
            }
        }

        void resetModem()
        {
            fail = false;
            DeviceState::getInstance().debug("GPRS: Modem reset");
            pinMode(SIMRESET, OUTPUT);
            digitalWrite(SIMRESET, LOW);
            delay(50);
            pinMode(SIMRESET, INPUT_PULLUP);
        }

        void communicate(String command)
        {
            if (!fail) {
                sendCommand(command);
                receiveCommand();
            }
        }

        void sendCommand(String command)
        {
            DeviceState::getInstance().debug("GDPR: >>> " + command);
            sim800->println(command);
        }

        void receiveCommand()
        {
            unsigned long stamp;
            success = false;
            char thischar = 0;
            stamp = millis();
            byte i = 0;

            while (!sim800->available() && millis() < stamp + 5000) {
                delay (50);
            }

            if (millis() > stamp + 5000) fail = true;
            delay (100);

            while (sim800->available()) {
                thischar = sim800->read();
                Serial.write(thischar);

                buffer[i] = thischar;

                // check if OK was read
                if (i > 0 && buffer[i - 1] == 'O' && buffer[i] == 'K') success = true;

                if (buffer[i] == ',') comma = i;

                i++;
                if (i >= BUFFERSIZE) i = 1;
            }

            Serial.println();
        }

        String payload(const String &body)
        {
            String result;
            result += "POST /api/readings HTTP/1.1\r\n";
            result += "Host: iot.pavoucek.net\r\n";
            result += "User-Agent: Esp8266GPRS/1.1\r\n";
            result += "Connection: close\r\n";
            result += "Content-Type: application/x-www-form-urlencoded\r\n";
            result += "Content-Length: ";
            result += (body.length() + 4);
            result += "\r\n\r\n";
            result += body;
            result += "\r\n\r\n";

            return result;
        }

};

#endif
