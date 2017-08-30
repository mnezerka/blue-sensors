#ifndef READINGS_HTTP_UPLOADER_H
#define READINGS_HTTP_UPLOADER_H

#include <ESP8266HTTPClient.h>
//#include <AES.h>
//#include <ebase64.h>
//#include <libb64/cencode.h>

#include "config.h"
#include "Sensors.h"
#include "DeviceState.h"

class ReadingsHttpUploader
{
    public:

    void processReadings()
    {
        HTTPClient http;
        //String url = "http://blue.pavoucek.cz";
        String url = PUSH_URL;
        String logPrefix = "[HTTP] ";

        // configure traged server and url
        http.begin(url); //HTTP

        http.addHeader("Content-Type", "application/json");

        DeviceState::getInstance().debug(logPrefix + "POST to " + url);

        //int httpCode = http.POST(encodePayload(readings2Json(sensors)));
        int httpCode = http.POST(readings2Json());

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

        result += "\"readings\": [";
    
        // get all sensor readings and generate appropriate HTML representation
        for(int i = 0; i < Sensors::getInstance().getSensors()->size(); i++)
        {
            Sensor *sensor = Sensors::getInstance().getSensors()->get(i);
            Reading* reading = sensor->getReadings();
            bool readingCounter = 0;
            while (reading->value != Reading::VALUE_LAST)
            {
                // Serial.println(reading->address + ": " + String(reading->value) + "C");
                if (readingCounter > 0) result += ",";
                result += "{\"address\": \"" + reading->address + "\", \"value\": " + reading->value + "}";
                reading++;
                readingCounter++;
            }
        }
        
        result += "]}";

        DeviceState::getInstance().debug(result);
        
        return result;
    }

    String encodePayload(String payload)
    {
        return payload;
    }

	// helper method for printing array of bytes
	void printArray(String name, byte *arr, int length)
	{
		Serial.print(name + ": ");
		for (int i = 0; i < length; i++)
		{
			Serial.write(arr[i]);
		}
		Serial.println();
	}

};

#endif
