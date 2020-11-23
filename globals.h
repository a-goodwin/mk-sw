#ifndef GLOBALS_H
#define GLOBALS_H


#ifndef __cplusplus
# define false (0)
# define true (1)
typedef unsigned char bool;
#endif

#define VER_STR "0.16 alpha (new i2c proto - block)"
#define CLKHD "%09u "
#define SOCK_PORT_BASE 7000
#define C_KT_COUNT 4
#define I2C_LOG_FNAME "/var/log/ip2c.log"
#define KPN_LOG_FNAME "/var/log/ip2u.log"

//#define C_UART_NAME "/dev/ttyUSB0"
#define C_GPIO_ACTIVE_VALUE 0

#endif // GLOBALS_H
