# ATTiny

## ATTiny 1634

8 MHz internal clock - burn bootloader

Programmer: USB-ASP - https://docs.platformio.org/en/latest/platforms/atmelavr.html

## Build

Tailor build by setting flags, e.g.::

```
export PLATFORMIO_BUILD_FLAGS=-DINTERVAL_READINGS=120
```

Build and flash to attiny:

```
pio run -t upload

```

Check state of the device by observing serial console:

```
pio device monitor
```

## Troubleshoting

- Not enough permissions to access usb device:
  https://andreasrohner.at/posts/Electronics/How-to-fix-device-permissions-for-the-USBasp-programmer/

