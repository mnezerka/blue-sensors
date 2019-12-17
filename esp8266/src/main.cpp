//#include <SPI.h> // this needs to be included to fix problem with complilation
//#include <Wire.h>
#include <Timer.h> // http://playground.arduino.cc/Code/Timer#Installation (https://github.com/JChristensen/Timer)
#include <OneWire.h>
#include "config.h"

#ifdef ENABLE_WIFI
#include "WifiConnectivity.h"
#include "WifiHttpUploader.h"
#else
#include <ESP8266WiFi.h>
#endif

#ifdef ENABLE_GPRS
#include "GprsConnectivity.h"
#endif

#ifdef ENABLE_LOCAL_HTTP_SERVER
#include "ThermoHttpServer.h"
#endif

#include "DeviceState.h"
#include "SerialWriter.h"

#include "Sensors.h"
#include "SensorDallas.h"
#include "SensorBME280.h"
#include "SensorDummy.h"

#ifdef ENABLE_TIME
#include "TimeProvider.h"
#endif

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Instance of global timer
Timer t;

// Instance of writer to serial interface
SerialWriter serialWriter;


#ifdef ENABLE_WIFI
// Instance of readings uploader (http client)
WifiHttpUploader wifiHttpUploader;
#endif

// Instance of Dallas sensor interface
#ifdef ENABLE_DALLAS_SENSORS
SensorDallas dallas(&oneWire);
#endif

// Instance of BME280 sensor interface
#ifdef ENABLE_BME280_SENSORS
SensorBME280 bme;
#endif

#ifdef ENABLE_DUMMY_SENSOR
SensorDummy dummy;
#endif

#ifdef ENABLE_WIFI
// Instance of wifi connectivity wrapper
WifiConnectivity wifiConnectivity;
#endif

#ifdef ENABLE_GPRS
// Instance of gprs connectivity wrapper
GprsConnectivity gprs;
#endif

// Instance of the simple web server
#ifdef ENABLE_LOCAL_HTTP_SERVER
ThermoHttpServer httpServer;
#endif

void takeReadings();
void handleHttpClient();
void handleDeviceState();

void setup(void)
{
    //Wire.begin(WIRE_SDA, WIRE_SCL);
    //Wire.begin(D3, D4);
    //Wire.setClock(100000);

    // init device state listeners
    DeviceState::getInstance().addListener(&serialWriter);

    DeviceState::getInstance().begin();

#ifdef ENABLE_TIME
    // init time provider
    TimeProvider::getInstance().begin();
#endif

    // init all sensors
#ifdef ENABLE_DALLAS_SENSORS
    Sensors::getInstance().addSensor(&dallas);
#endif

#ifdef ENABLE_BME280_SENSORS
    Sensors::getInstance().addSensor(&bme);
#endif

#ifdef ENABLE_DUMMY_SENSOR 
    Sensors::getInstance().addSensor(&dummy);
#endif

    Sensors::getInstance().begin();

#ifdef ENABLE_WIFI
    // init wifi manager
    wifiConnectivity.begin();
#else
    WiFi.mode(WIFI_OFF);
#endif

#ifdef ENABLE_GPRS
    // init gdpr
    gprs.begin();
#endif

#ifdef ENABLE_LOCAL_HTTP_SERVER
    // init http server
    httpServer.begin();
#endif

    // configure control loop
    DeviceState::getInstance().state("Control loop");
    t.every(INTERVAL_READINGS * 1000, takeReadings);

#ifdef ENABLE_LOCAL_HTTP_SERVER
    t.every(100, handleHttpClient);
#endif

    t.every(800, handleDeviceState);
}

//////////////////////////// LOOP /////////////////////////////

int loopFirst = true;

void loop(void)
{
    if (loopFirst) {
        loopFirst = false;
        takeReadings();
    }

#ifdef ENABLE_TIME
    TimeProvider::getInstance().update();
#endif

    t.update();
}

#ifdef ENABLE_LOCAL_HTTP_SERVER
void handleHttpClient()
{
    httpServer.handleClient();
}
#endif

void handleDeviceState()
{
    DeviceState::getInstance().update();
}

void takeReadings()
{
    // take readings of all registered sensors
    Sensors::getInstance().takeReadings();

    serialWriter.processReadings();

#ifdef ENABLE_WIFI
    wifiHttpUploader.processReadings();
#endif

    //String push_data = CommProtocol::readings2Json();
    //DeviceState::getInstance().debug(push_data);

    //gprs.processReadings(push_data);
}
