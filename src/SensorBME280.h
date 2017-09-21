#ifndef SENSORBME280_H
#define SENSORBME280_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "config.h"
#include "Sensor.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

class SensorBME280 : public Sensor
{
    private:
        int bmeCount = 0; // number of available sensors
        Adafruit_BME280 *bme = NULL;
        Reading readings[2];
    
    public:

        bool providesTemperature() { return true; };
        bool providesHumidity() { return true; };
        bool providesPressure() { return true; };

		void begin()
		{
            //Wire.begin(D3, D4);
            Wire.begin(WIRE_SDA, WIRE_SCL);
			bme = new Adafruit_BME280();

			DeviceState::getInstance().state("Init BME280");

			bool status = bme->begin(SENSOR_BMA280_ADDR);  
			if (!status) {
				DeviceState::getInstance().debug("Could not find a valid BME280 sensor, check wiring!");
			} else {
                bmeCount = 1;
            }
		}

		Reading* takeReadings()
		{
			return getReadings();
		}

		Reading* getReadings()
		{
            if (bmeCount == 0)
            {
                readings[0].isLast = true;
            } else
            {
                readings[0].address = "BMA-";
                readings[0].address += DEVICE_ID;
                readings[0].address += "-" + String(SENSOR_BMA280_ADDR);
                readings[0].temperature = bme->readTemperature();
                readings[0].pressure = bme->readPressure();
                readings[0].humidity = bme->readHumidity();
                readings[1].isLast = true;
            }
			return readings;
		}
};

#endif
