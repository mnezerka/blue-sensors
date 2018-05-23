Building
========

Building and deployment (flashing) of software is managed by platform.io
framework.  It is possible to configure buld variant via build flags. One of the possibilities for providing
flags is environment are provided as environment variables::

    export PLATFORMIO_BUILD_FLAGS=-DENABLE_DUMMY_SENSORS
    export PLATFORMIO_BUILD_FLAGS='-DENABLE_DUMMY_SENSORS -DPUSH_URL=\"http://192.168.0.11:9200/api/readings\"'

Available flags:

- `ENABLE_DUMMY_SENSOR` - enable dummy temperature sensor which simulates thermo sensor 
- `ENABLE_DALLAS_SENSORS` - enable dallas onewire sensors
- `ENABLE_BME280_SENSORS` - enable BME280 sensor
- `DEVICE_ID` - device identification string
- `INTERVAL_READINGS` - interval (ms) for reading and pushing sensor readings
- `PUSH_URL` - url of the api for processing of pushed readings
- `PUSH_ENCRYPTION_PASSWORD` - password to be used for http content encryption (shall have 16 characters)
- `PUSH_AES_BUFFER_SIZE` - size of the buffer for complete content sent to the server (default 1024)
- `ONE_WIRE_BUS` - GPIO pin to which is connected one wire data cable (for Dallas sensors)
- `WIRE_SDA` - GPIO pin to which is connected I2C bus SDA
- `WIRE_SCL` - GPIO pin to which is connected I2C bus SCL
- `SENSOR_BMA_ADDR` - address for BMA sensor

Step By Step
------------

List available devices::

    pio device list

Device connected to usb should be in list (in our case `ttyUSB0`)::

    /dev/ttyS0
    ----------
    Hardware ID: PNP0501
    Description: ttyS0

    /dev/ttyUSB0
    ------------
    Hardware ID: USB VID:PID=1A86:7523 LOCATION=1-1.3
    Description: USB2.0-Serial

Tailor build by setting flags, e.g.::

    export PLATFORMIO_BUILD_FLAGS=-DENABLE_DALLAS_SENSORS

Build and falsh to selected environment (`d1_mini` in our case)::
    
    pio run -e d1_mini -t upload

Check state of the device by observing serial console::

    pio device monitor

Troubleshooting
---------------
Try I2C scanner in case of inactive I2C device:
https://github.com/mnezerka/arduino/tree/master/i2c_scanner
