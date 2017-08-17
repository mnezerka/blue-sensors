# Blue Sensors
Implementation of simple sensors (e.g. temperature) reader and publisher

## Build Variants
It is possible to configure buld variant via build flags. 

```
export PLATFORMIO_BUILD_FLAGS=-DENABLE_DUMMY_SENSOR
```

Available flags:
- `ENABLE_DUMMY_SENSOR` - enable dummy temperature sensor which simulates thermo sensor 
- `ENABLE_DALLAS_SENSOR` - enable dallas onewire sensors
- `DEVICE_ID` - device identification string
- `INTERVAL_READINGS` - interval (ms) for reading and pushing sensor readings
- `PUSH_URL` - url of the api for processing of pushed readings
- `PUSH_ENCRYPTION_PASSWORD` - password to be used for http content encryption (shall have 16 characters)
- `PUSH_AES_BUFFER_SIZE` - size of the buffer for complete content sent to the server (default 1024)
