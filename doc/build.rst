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
