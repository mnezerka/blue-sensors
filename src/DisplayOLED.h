#ifndef DISPLAYOLED_H
#define DISPLAYOLED_H

#include <Wire.h>
#include "SSD1306.h"
#include "config.h"
#include "DeviceStateListener.h"
#include "TimeProvider.h"
#include "images.h"

class DisplayOLED: public DeviceStateListener
{
    private:
        // declare OLED display
        SSD1306 *oled;
		String lastState;

    public:

    DisplayOLED()
    {
        // display(SDA, SCL);
        // SDA and SCL are the GPIO pins of ESP8266 that are connected to
        // respective pins of display.
        // Initialize the OLED display using Wire library
        // D3 -> SDA
        // D5 -> SCL
        oled = new SSD1306(0x3c, D3, D5);
    }
  
    void begin()
    {

        // initialize display
        oled->init();

        oled->clear();
        oled->flipScreenVertically();
        oled->setFont(ArialMT_Plain_10);
    }

    void state(String state)
    {
		lastState = state;
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

                oled->setTextAlignment(TEXT_ALIGN_LEFT);
                oled->drawString(0, 10 * readingCounter, msg);

                reading++; // move to next reading
                readingCounter++;
            }
        }
    }


    void update()
    {
        oled->clear();

        // update time
        oled->setTextAlignment(TEXT_ALIGN_RIGHT);
        oled->drawString(128, 0, TimeProvider::getInstance().getTimeStr());

        oled->setTextAlignment(TEXT_ALIGN_LEFT);

		// wifi information

		// device state
		oled->drawString(0, 10, "State: " + lastState);

		// wifi information
		oled->drawString(0, 20, "Wifi: " + WiFi.SSID());
		oled->drawString(0, 30, "IP: " + WiFi.localIP().toString());
		oled->drawString(0, 40, "Wifi strength: " + String(WiFi.RSSI()) + " dBm");

        oled->display();
    }
};

#endif
