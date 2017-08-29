#ifndef CONFIG_H 
#define CONFIG_H

// device identification (for various purposes)
#ifndef DEVICE_ID
#define DEVICE_ID "thermo";
#endif

// data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0

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

// password used for encyption of 
#ifndef PUSH_ENCRYPTION_PASSWORD
#define PUSH_ENCRYPTION_PASSWORD "mysecret"
#endif

// size of the encryption buffer (expeced size of pkcs7 padded push message)
#ifndef PUSH_AES_BUFFER_SIZE
#define PUSH_AES_BUFFER_SIZE 1024
#endif

// time zone offset in seconds
#ifndef TIME_ZONE_OFFSET
#define TIME_ZONE_OFFSET 60 * 60 * 2
#endif

#endif
