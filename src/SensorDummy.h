#ifndef SENSORDUMMY_H
#define SENSORDUMMY_H

#include "Sensor.h"

#define DUMMY_VALUE 8

class SensorDummy: public Sensor
{
    private:

    int lastValue = DUMMY_VALUE;
    Reading readings[2];

    public:

    SensorDummy()
    {
        readings[0].temperature = DUMMY_VALUE;
        readings[0].address = "dummy";
        readings[1].isLast = true;
    }

    Reading* takeReadings()
    {
        readings[0].temperature = readings[0].temperature * -1;
        return getReadings();
    }

    Reading* getReadings()
    {
        return readings;
    }
};

#endif
