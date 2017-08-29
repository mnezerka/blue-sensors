#ifndef DISPLAYOLED_H
#define DISPLAYOLED_H

#include <Wire.h>
#include <OLED.h>
#include "config.h"
#include "DeviceStateListener.h"

class DisplayOLED: public DeviceStateListener
{
    private:
        // declare OLED display
        OLED *oled; 

    public:

    DisplayOLED()
    {
        // display(SDA, SCL);
        // SDA and SCL are the GPIO pins of ESP8266 that are connected to
        // respective pins of display.
        oled = new OLED(D3, D5);
    }
  
    void begin()
    {
        // initialize display
        oled->begin();
    }

    void state(String state)
    {
        oled->clear();
        oled->print((char *)state.c_str(), 0, 0);
    }

    void processReadings(Sensors &sensors)
    {
        //oled->clear();
        //lcd->home();

        // get all sensor readings and generate appropriate HTML representation
        for(int i = 0; i < sensors.getSensors()->size(); i++)
        {
            Sensor *sensor = sensors.getSensors()->get(i);
            Reading* reading = sensor->getReadings();
            bool readingCounter = 1;
            while (reading->value != Reading::VALUE_LAST)
            {
                String msg = String(i) + "-" + String(readingCounter) + ": " + String(reading->value) + " C";
				oled->print((char*)msg.c_str(), readingCounter, 0);

                reading++; // move to next reading
                readingCounter++;
            }
        }
    }

};

#endif
