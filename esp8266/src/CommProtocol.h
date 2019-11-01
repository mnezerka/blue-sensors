#ifndef COMMPROTOCOL_H
#define COMMPROTOCOL_H

#include <blue_aes.h>
#include "Sensors.h"
#include "TimeProvider.h"

class CommProtocol
{
    public:

        void encode(String &data, char *buffer, int buffer_size, int &encoded_size)
        {

            // store data as sequence of chars
            data.toCharArray(buffer, PUSH_AES_BUFFER_SIZE);

            unsigned int push_buffer_content_size = strlen(buffer);

            // encrypt json with readings data
            push_buffer_content_size = blue_aes_pkcs7_encrypt(
                (unsigned char*)buffer,
                push_buffer_content_size,
                PUSH_AES_BUFFER_SIZE,
                (unsigned char*)PUSH_ENCRYPTION_PASSWORD);
        }


        static String readings2Json()
        {
            String result = "";
            result += "{";
            result += "\"device\": \"";
            result +=  DEVICE_ID;
            result += "\", ";

#ifdef ENABLE_WIFI
            result += "\"ip\": \"";
            result += (WiFi.localIP().toString());
            result += "\", ";

            result += "\"wifi-ssid\": \"";
            result += (WiFi.SSID());
            result += "\", ";

            result += "\"wifi-strength\": ";
            result += (WiFi.RSSI());
            result += ", ";
#endif
            result += "\"time\": ";
            result += TimeProvider::getInstance().getUnixTimestamp();
            result += ", ";

            result += "\"readings\": [";

            // get all sensor readings and generate appropriate HTML representation
            for(int i = 0; i < Sensors::getInstance().getSensors()->size(); i++)
            {
                Sensor *sensor = Sensors::getInstance().getSensors()->get(i);
                Reading* reading = sensor->getReadings();
                bool readingCounter = 0;
                while (!reading->isLast)
                {
                    if (readingCounter > 0) result += ",";
                    result += "{\"address\": \"" + reading->address + "\"";
                    if (sensor->providesTemperature())
                    {
                        result += ", \"t\": " + String(reading->temperature);
                    }
                    if (sensor->providesHumidity())
                    {
                        result += ", \"h\": " + String(reading->humidity);
                    }
                    result += "}";
                    reading++;
                    readingCounter++;
                }
            }

            result += "]}";

            return result;
        }
};

#endif
