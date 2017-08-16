#ifndef SENSOR_H
#define SENSOR_H

class Reading
{
  public:
    static const int VALUE_NA = -8888;
    static const int VALUE_LAST = -9999;
    
    String address;
    float value;

    Reading() : address(""), value(VALUE_NA) {}
};

class Sensor
{
  public:
    //Sensor(DeviceState *deviceState);
    virtual void begin() {};

    // force sensor(s) to take readings
    virtual Reading* takeReadings();

    // get last measured readings
    virtual Reading* getReadings();
};

#endif
