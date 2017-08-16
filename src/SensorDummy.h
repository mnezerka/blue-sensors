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
        readings[0].value = DUMMY_VALUE;
        readings[0].address = "dummy";
        readings[1].value = Reading::VALUE_LAST;
    }

    Reading* takeReadings()
    {
        readings[0].value = readings[0].value * -1;
        return getReadings();
    }

    Reading* getReadings()
    {
        return readings;
    }
};

#endif
