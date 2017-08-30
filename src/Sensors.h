#ifndef SENSORS_H
#define SENSORS_H

#include "LinkedList.h"
#include "DeviceState.h"
#include "Sensor.h"

class Sensors
{
    private:
        LinkedList<Sensor*> mySensors;

        Sensors()
        {
          mySensors = LinkedList<Sensor*>();
        }
    
    public:

        static Sensors& getInstance()
        {
            static Sensors instance; // guaranteed to be destroyed.
                                     // instantiated on first use.
            return instance;
        }

        // technique of deleting the methods we don't want.
        Sensors(Sensors const&) = delete;
        void operator=(Sensors const&)  = delete;
 
    
        void addSensor(Sensor* sensor)
        {
            mySensors.add(sensor);
        }

        LinkedList<Sensor*> *getSensors() { return &mySensors; }

        void begin()
        {
            DeviceState::getInstance().state("Init sensors");
            
            for(int i = 0; i < mySensors.size(); i++)
            {
                Sensor *sensor = mySensors.get(i);
                sensor->begin();
            }
        }

        void takeReadings()
        {
            for(int i = 0; i < mySensors.size(); i++)
            {
                mySensors.get(i)->takeReadings();
            }
        }
};

#endif
