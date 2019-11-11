#ifndef READINGS_HTTP_UPLOADER_H
#define READINGS_HTTP_UPLOADER_H

#include <ESP8266HTTPClient.h>
#include <blue_aes.h>
#include "config.h"
#include "Sensors.h"
#include "DeviceState.h"
#include "TimeProvider.h"

class ReadingsHttpUploader
{
    public:

    void processReadings()
    {
        HTTPClient http;
        String url = PUSH_URL;
        String logPrefix = "[HTTP] ";

        // buffer used for preparation of data to push over http
        char push_buffer[PUSH_AES_BUFFER_SIZE];

        DeviceState::getInstance().debug(logPrefix + "POST to " + url);

        http.begin(url);
        http.addHeader("Content-Type", "application/octet-stream");

        // get all readings in json format and store is as sequence of chars
        readings2Json().toCharArray(push_buffer, PUSH_AES_BUFFER_SIZE);

        unsigned int push_buffer_content_size = strlen(push_buffer);

        // encrypt json with readings data
        if (PUSH_ENCRYPTION_ENABLED)
        {
            DeviceState::getInstance().debug(logPrefix + "POST encryption enabled, encrypting content");

            push_buffer_content_size = blue_aes_pkcs7_encrypt(
                (unsigned char*)push_buffer,
                (unsigned int)strlen(push_buffer),
                PUSH_AES_BUFFER_SIZE,
                (unsigned char*)PUSH_ENCRYPTION_PASSWORD);
        }

        // initiate HTTP POST request
        int httpCode = http.POST((unsigned char*)push_buffer, push_buffer_content_size);

        // httpCode will be negative on error
        if(httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            DeviceState::getInstance().debug(logPrefix + "POST status code: " + httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                DeviceState::getInstance().debug(logPrefix + payload);
            }
        } else
        {
            DeviceState::getInstance().debug(logPrefix + "POST failed, error: " + http.errorToString(httpCode));
        }

        http.end();
    }

    private:

    String readings2Json()
    {
        String result = "";
        result += "{";
        result += "\"device\": \"";
        result +=  DEVICE_ID;
        result += "\", ";

        result += "\"ip\": \"";
        result += (WiFi.localIP().toString());
        result += "\", ";

        result += "\"wifi-ssid\": \"";
        result += (WiFi.SSID());
        result += "\", ";

        result += "\"wifi-strength\": ";
        result += (WiFi.RSSI());
        result += ", ";

        /*
        result += "\"time\": ";
        result += TimeProvider::getInstance().getUnixTimestamp();
        result += ", ";
        */

        result += "\"readings\": [";

        // get all sensor readings and generate appropriate HTML representation
        int readingCounter = 0;
        for(int i = 0; i < Sensors::getInstance().getSensors()->size(); i++)
        {
            Sensor *sensor = Sensors::getInstance().getSensors()->get(i);
            Reading* reading = sensor->getReadings();
            while (!reading->isLast)
            {
                if (readingCounter > 0) {
                    result += ",";
                }
                result += "{\"address\": \"" + reading->address + "\"";
                if (sensor->providesTemperature())
                {
                    result += ", \"t\": " + String(reading->temperature);
                }
                if (sensor->providesHumidity())
                {
                    result += ", \"h\": " + String(reading->humidity);
                }
                if (sensor->providesPressure())
                {
                    result += ", \"p\": " + String(reading->pressure);
                }

                result += "}";
                reading++; // move pointer to the next reading
                readingCounter++;
            }
        }

        result += "]}";

        DeviceState::getInstance().debug(result);

        return result;
    }

    // helper method for printing array of bytes
    /*
    void printArray(String name, byte *arr, int length)
    {
        Serial.print(name + ": ");
        for (int i = 0; i < length; i++)
        {
            Serial.write(arr[i]);
        }
        Serial.println();
    }
    */
};

#endif
