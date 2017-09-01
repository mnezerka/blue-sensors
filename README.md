# Blue Sensors
Implementation of simple sensors (e.g. temperature) reader and publisher

## Boards 

All boards are based on ESP8266 chip.

### nodemcuv2

https://www.aliexpress.com/item/1pcs-V3-4M-4FLASH-NodeMcu-Lua-WIFI-Networking-development-board-Based-ESP8266/32787382982.html?spm=a2g0s.9042311.0.0.1w6xrw


### Wemos D1 Mini

https://wiki.wemos.cc/products:d1:d1_mini

https://www.aliexpress.com/item/D1-mini-V1-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/32787418018.html?spm=a2g0s.9042311.0.0.1w6xrw

## Peripherials 

This is list of supported devices that can be connected and configured via build flags.

### Temperature Sensor Dallas (DS18B20)
One Wire digital temperature sensor from Maxim IC. Reports degrees C with 9 to
12-bit precision, measures temperature from -55C to 125C, accuracty from -10C
to +85C is +/-0.5C. Each sensor has a unique 64-Bit Serial number etched into
it - allows for a huge number of sensors to be used on one data bus. 

https://www.aliexpress.com/item/100-New-and-original-DALLAS-DS18B20-TO-92-IC-free-shipping/1327754867.html?spm=a2g0s.9042311.0.0.1w6xrw

```

               _____
              /     \   
             /       \   
            +---------+
            |         |
            |         |
            |         |
            +---------+
             |   |   |
         GND |   |   | VCC
             | DATA  |
             |   |   +------------+
             |   |                |
             |   |                |
             |   | +----------+   |
             |   +-+ 4700 ohm +---+
             |   | +----------+   |
             |   |                |
             |   |                |
            GND  DX              3V

```
4700 ohm resistor (bar colors: yello, blue, red and white) is needed



### Waterproof Temperature Sensor Dallas (DS18B20) 
Stainless Steel Package 1 Meters Waterproof DS18b20 Temperature Probe Temperature Sensor 18B20
https://www.aliexpress.com/item/Stainless-Steel-Package-1-Meters-Waterproof-DS18b20-Temperature-Probe-Temperature-Sensor-18B20/32359386131.html?spm=a2g0s.9042311.0.0.1w6xrw

```
                                       +--------------+
        --- red --------+--------------+  waterproof  |
        --- yellow -----|  cable (1m)  |  DS18B20     |
        --- green  -----+--------------+              |
                                       +---------------
```

### LCD I2C Display

https://www.aliexpress.com/item/IIC-I2C-TWI-Serial-LCD-2004-20x4-Display-Shield-Blue-Backlight-for-Arduino-Free-Shipping/32599904427.html?spm=a2g0s.9042311.0.0.1w6xrw

### OLED ICC Display

https://www.aliexpress.com/item/1pcs-Free-Shipping-White-Blue-White-and-Blue-color-0-96-inch-128X64-OLED-Display-Module/32717950155.html?spm=a2g0s.9042311.0.0.1w6xrw

Wiring:

```
                         +---------------+
   GND <-> GND           |               |
   VDD <-> 3V3           |     OLED      |
   SCK <-> D5            |    Display    |
   SDA <-> D3            |               |
                         +---------------+
                           |   |   |   |
                          GND VDD SCK SDA
                           .   .   .   .
                           .   ................................      
                           .       .   .                      . 
                           ................................   .
                                   .   .                  .   .
                                   .   .                  .   .
                   .....................                  .   .
                   .               .                      .   .
                  D3              D5                     GND 3V3
       |   |   |   |   |   |   |   |   |   |   |   |  |   |   |
      +---------------------------------------------------------+
      |                                                         |
      |                                                 FLASH O |
      |                                                         |
      |                                                         |
      |                                                     +---+
      |                                                     |   | USB
      |                                                     |   |
      |                                                     +---+
      |                                                         |
      |                                                  RST  O |
      |                                                         |
      +---------------------------------------------------------+
       |   |   |   |   |   |   |   |   |   |   |   |  |   |   |
```

## Build Variants
It is possible to configure buld variant via build flags. 

```
export PLATFORMIO_BUILD_FLAGS=-DENABLE_DUMMY_SENSOR
export PLATFORMIO_BUILD_FLAGS='-DENABLE_DUMMY_SENSOR -DPUSH_URL=\"http://192.168.0.11:9200/api/readings\"'
```

Available flags:
- `ENABLE_DUMMY_SENSOR` - enable dummy temperature sensor which simulates thermo sensor 
- `ENABLE_DALLAS_SENSOR` - enable dallas onewire sensors
- `DEVICE_ID` - device identification string
- `INTERVAL_READINGS` - interval (ms) for reading and pushing sensor readings
- `PUSH_URL` - url of the api for processing of pushed readings
- `PUSH_ENCRYPTION_PASSWORD` - password to be used for http content encryption (shall have 16 characters)
- `PUSH_AES_BUFFER_SIZE` - size of the buffer for complete content sent to the server (default 1024)
