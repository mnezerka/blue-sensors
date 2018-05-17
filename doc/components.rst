Components
==========

Implementation of simple sensors (e.g. temperature) reader and publisher

Boards 
------

All boards are based on ESP8266 chip.

nodemcuv2
.........

https://www.aliexpress.com/item/1pcs-V3-4M-4FLASH-NodeMcu-Lua-WIFI-Networking-development-board-Based-ESP8266/32787382982.html?spm=a2g0s.9042311.0.0.1w6xrw


Wemos D1 Mini
.............

https://wiki.wemos.cc/products:d1:d1_mini

https://www.aliexpress.com/item/D1-mini-V1-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/32787418018.html?spm=a2g0s.9042311.0.0.1w6xrw

PCB
...
Universal PCB board (4x6cm)
https://www.aliexpress.com/item/5PCS-Double-side-Prototype-PCB-Tinned-Universal-board-4x6-4-6cm/32829197097.html?spm=a2g0s.9042311.0.0.3da24c4d2oyrgA


Peripherials 
------------

This is list of supported devices that can be connected and configured via build flags.

Temperature Sensor Dallas (DS18B20)
...................................
One Wire digital temperature sensor from Maxim IC. Reports degrees C with 9 to
12-bit precision, measures temperature from -55C to 125C, accuracty from -10C
to +85C is +/-0.5C. Each sensor has a unique 64-Bit Serial number etched into
it - allows for a huge number of sensors to be used on one data bus. 

https://www.aliexpress.com/item/100-New-and-original-DALLAS-DS18B20-TO-92-IC-free-shipping/1327754867.html?spm=a2g0s.9042311.0.0.1w6xrw

::

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


4700 ohm resistor (bar colors: yellow, blue, red and white) is needed



Waterproof Temperature Sensor Dallas (DS18B20) 
..............................................
Stainless Steel Package 1 Meters Waterproof DS18b20 Temperature Probe Temperature Sensor 18B20
https://www.aliexpress.com/item/Stainless-Steel-Package-1-Meters-Waterproof-DS18b20-Temperature-Probe-Temperature-Sensor-18B20/32359386131.html?spm=a2g0s.9042311.0.0.1w6xrw

::

                                       +--------------+
        --- red --------+--------------+  waterproof  |
        --- yellow -----|  cable (1m)  |  DS18B20     |
        --- green  -----+--------------+              |
                                       +---------------

BME280 I2C Sensor
.................

I2C address can be 0x76 or 0x77 (use I2C scanner to detect right value)

Wiring::

    GND <-> GND
    VCC <-> 3V
    SDA <-> D3
    SCL <-> D4

LCD I2C Display
...............

https://www.aliexpress.com/item/IIC-I2C-TWI-Serial-LCD-2004-20x4-Display-Shield-Blue-Backlight-for-Arduino-Free-Shipping/32599904427.html?spm=a2g0s.9042311.0.0.1w6xrw

OLED ICC Display
................

https://www.aliexpress.com/item/1pcs-Free-Shipping-White-Blue-White-and-Blue-color-0-96-inch-128X64-OLED-Display-Module/32717950155.html?spm=a2g0s.9042311.0.0.1w6xrw

Wiring::

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

Dupont
------

Crimping Tool
.............

https://www.aliexpress.com/item/New-Arrival-SN-28B-Pin-Crimping-Tool-Crimping-Plier-2-54mm-3-96mm-28-18AWG/32649914049.html?spm=a2g0s.9042311.0.0.3da24c4d2oyrgA

Various connectors
..................

https://www.aliexpress.com/item/Free-shiiping-Hot-Sale10pcs-40-Pin-1x40-Single-Row-Male-2-54mm-Breakable-Pin-Header-Connector/32699944798.html?spm=a2g0s.9042311.0.0.3da24c4d2oyrgA
https://www.aliexpress.com/item/620pcs-A-Set-Dupont-Wire-Jumper-Pin-Header-Connector-Housing-Kit-Male-Crimp-Pins-Female-Pin/32736024614.html?spm=a2g0s.9042311.0.0.3da24c4d2oyrgA
https://www.aliexpress.com/item/120Pcs-Lot-10cm-30cm-Color-Ribbon-Line-Breadboard-Dupont-Cable-Jump-Jumper-Wire-Male-to-Male/32798011999.html?spm=a2g0s.9042311.0.0.3da24c4d2oyrgA
https://www.aliexpress.com/item/10pcs-2-54mm-1x40-Pin-Right-Angle-Single-Row-Female-Pin-Header-Connector-Strip-Bending/32326267382.html?spm=a2g0s.9042311.0.0.27424c4do7p8SV

