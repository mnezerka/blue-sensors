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

// push HOST
#ifndef PUSH_HOST
#define PUSH_HOST "some.domain.org"
#endif

// push PATH
#ifndef PUSH_PATH
#define PUSH_PATH "/path/to/handler"
#endif



#endif
