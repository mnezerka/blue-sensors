#ifndef SENSOR_H
#define SENSOR_H

class Reading
{
  public:
    static const int VALUE_NA = -8888;

    String address;
    float temperature;
    float humidity;
    float pressure;
    bool isLast;
    Reading() : address(""), temperature(VALUE_NA), humidity(VALUE_NA), pressure(VALUE_NA), isLast(false) {}
};

class Sensor
{
  public:
    virtual void begin() {};

    // force sensor(s) to take readings
    virtual Reading* takeReadings();

    // get last measured readings
    virtual Reading* getReadings();

    virtual bool providesTemperature() { return false; };
    virtual bool providesHumidity() { return false; };
    virtual bool providesPressure() { return false; };
};

#endif
