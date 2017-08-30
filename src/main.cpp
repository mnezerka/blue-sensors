#include <SPI.h> // this needs to be included to fix problem with complilation
#include <Wire.h>
#include <Timer.h> // http://playground.arduino.cc/Code/Timer#Installation (https://github.com/JChristensen/Timer)
#include <OneWire.h>
#include "config.h"
#include "WifiConnectivity.h"
#include "ThermoHttpServer.h"
#include "DeviceState.h"
#include "SerialWriter.h"
#include "ReadingsHttpUploader.h"
#include "Lcd.h"
#include "DisplayOLED.h"
#include "Sensors.h"
#include "SensorDallas.h"
#include "SensorDummy.h"
#include "TimeProvider.h"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Instance of global timer
Timer t;

// Instance of writer to serial interface
SerialWriter serialWriter;

// Instance of readings uploader (http client)
ReadingsHttpUploader readingsHttpUploader;

// Instance LCD display
Lcd lcd;

// Instance of OLED display
DisplayOLED oled;

// Instance of Dallas sensor interface
#ifdef ENABLE_DALLAS_SENSORS
SensorDallas dallas(&oneWire);
#endif

#ifdef ENABLE_DUMMY_SENSOR 
SensorDummy dummy;
#endif

// Instance of wifi connectivity wrapper
WifiConnectivity wifiConnectivity;

// Instance of the simple web server
ThermoHttpServer httpServer;

void takeReadings();
void handleHttpClient();
void handleDeviceState();

void setup(void)
{
    // init device state listeners
    DeviceState::getInstance().addListener(&serialWriter);
    DeviceState::getInstance().addListener(&lcd);
    DeviceState::getInstance().addListener(&oled);
    DeviceState::getInstance().begin();

    // init time provider
    TimeProvider::getInstance().begin();

    // init all sensors
#ifdef ENABLE_DALLAS_SENSORS
    Sensors::getInstance().addSensor(&dallas);
#endif

#ifdef ENABLE_DUMMY_SENSOR 
    Sensors::getInstance().addSensor(&dummy);
#endif

    Sensors::getInstance().begin();

    // init wifi manager
    wifiConnectivity.begin();

    // init http server
    httpServer.begin();

    // configure control loop
    DeviceState::getInstance().state("Control loop");
    t.every(INTERVAL_READINGS, takeReadings);
    t.every(100, handleHttpClient);
    t.every(800, handleDeviceState);
}

//////////////////////////// LOOP /////////////////////////////

void loop(void)
{ 
    TimeProvider::getInstance().update();

    t.update();
}

void handleHttpClient()
{
    httpServer.handleClient();
}

void handleDeviceState()
{
    DeviceState::getInstance().update();
}

void takeReadings()
{
    // take readings of all registered sensors
    Sensors::getInstance().takeReadings();

    serialWriter.processReadings();

    readingsHttpUploader.processReadings();

    lcd.processReadings();

    oled.processReadings();
}
