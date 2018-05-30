#ifndef CONFIG_H 
#define CONFIG_H

// device identification (for various purposes)
#ifndef DEVICE_ID
#define DEVICE_ID "thermo";
#endif

// data wire is plugged into port 2 on the Arduino
#ifndef ONE_WIRE_BUS
#define ONE_WIRE_BUS 4
#endif

// define interval for reading data from sensors (in miliseconds)
#define INTERVAL_READINGS 10000

// LCD display settings
#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

// push URL
#ifndef PUSH_URL
#define PUSH_URL "http://192.168.0.12:9200/api/readings"
#endif

// enable encryption of pushed data
#ifndef PUSH_ENCRYPTION_ENABLED
#define PUSH_ENCRYPTION_ENABLED 0
#endif

// password used for encyption
#ifndef PUSH_ENCRYPTION_PASSWORD
#define PUSH_ENCRYPTION_PASSWORD "xyxyxyxyxyxyxyxy"
#endif


// size of the encryption buffer (expeced size of pkcs7 padded push message)
#ifndef PUSH_AES_BUFFER_SIZE
#define PUSH_AES_BUFFER_SIZE 1024
#endif

// time zone offset in seconds
#ifndef TIME_ZONE_OFFSET
#define TIME_ZONE_OFFSET 60 * 60 * 2
#endif

// Wire (I2C) settings common for all I2C devices
#ifndef WIRE_SDA
#define WIRE_SDA D3
#endif

#ifndef WIRE_SCL
#define WIRE_SCL D4
#endif

#ifndef SENSOR_BMA280_ADDR
#define SENSOR_BMA280_ADDR 0x76
#endif

#endif
