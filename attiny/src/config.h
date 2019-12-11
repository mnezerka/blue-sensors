#ifndef CONFIG_H
#define CONFIG_H

// device identification (for various purposes)
#ifndef DEVICE_ID
#define DEVICE_ID "device001"
#endif

// define interval for reading data from sensors (in seconds)
#ifndef INTERVAL_READINGS
#define INTERVAL_READINGS 60
#endif

#ifndef PUSH_HOST
#define PUSH_HOST "some.domain.org"
#endif

#ifndef PUSH_PORT
#define PUSH_PORT 80
#endif

#ifndef PUSH_PATH
#define PUSH_PATH "/path/to/handler"
#endif

#define SDA_PORT PORTA
#define SCL_PORT PORTA
#define SDA_PIN 4
#define SCL_PIN 1


#endif
